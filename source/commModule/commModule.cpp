#include "commModule.h"
#include "websocketpp/common/connection_hdl.hpp"
CommModule *CommModule::m_instance = nullptr;
CommModule *CommModule::getInstance(){
  if(!m_instance){
    m_instance = new CommModule();
  }
  return m_instance;
}

CommModule::CommModule() {
  // this->user = user;
  // this->pass = pass;
  this->uri = "ws://10.150.11.50:9003";
  executeInitiation();
}
CommModule::~CommModule(){
  if(m_instance != nullptr)
    delete m_instance;
  m_instance = nullptr;
}

bool CommModule::setUserIdPass(User cred){
  m_tag = "Client[" + cred.name + "]:\t";

  std::cout << m_tag << "__setting user id and pass" << "\n";
  this->user = cred.name;
  this->pass = cred.pass;
  
  return true;
}

void CommModule::setLoginStatus(const bool login){
  m_bIsLoggedIn = login;
}

bool CommModule::getLoginStatus(){
  return m_bIsLoggedIn;
}

void CommModule::sendLogin(){
  std::cout << m_tag << "__sending login" << "\n";
  if(!isConnected()){
    return;
  }
  json login = {{"type", "login"}, {"username", user}, {"password", pass},{"from", user}, {"toUser", m_destUser}};
  m_client.send(hdl, login.dump(), websocketpp::frame::opcode::text);
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
      // if (msg->get_opcode() != websocketpp::frame::opcode::text)
      //     return;

      try {
          const std::string& payload = msg->get_payload();
          std::cout << m_tag << "__payload received: " << payload << "\n";

          if (m_onMessage)
              m_onMessage(payload);
      } catch (...) {
          std::cout << "__message handling failed\n";
      }
  });
  m_client.set_open_handler([this](websocketpp::connection_hdl conn) {
    hdl = conn;
    std::cout << m_tag << "__connected: Sending Login..\n";

    // json login = {{"type", "login"}, {"username", user}, {"password", pass},{"from", user}, {"toUser", m_destUser}};
    // m_client.send(hdl, login.dump(), websocketpp::frame::opcode::text);

  });

  m_client.set_close_handler([this](websocketpp::connection_hdl conn) {
    std::cout << m_tag << "__connection closed\n";

    // If you stored the handle, you may clear/reset it
    hdl.reset();

    // Optional: get close code/reason
    auto con = m_client.get_con_from_hdl(conn);
    std::cout << "Code: " << con->get_remote_close_code()
              << " Reason: " << con->get_remote_close_reason()
              << "\n";
  });

  std::cout << m_tag << "__init Done..\n";
}

bool CommModule::isConnected()
{
    if (hdl.expired())
        return false;

    auto con = m_client.get_con_from_hdl(hdl);

    return con->get_state() == websocketpp::session::state::open;
}
void CommModule::connect() {
  websocketpp::lib::error_code errCode;
  auto connection = m_client.get_connection(uri, errCode);

  if (errCode) {
    std::cout << m_tag << "__connection Failed: " << errCode.message()
              << "\n";
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
