#include <thread>
#include <wx/wx.h>
#include "../../source/commModule/commModule.h"
class ChatFrame : public wxFrame{
public:
    wxTextCtrl* chatLog;
    wxTextCtrl* input;
    using SendCallback = std::function<void(const std::string&)>;  
    using SendLoginCallback = std::function<void(const std::string&)>; 

    explicit ChatFrame(const wxString& user, std::string pass, std::string toUser, SendCallback cb);

    void AppendMessage(const wxString& msg);
    void OnSend(wxCommandEvent& ev);
    void destroyClient();
    void sendMessage(std::string msg);
    void setToUser(std::string msg);
    void executeInitiation();
    bool getLoginSuccess();
    void setLoginCallBack(SendLoginCallback login);

private:
    SendCallback sendCallback;
    SendLoginCallback loginCallBack;
    class CommModule *m_clientModule = nullptr;
    std::string m_tag = "CUI: \t";
};

