#include <thread>
#include <wx/wx.h>
#include "../../source/commModule/commModule.h"
class ChatFrame : public wxFrame, public CommModule {
public:
    wxTextCtrl* chatLog;
    wxTextCtrl* input;
    using SendCallback = std::function<void(const std::string&)>;   

    explicit ChatFrame(const wxString& user, std::string pass, SendCallback cb);

    void AppendMessage(const wxString& msg);
    void OnSend(wxCommandEvent& ev);

    private:
     SendCallback sendCallback;
};

