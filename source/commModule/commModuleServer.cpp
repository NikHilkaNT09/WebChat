#pragma once

#include <functional>
#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/frame.hpp>
#include <websocketpp/server.hpp>
#include <nlohmann/json.hpp>

#include <iostream>
#include <set>

#include "/home/nikhil/devel/vsCode/MPGame/source/common.h"


using json = nlohmann::json;
// using websocketpp::lib::placeholders::_1;

struct User{
std::string passWord;
std::string status;
};

class CommModuleServer{
public:
    using m_masterServer = websocketpp::server<websocketpp::config::asio>;

    explicit CommModuleServer(){
        establishServer();
    };

    void establishServer(){
        init(),
        setHandlers();
        startServer();
    }
private:
    std::string m_tag = "Server[9003]:\t";
    m_masterServer m_server;

    /** 
    *connection,
    *user 
    */
    std::map<websocketpp::connection_hdl, std::string, 
                std::owner_less<websocketpp::connection_hdl>> m_sessions;

    std::unordered_map<std::string, std::vector<json>> offlineMessages;
    std::unordered_map<std::string, websocketpp::connection_hdl> onlineUsers;

    std::unordered_map<std::string, User> users= {
    {"nikhil", {"1234", "offline"}},
    {"bob",   {"1234", "offline"}}
    };


    void init(){
        std::cout << m_tag << "Initialising server..\n";
        m_server.init_asio();
    }

    void startServer(){
        websocketpp::lib::error_code errCode;
        // system("fuser -k 9003/tcp");
        m_server.listen(9003);
        m_server.start_accept();
        // std::thread serverThread([this]() {
            m_server.run();
        // });
        // serverThread.detach();

    }
    void setHandlers(){
        std::cout << m_tag << "Setting handlers..\n";
        m_server.set_open_handler([this](websocketpp::connection_hdl hdl){
            onOpen(hdl);
        });
        m_server.set_close_handler([this](websocketpp::connection_hdl hdl){
            onClose(hdl);
        });
        m_server.set_message_handler([this](websocketpp::connection_hdl hdl, m_masterServer::message_ptr msg){
            onMessage(hdl, msg);
        });

        std::cout << m_tag << "Handlers are Set\n";
    }

    void broadCast(const json &msg){
        for(auto &[conn, user]: m_sessions){
            if (msg.contains(JSON_KEYS::TEXT)) {
                std::cout << m_tag << " >>> " << msg[JSON_KEYS::TEXT] << std::endl;
            }
            if(msg.contains(JSON_KEYS::TO_USER) ){
                if( user == msg[JSON_KEYS::TO_USER] && users[user].status == "online")
                    m_server.send(conn, msg.dump(), websocketpp::frame::opcode::text);
                else{
                    offlineMessages[msg[JSON_KEYS::TO_USER]].push_back(msg);
                    std::cout << "__pushing data in vector__::" << msg << std::endl;
                }
            }
 
        }
    }

    void onOpen(websocketpp::connection_hdl hdl){
        std::cout << m_tag << "Client Connected" ;
    }

    void onClose(websocketpp::connection_hdl hdl){
        std::cout << m_tag << "Connection Closed";

        if(m_sessions.count(hdl)){
            std::string user = m_sessions[hdl];
            users[user].status = JSON_KEYS::OFFLINE;
            m_sessions.erase(hdl);
            json msg;
            msg[JSON_KEYS::TYPE] = JSON_KEYS::STATUS;
            msg[JSON_KEYS::USER] = user;
            msg[JSON_KEYS::STATUS] = JSON_KEYS::OFFLINE;
        }
    }

    void onLogin(const std::string& user, const std::string& pass, websocketpp::connection_hdl hdl) {
        // onlineUsers[user] = hdl;
        if(!users.count(user) || users[user].passWord != pass){
            m_server.send(hdl, R"({"type":"error","messge":"Invalid Login"})", websocketpp::frame::opcode::text);
            return;
        }

        m_sessions[hdl] = user;
        users[user].status = JSON_KEYS::ONLINE;

        json msg;
        msg[JSON_KEYS::TYPE] = JSON_KEYS::STATUS;
        msg[JSON_KEYS::USER] = user;
        msg[JSON_KEYS::STATUS] = JSON_KEYS::ONLINE;
        // send stored messages
        auto it = offlineMessages.find(user);
        if (it != offlineMessages.end()) {
            for (auto& msg : it->second) {
                m_server.send(hdl, msg.dump(), websocketpp::frame::opcode::text);
            }
            offlineMessages.erase(it); // clear after delivery
        }
    }
    void onMessage(websocketpp::connection_hdl hdl, m_masterServer::message_ptr msg){
        auto data = json::parse(msg->get_payload());
        
        std::string to = data["toUser"];
        std::string from = data["from"];

        auto it = onlineUsers.find(to);
        if(data[JSON_KEYS::TYPE] == "login"){
            std::string user = data[User_Keys::USERNAME];
            std::string pass = data[User_Keys::PASSWORD];
            onLogin(user, pass, hdl);

            return;
            // broadCast(msg);
        }

        if(data[JSON_KEYS::TYPE] == JSON_KEYS::MSG){
            if(!m_sessions.count(hdl)){
                std::cout << m_tag << "User not found..\n";
                return;
            }

            json msg;
            msg[JSON_KEYS::TYPE] = JSON_KEYS::MSG;
            msg[JSON_KEYS::FROM] = m_sessions[hdl];
            msg[JSON_KEYS::TEXT] = data[JSON_KEYS::TEXT];
            msg[JSON_KEYS::TO_USER] = data[JSON_KEYS::TO_USER];
            msg[JSON_KEYS::TIMESTAMP] = std::time(nullptr);

            std::cout << m_tag << "Received Msg for user : " << data[JSON_KEYS::TO_USER] << "\n";
            broadCast(msg);
        }
    }
};

int main(){
    CommModuleServer* m_ptr = new CommModuleServer();
    // std::cout << tag << "Starting Server:" << std::endl;
    // m_server.init_asio();
    // m_server.set_open_handler(&onOpen);
    // m_server.set_close_handler(&onClose);
    // m_server.set_message_handler(&onMessage);

    // m_server.listen(9003);
    // // m_masterServer.listen(9004);
    // m_server.start_accept();
    // m_server.run();
    // std::cout << tag << "Server Started" << std::endl;
}