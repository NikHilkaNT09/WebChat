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

std::string tag = "Server:\t";
using server_t = websocketpp::server<websocketpp::config::asio>;
using json = nlohmann::json;

struct User{
std::string passWord;
std::string status;
};

server_t m_masterServer;
std::map<websocketpp::connection_hdl, std::string, 
                    std::owner_less<websocketpp::connection_hdl>> sessions;

std::unordered_map<std::string, User> users= {
    {"alice", {"1234", "offline"}},
    {"bob",   {"1234", "offline"}}
};

void broadCast(const json &msg){
    for(auto &[conn, user]: sessions){
        if (msg.contains(JSON_KEYS::TEXT)) {
            std::cout << tag << " >>> " << msg[JSON_KEYS::TEXT] << std::endl;
        }
        if(msg.contains(JSON_KEYS::TO_USER) ){

            std::cout << tag << " +++++++ " << user;
        if( user == msg[JSON_KEYS::TO_USER])
            m_masterServer.send(conn, msg.dump(), websocketpp::frame::opcode::text);
        }
    }
}

void onOpen(websocketpp::connection_hdl hdl){
    std::cout << tag << "Client Connected" ;
}

void onClose(websocketpp::connection_hdl hdl){
    std::cout << tag << "Connection Closed";

    if(sessions.count(hdl)){
        std::string user = sessions[hdl];
        users[user].status = JSON_KEYS::OFFLINE;
        sessions.erase(hdl);
        json msg;
        msg[JSON_KEYS::TYPE] = JSON_KEYS::STATUS;
        msg[JSON_KEYS::USER] = user;
        msg[JSON_KEYS::STATUS] = JSON_KEYS::OFFLINE;
        broadCast(msg);
    }
}
void sendToReceiver(websocketpp::connection_hdl hdl, server_t::message_ptr msg){
    std::cout << tag << "Sending data to Client back:";
    if(sessions.count(hdl)){
        std::string user = sessions[hdl];
        // if(users[user] == ""){

        // }
    }
    m_masterServer.send(hdl, msg);
}

void onMessage(websocketpp::connection_hdl hdl, server_t::message_ptr msg){
    auto data = json::parse(msg->get_payload());
    sendToReceiver(hdl, msg);

    if(data[JSON_KEYS::TYPE] == "login"){
        std::string user = data[User_Keys::USERNAME];
        std::string pass = data[User_Keys::PASSWORD];

        if(!users.count(user) || users[user].passWord != pass){
            m_masterServer.send(hdl, R"({"type":"error","messge":"Invalid Login"})", websocketpp::frame::opcode::text);
            return;
        }

        sessions[hdl] = user;
        users[user].status = JSON_KEYS::ONLINE;

        json msg;
        msg[JSON_KEYS::TYPE] = JSON_KEYS::STATUS;
        msg[JSON_KEYS::USER] = user;
        msg[JSON_KEYS::STATUS] = JSON_KEYS::ONLINE;
        broadCast(msg);
    }

    if(data[JSON_KEYS::TYPE] == JSON_KEYS::MSG){
        if(!sessions.count(hdl))return;

        json msg;
        msg[JSON_KEYS::TYPE] = JSON_KEYS::MSG;
        msg[JSON_KEYS::FROM] = sessions[hdl];
        msg[JSON_KEYS::TEXT] = data[JSON_KEYS::TEXT];
        msg[JSON_KEYS::TO_USER] = data[JSON_KEYS::TO_USER];
        msg[JSON_KEYS::TIMESTAMP] = std::time(nullptr);
        broadCast(msg);
    }
}


int main(){
    std::cout << tag << "Starting Server:" << std::endl;
    m_masterServer.init_asio();
    m_masterServer.set_open_handler(&onOpen);
    m_masterServer.set_close_handler(&onClose);
    m_masterServer.set_message_handler(&onMessage);

    m_masterServer.listen(9003);
    // m_masterServer.listen(9004);
    m_masterServer.start_accept();
    m_masterServer.run();
    std::cout << tag << "Server Started" << std::endl;
}
