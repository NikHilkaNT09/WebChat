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

    explicit CommModuleServer();

    void establishServer();
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
    {"arush",   {"arush@arush", "offline"}},
    {"bani", {"bani@bani", "offline"}},
    {"amrit",   {"amrit@amrit", "offline"}},
    {"lalit",   {"lalit@lalit", "offline"}},
    {"barjesh",   {"barjesh@barjesh", "offline"}},
    {"nasar",   {"nasar@nasar", "offline"}},
    {"tushar",   {"tushar@tushar", "offline"}},
    {"navin",   {"navin@navin", "offline"}}
    };


    void init();

    void startServer();
    void setHandlers();

    void broadCast(const json &msg);

    void onOpen(websocketpp::connection_hdl hdl);

    void onClose(websocketpp::connection_hdl hdl);

    void onLogin(const std::string& user, const std::string& pass, websocketpp::connection_hdl hdl);
    void onMessage(websocketpp::connection_hdl hdl, m_masterServer::message_ptr msg);
    
};

int main(){
    CommModuleServer * mode = new CommModuleServer();
}