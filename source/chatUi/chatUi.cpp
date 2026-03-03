// #include <thread>
// #include <wx/wx.h>
#include "chatUi.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <wx/wx.h>

ChatFrame::ChatFrame(const wxString& user, std::string pass, std::string toUser, SendCallback cb)
    : wxFrame(nullptr, wxID_ANY,
                "Chat Client - " + user,
                wxDefaultPosition,
                wxSize(800,800))
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    m_clientModule = CommModule::getInstance();

    chatLog = new wxTextCtrl(this, wxID_ANY, "",
                                wxDefaultPosition, wxDefaultSize,
                                wxTE_MULTILINE | wxTE_READONLY);

    input = new wxTextCtrl(this, wxID_ANY, "",
                            wxDefaultPosition, wxDefaultSize,
                            wxTE_PROCESS_ENTER);

    sizer->Add(chatLog, 1, wxEXPAND | wxALL, 5);
    sizer->Add(input, 0, wxEXPAND | wxALL, 5);

    SetSizer(sizer);



    input->Bind(wxEVT_TEXT_ENTER, &ChatFrame::OnSend, this);
#if 0
    if(m_clientModule == nullptr)
        m_clientModule = new CommModule(static_cast<std::string>(user), pass);
#endif
    // m_clientModule->executeInitiation();
    User u;
    u.name = user;
    u.pass = pass;
    m_clientModule->setUserIdPass(u);
    m_clientModule->sendLogin();
    m_clientModule->setMessageCallback([=](const std::string& data) {
        std::cout << m_tag << "__respose from server Received" << "\n";
        this->CallAfter([this, data]() {
            json j = json::parse(data);
 
            if(j.contains("messge") && j["messge"] == "Invalid Login"){
                std::cout << m_tag << " Invalid Login..." << "\n";
                // destroyClient();
            }
            else{
                std::cout << m_tag << "__login was succes" << "\n";
                loginCallBack("true");
                if(j.contains("text") && j.contains("from")){
                std::string res = j["text"];
                std::string fromUser = j["from"];
                
                AppendMessage(fromUser + ":" + res);
            }
                
            }
            
        });
    });
    // this->Hide();
}

void ChatFrame::AppendMessage(const wxString& msg) {
    chatLog->AppendText(msg + "\n");
}
void ChatFrame::destroyClient(){
    // delete m_clientModule;
    m_clientModule = nullptr;
}
void ChatFrame::OnSend(wxCommandEvent&) {
    wxString msg = input->GetValue();
    input->Clear();
    m_clientModule->sendData(static_cast<std::string>(msg));
    AppendMessage("You: " + msg);
}

void ChatFrame::setLoginCallBack(SendLoginCallback login){
    loginCallBack = std::move(login);
}

void ChatFrame::sendMessage(std::string msg){
    m_clientModule->sendData(msg);
}

void ChatFrame::setToUser(std::string user){
    m_clientModule->setToUser(user);
}

void ChatFrame::executeInitiation(){
    m_clientModule->executeInitiation();
}

bool ChatFrame::getLoginSuccess(){
    return m_clientModule->getLoginStatus();
}
 