#include "login.h"

Login::Login(LoginCallback cb): wxFrame(nullptr, wxID_ANY, "Login",
              wxDefaultPosition, wxSize(300, 350)),
      m_onLogin(std::move(cb))
{
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    vbox->Add(new wxStaticText(panel, wxID_ANY, "Username"), 0, wxALL, 5);
    m_userCtrl = new wxTextCtrl(panel, wxID_ANY);
    vbox->Add(m_userCtrl, 0, wxEXPAND | wxALL, 5);

    vbox->Add(new wxStaticText(panel, wxID_ANY, "Password"), 0, wxALL, 5);
    m_passCtrl = new wxTextCtrl(panel, wxID_ANY, "",
                               wxDefaultPosition, wxDefaultSize,
                               wxTE_PASSWORD);
    vbox->Add(m_passCtrl, 0, wxEXPAND | wxALL, 5);

    vbox->Add(new wxStaticText(panel, wxID_ANY, "toUser"), 0, wxALL, 5);
    m_toUserCtrl = new wxTextCtrl(panel, wxID_ANY, "",
                               wxDefaultPosition, wxDefaultSize);
    vbox->Add(m_toUserCtrl, 0, wxEXPAND | wxALL, 5);

    m_loginBtn = new wxButton(panel, wxID_ANY, "Login");
    vbox->Add(m_loginBtn, 0, wxALIGN_CENTER | wxALL, 10);

    panel->SetSizer(vbox);

    m_loginBtn->Bind(wxEVT_BUTTON, &Login::OnLogin, this);
}

void Login::OnLogin(wxCommandEvent&) {
    std::cout << "___on login__" << std::endl;
    std::string user = m_userCtrl->GetValue().ToStdString();
    std::string pass = m_passCtrl->GetValue().ToStdString();
    std::string toUser = m_toUserCtrl->GetValue().ToStdString();

    if (user.empty() || pass.empty() || toUser.empty()) {
        wxMessageBox("Username, toUser and password cannot be empty",
                     "Login Error",
                     wxICON_ERROR);
        return;
    }

    if (m_onLogin) {
        m_onLogin(user, pass, toUser);
    }

    // Close(); // close login window
}