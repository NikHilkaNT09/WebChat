#include <memory>
#include <unordered_map>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/frame.hpp>
#include <websocketpp/server.hpp>
#include <nlohmann/json.hpp>

#include <iostream>
#include <set>

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
    // std::cout << tag << ">>>>> " << msg["text"];
    for(auto &conn: sessions){
        if (msg.contains("text")) {
        std::cout << tag << " >>> " << msg["text"] << std::endl;
        }
        m_masterServer.send(conn.first, msg.dump(), websocketpp::frame::opcode::text);
    }
}

void onOpen(websocketpp::connection_hdl hdl){
    std::cout << tag << "Client Connected" ;
}

void onClose(websocketpp::connection_hdl hdl){
    std::cout << tag << "Connection Closed";

    if(sessions.count(hdl)){
        std::string user = sessions[hdl];
        users[user].status = "offline";
        sessions.erase(hdl);
        json msg;
        msg["type"] = "status";
        msg["user"] = user;
        msg["status"] = "offline";
        broadCast(msg);
    }
}

void onMessage(websocketpp::connection_hdl hdl, server_t::message_ptr msg){
    auto data = json::parse(msg->get_payload());

    if(data["type"] == "login"){
        std::string user = data["username"];
        std::string pass = data["password"];

        if(!users.count(user) || users[user].passWord != pass){
            m_masterServer.send(hdl, R"({"type":"error","messge":"Invalid Login"})", websocketpp::frame::opcode::text);
            return;
        }

        sessions[hdl] = user;
        users[user].status = "online";

        json msg;
        msg["type"] = "status";
        msg["user"] = user;
        msg["status"] = "online";
        broadCast(msg);
    }

    if(data["type"] == "message"){
        if(!sessions.count(hdl))return;

        json msg;
        msg["type"] = "message";
        msg["from"] = sessions[hdl];
        msg["text"] = data["text"];
        msg["timestamp"] = std::time(nullptr);
        broadCast(msg);
    }
}

void sendToReceiver(){
    
}

int main(){
    std::cout << tag << "Starting Server:" << std::endl;
    m_masterServer.init_asio();
    m_masterServer.set_open_handler(&onOpen);
    m_masterServer.set_close_handler(&onClose);
    m_masterServer.set_message_handler(&onMessage);

    m_masterServer.listen(9003);
    m_masterServer.start_accept();
    m_masterServer.run();
    std::cout << tag << "Server Started" << std::endl;
}
