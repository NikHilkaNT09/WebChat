// #include <thread>
// #include <wx/wx.h>
#include "chatUi.h"
#include <cstddef>
#include <iterator>
#include <wx/wx.h>

ChatFrame::ChatFrame(const wxString& user, std::string pass, std::string toUser, SendCallback cb)
    : wxFrame(nullptr, wxID_ANY,
                "Chat Client - " + user,
                wxDefaultPosition,
                wxSize(800,800))
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

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
    if(m_clientModule == nullptr)
        m_clientModule = new CommModule(static_cast<std::string>(user), pass);

    m_clientModule->setMessageCallback([this](const std::string& data) {
        this->CallAfter([this, data]() {
            json j = json::parse(data);
            std::string res = j["text"];
            std::string fromUser = j["from"];
            
            if(j.contains("messge") && j["messge"] == "Invalid Login"){
                std::cout << " Invalid Login..." << std::endl;
                destroyClient();
            }
            else{
                AppendMessage(fromUser + ":" + res);
            }
            
        });
    });
}

void ChatFrame::AppendMessage(const wxString& msg) {
    chatLog->AppendText(msg + "\n");
}
void ChatFrame::destroyClient(){
    delete m_clientModule;
    m_clientModule = nullptr;
}
void ChatFrame::OnSend(wxCommandEvent&) {
    wxString msg = input->GetValue();
    input->Clear();
    m_clientModule->sendData(static_cast<std::string>(msg));
    AppendMessage("You: " + msg);
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
 