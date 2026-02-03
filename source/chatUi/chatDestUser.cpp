#include "chatUi.h"

class ChatApp : public wxApp {
public:
    virtual bool OnInit() {
                ChatFrame* frame = new ChatFrame(
                            "bob",
                            "1234",
                            [frame](const std::string& msg) {
                                frame->sendMessage(msg);
                            }
    );
        std::string user1 = "nikhil";
        frame->setToUser(user1);
        frame->Show(true);
        frame->executeInitiation();
        
        return true;
    }
};


wxIMPLEMENT_APP(ChatApp);