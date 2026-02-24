
#pragma once
#include <wx/wx.h>
#include <functional>
#include <string>

class Login : public wxFrame {
public:
    using LoginCallback = std::function<void(const std::string&, const std::string&, const std::string&)>;

    explicit Login(LoginCallback cb);

private:
    wxTextCtrl* m_userCtrl;
    wxTextCtrl* m_passCtrl;
    wxTextCtrl* m_toUserCtrl;
    wxButton*   m_loginBtn;

    LoginCallback m_onLogin;

    void OnLogin(wxCommandEvent& event);
};
