#pragma once
#include "commModule.h"

CommModule::CommModule(std::string user, std::string pass) {
  m_tag = "Client[" + user + "]:\t";
  this->user = user;
  this->pass = pass;
  this->uri = "ws://10.150.11.50:9003";
}

void CommModule::executeInitiation() {
  init();
  connect();
  establishComm();
}

void CommModule::setToUser(std::string &user){
  m_destUser = user;
}

void CommModule::sendData(std::string data) { 
  inputData(data); 
}

void CommModule::setMessageCallback(MessageCallBack cb){
  m_onMessage = std::move(cb);
}

void CommModule::init() {
  m_client.init_asio();

  m_client.set_message_handler([this](auto, auto msg) {
      if (msg->get_opcode() != websocketpp::frame::opcode::text)
          return;

      try {
          const std::string& payload = msg->get_payload();
          if (m_onMessage)
              m_onMessage(payload);
      } catch (...) {
          std::cout << "__message handling failed\n";
      }
  });
  m_client.set_open_handler([this](websocketpp::connection_hdl conn) {
    hdl = conn;
    std::cout << m_tag << "__connected: Sending Login..\n";

    json login = {{"type", "login"}, {"username", user}, {"password", pass},{"from", user}, {"toUser", m_destUser}};
    m_client.send(hdl, login.dump(), websocketpp::frame::opcode::text);

  });

  std::cout << m_tag << "__init Done..\n";
}

void CommModule::connect() {
  websocketpp::lib::error_code errCode;
  auto connection = m_client.get_connection(uri, errCode);

  if (errCode) {
    std::cout << m_tag << "__connection Failed: " << errCode.message()
              << std::endl;
    return;
  }

  m_client.connect(connection);
  std::cout << m_tag << "__connecting...\n";
}

void CommModule::establishComm() {
  std::cout << m_tag << "__establishing Comms..\n";
  m_thread = std::thread([this]() { m_client.run(); });
  m_thread.detach();
}

void CommModule::inputData(std::string input) {
  json msg = {{"type", "message"}, {"from", user}, {"text", input}, {"toUser", m_destUser}};

  m_client.send(hdl, msg.dump(), websocketpp::frame::opcode::text);
}
