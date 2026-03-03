// // #include <thread>
// // #include <wx/wx.h>
// #include "chatFrame.h"
// #include <cstddef>
// #include <iterator>
// #include <memory>
// // #include <wx/wx.h>

// #include "../../source/commModule/commModule.h"
// // #include <wx/button.h>
// ChatFrame::ChatFrame(const wxString& user, std::string pass, std::string toUser)
//     : wxFrame(nullptr, wxID_ANY,
//                 "Chat Client - " + user,
//                 wxDefaultPosition,
//                 wxSize(800,800)),
//                 m_comm(std::make_unique<CommModule>(static_cast<std::string>(user), pass))
// {
//     std::cout << m_tag << "__entered chat frame__" << std::endl;
//     std::cout << m_tag << "__user :" << user << "__pass : " << pass << std::endl;
//     wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);


//     // 🔙 Back button (top bar)
//     backButton = new wxButton(this, wxID_ANY, "Back");
//     chatLog = new wxTextCtrl(this, wxID_ANY, "",
//                                 wxDefaultPosition, wxDefaultSize,
//                                 wxTE_MULTILINE | wxTE_READONLY);

//     input = new wxTextCtrl(this, wxID_ANY, "",
//                             wxDefaultPosition, wxDefaultSize,
//                             wxTE_PROCESS_ENTER);

//     sizer->Add(chatLog, 1, wxEXPAND | wxALL, 5);
//     sizer->Add(input, 0, wxEXPAND | wxALL, 5);
//     sizer->Add(backButton, 0, wxALL | wxALIGN_LEFT, 5);

//     SetSizer(sizer);



//     input->Bind(wxEVT_TEXT_ENTER, &ChatFrame::OnSend, this);
//     backButton->Bind(wxEVT_BUTTON, &ChatFrame::OnBack, this);
//     // if(m_clientModule == nullptr)
//         // m_clientModule = std::make_shared<CommModule>(static_cast<std::string>(user), pass);
//     // m_clientModule->executeInitiation();
//     // m_clientModule->setMessageCallback([this](const std::string& data) {

//  /*
//     m_clientModule->setMessageCallback([this](const std::string& data) {
//         json j;
//         try {
//             j = json::parse(data);
//         } catch (...) {
//             return;
//         }

//         // if (j.contains("messge") && j["messge"] == "Invalid Login") {
//         //     m_clientModule->resolveLogin(false);
//         //     return;
//         // }

//         // if (j.contains("login") && j["login"] == "Login Success") {
//         //     m_clientModule->resolveLogin(true);
//         //     return;
//         // }

//         // Normal chat message
//         if (j.contains("text")) {
//             std::string from = j["from"];
//             std::string text = j["text"];

//             CallAfter([this, from, text]() {
//                 AppendMessage(from + ": " + text);
//             });
//         }
//     });
//     */
//     m_comm->setToUser(toUser);

//     // m_clientModule->setMessageCallback([this](const std::string& data) {
//     //     std::cout << m_tag << "__received data__" << data << std::endl;
//     //     this->CallAfter([this, data]() {
//     //         json j = json::parse(data);

//     //         if(j.contains("messge") && j["messge"] == "Invalid Login"){
//     //             std::cout << m_tag << " Invalid Login..." << std::endl;
//     //             destroyClient();
//     //         }
//     //         else if(j.contains("login") && j["login"] == "Login Success"){
//     //             std::cout << m_tag << "__login succes__" <<std::endl;
//     //             m_bIsLoginSuccess = true;
//     //         }
//     //         else{
//     //             std::string res = j["text"];
//     //             std::string fromUser = j["from"];
//     //             AppendMessage(fromUser + ":" + res);
//     //         }
//     //     });
//     // });
// }

// void ChatFrame::waitForLoginResult() {
//     bool success = m_loginFuture.get();  // blocks safely

//     if (success) {
//         CallAfter([this]() {
//             Show();
//             Raise();
//         });
//     } else {
//         CallAfter([this]() {
//             wxMessageBox("Invalid username or password",
//                          "Login Failed",
//                          wxICON_ERROR);
//             Destroy();
//         });
//     }
// }

// void ChatFrame::AppendMessage(const wxString& msg) {
//     chatLog->AppendText(msg + "\n");
// }
// void ChatFrame::destroyClient(){
//     m_bIsLoginSuccess = false;
//     // delete m_clientModule;
//     // m_clientModule = nullptr;
// }
// void ChatFrame::OnSend(wxCommandEvent&) {
//     wxString msg = input->GetValue();
//     input->Clear();
//     m_comm->sendData(static_cast<std::string>(msg));
//     AppendMessage("You: " + msg);
// }

// void ChatFrame::OnBack(wxCommandEvent& ev){
//     this->Destroy();
// }

// void ChatFrame::sendMessage(std::string msg){
//     std::cout << m_tag << "__sending message from frame: " << msg << std::endl;
//     m_comm->sendData(msg);
// }

// void ChatFrame::setToUser(std::string user){
//     m_comm->setToUser(user);
// }

// void ChatFrame::setLoginCallback(std::function<void(bool)> cb) {
//     m_comm->setLoginStatus(std::move(cb));  // ➕ ADDED
// }
// void ChatFrame::executeInitiation(){
//     m_comm->executeInitiation();
// }

// bool ChatFrame::bIsLoginSuccess(){
//     return m_bIsLoginSuccess;
// }
 