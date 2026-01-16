#include <ostream>
#include <websocketpp/common/system_error.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <nlohmann/json.hpp>

#include <iostream>
#include <thread>

std::string tag = "Client:\t";
using client_t = websocketpp::client<websocketpp::config::asio_client>;
using json = nlohmann::json;

int main(){
    client_t m_client;

    m_client.init_asio();

    m_client.set_message_handler([](auto ,auto msg){
        std::cout << tag << msg->get_payload() << std::endl;
    });

    m_client.set_open_handler(    [&](websocketpp::connection_hdl hdl) {

        std::cout << "Connection opened, sending login...\n";

        m_client.send(
            hdl,
            json{
                {"type", "login"},
                {"username", "alice"},
                {"password", "1234"}
            }.dump(),
            websocketpp::frame::opcode::text
        );
    });

    websocketpp::lib::error_code errCode;
    auto conn = m_client.get_connection("ws://localhost:9003", errCode);

    m_client.connect(conn);
    std::cout << tag << " Connecting Client:" << std::endl;
    std::thread mThread([&m_client](){
        m_client.run();
    });

    std::cout << tag << " Connecting Client: Point 1" << std::endl;
    // m_client.send(conn->get_handle(),
    // json{{"type","login"},{"username","alice"},{"password","1234"}}.dump(),
    // websocketpp::frame::opcode::text);

    std::cout << tag << " Connecting Client: Point 2" << std::endl;
    while (true) {
    std::string text;
    std::getline(std::cin, text);

    m_client.send(conn->get_handle(),
        json{{"type","message"},{"text",text}}.dump(),
        websocketpp::frame::opcode::text);
    }

    mThread.join();
} 

