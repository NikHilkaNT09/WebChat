// #include <thread>
// #include <wx/wx.h>
#include "chatUi.h"
#include <iterator>
#include <wx/wx.h>

ChatFrame::ChatFrame(const wxString& user, std::string pass, SendCallback cb)
    : wxFrame(nullptr, wxID_ANY,
                "Chat Client - " + user,
                wxDefaultPosition,
                wxSize(800,800)), CommModule(static_cast<std::string>(user), pass),sendCallback(std::move(cb))
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
    setMessageCallback([this](const std::string& data) {
        this->CallAfter([this, data]() {
            json j = json::parse(data);
            std::cout << "__dxgfds$$__\t" << j << std::endl;
            std::string res = j["text"];
            std::string fromUser = j["from"];

            AppendMessage(fromUser + ":" + res);
        });
    });
}

void ChatFrame::AppendMessage(const wxString& msg) {
    chatLog->AppendText(msg + "\n");
}

void ChatFrame::OnSend(wxCommandEvent&) {
    wxString msg = input->GetValue();
    input->Clear();
    sendData(static_cast<std::string>(msg));
    AppendMessage("You: " + msg);
        // if (sendCallback)
        // sendCallback(msg.ToStdString());
}



 