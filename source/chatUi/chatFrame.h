#pragma once
#include "../../source/libs/websocketpp_config.h"
#include <memory>
#include <thread>
// #include <wx/gtk/button.h>
#include <wx/wx.h>
#include "../../source/commModule/commModule.h"

class ChatFrame : public wxFrame{
public:
    wxTextCtrl* chatLog;
    wxTextCtrl* input;
    wxButton* backButton;
    using SendCallback = std::function<void(const std::string&)>;
    using SendCallbackLog = std::function<void(const bool&)>;   

    explicit ChatFrame(const wxString& user, std::string pass, std::string toUser);

    void AppendMessage(const wxString& msg);
    void OnSend(wxCommandEvent& ev);
    void OnBack(wxCommandEvent& ev);
    void destroyClient();
    void sendMessage(std::string msg);
    void setToUser(std::string msg);
    void executeInitiation();
    bool bIsLoginSuccess();
    void setLoginCallback(std::function<void(bool)> cb);
    void waitForLoginResult();
private:
    SendCallback sendCallback;
    SendCallbackLog sendLogStatus;
    std::unique_ptr<CommModule> m_comm; 
    bool m_bIsLoginSuccess = false;
    std::promise<bool> m_loginPromise;
    std::future<bool>  m_loginFuture;
    std::atomic<bool>  m_loginResolved{false};

};

