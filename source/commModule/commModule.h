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
struct User{
    std::string name;
    std::string pass;
};
class CommModule{
public:
    static CommModule *getInstance();
    CommModule(const CommModule&) = delete;
    CommModule& operator=(const CommModule&) = delete;
    bool setUserIdPass(User cred);
    void setLoginStatus(const bool login);
    bool getLoginStatus();

    using MasterClient = websocketpp::client<websocketpp::config::asio_client>;
    using MessageCallBack = std::function<void(const std::string&)>;
    // explicit CommModule(std::string user, std::string pass);
    void sendLogin();
    void executeInitiation();
    void setToUser(std::string &user);

    void sendData(std::string data);
    void setMessageCallback(MessageCallBack cb);
private:
    static CommModule* m_instance;
    explicit CommModule();
    ~CommModule();

    bool m_bIsLoggedIn;

    MasterClient m_client;
    MessageCallBack m_onMessage;
    websocketpp::connection_hdl hdl;

    std::string m_tag = "CM: \t";
    std::thread m_thread;

    std::string user;
    std::string pass;
    std::string uri;
    std::string m_destUser;

    void init();

    void connect();
    bool isConnected();
    void establishComm();

    void inputData(std::string input);
};