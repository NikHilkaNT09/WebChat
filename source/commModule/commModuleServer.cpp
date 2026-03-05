#pragma once

#include "commModuleServer.h"

using json = nlohmann::json;

CommModuleServer::CommModuleServer(){
    establishServer();
};

void CommModuleServer::establishServer(){
    init(),
    setHandlers();
    startServer();
}



void CommModuleServer::init(){
    std::cout << m_tag << "Initialising server..\n";
    m_server.init_asio();
}

void CommModuleServer::startServer(){
    websocketpp::lib::error_code errCode;
    m_server.listen(9003);
    m_server.start_accept();
    m_server.run();
}
void CommModuleServer::setHandlers(){
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

void CommModuleServer::broadCast(const json &msg){
    for(auto &[conn, user]: m_sessions){
        if (msg.contains(JSON_KEYS::TEXT)) {
            std::cout << m_tag << " >>> " << msg[JSON_KEYS::TEXT] << "\n";
        }
        if(msg.contains(JSON_KEYS::TO_USER) ){
            if( user == msg[JSON_KEYS::TO_USER] && users[user].status == "online")
                m_server.send(conn, msg.dump(), websocketpp::frame::opcode::text);
            else{
                offlineMessages[msg[JSON_KEYS::TO_USER]].push_back(msg);
                std::cout << m_tag << "__pushing data in vector__::" << msg << "\n";
            }
        }
    }
}

void CommModuleServer::onOpen(websocketpp::connection_hdl hdl){
    std::cout << m_tag << "Client Connected" ;
}

void CommModuleServer::onClose(websocketpp::connection_hdl hdl){
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

void CommModuleServer::onLogin(const std::string& user, const std::string& pass, websocketpp::connection_hdl hdl) {

    if(!users.count(user) || users[user].passWord != pass){
        m_server.send(hdl, R"({"type":"error","messge":"Invalid Login"})", websocketpp::frame::opcode::text);
        return;
    }
    m_server.send(hdl, R"({"type":"error","messge":"Login Success"})", websocketpp::frame::opcode::text);

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
void CommModuleServer::onMessage(websocketpp::connection_hdl hdl, m_masterServer::message_ptr msg){
    auto data = json::parse(msg->get_payload());
    
    std::string to = data["toUser"];
    std::string from = data["from"];

    auto it = onlineUsers.find(to);
    if(data[JSON_KEYS::TYPE] == "login"){
        std::string user = data[User_Keys::USERNAME];
        std::string pass = data[User_Keys::PASSWORD];
        onLogin(user, pass, hdl);

        return;
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
