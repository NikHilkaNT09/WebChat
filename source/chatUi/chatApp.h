#pragma once
#include <wx/wx.h>

class ChatFrame;

class ChatApp : public wxApp {
public:
    bool OnInit() override;

private:
    ChatFrame* m_chatFrame = nullptr;
};
