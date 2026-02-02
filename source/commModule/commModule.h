#pragma once

#include <iostream>
#include <string.h>

#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/common/system_error.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <nlohmann/json.hpp>

#include <iostream>
#include <thread>
#include <websocketpp/frame.hpp>


using json = nlohmann::json;
class CommModule{
public:
    using MasterClient = websocketpp::client<websocketpp::config::asio_client>;
    using MessageCallBack = std::function<void(const std::string&)>;
    explicit CommModule(std::string user, std::string pass);

    void executeInitiation();
    void setToUser(std::string &user);

    void sendData(std::string data);
    void setMessageCallback(MessageCallBack cb);
private:
    MasterClient m_client;
    MessageCallBack m_onMessage;
    websocketpp::connection_hdl hdl;
    std::string m_tag;
    std::thread m_thread;

    std::string user;
    std::string pass;
    std::string uri;
    std::string m_destUser;

    void init();

    void connect();

    void establishComm();

    void inputData(std::string input);
};