#include <thread>
#include <wx/wx.h>
#include "../../source/commModule/commModule.h"
class ChatFrame : public wxFrame{
public:
    wxTextCtrl* chatLog;
    wxTextCtrl* input;
    using SendCallback = std::function<void(const std::string&)>;   

    explicit ChatFrame(const wxString& user, std::string pass, std::string toUser, SendCallback cb);

    void AppendMessage(const wxString& msg);
    void OnSend(wxCommandEvent& ev);
    void destroyClient();
    void sendMessage(std::string msg);
    void setToUser(std::string msg);
    void executeInitiation();

private:
    SendCallback sendCallback;
    CommModule *m_clientModule = nullptr;
};

