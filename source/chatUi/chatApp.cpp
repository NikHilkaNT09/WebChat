#include "chatUi.h"
#include "../../source/login/login.h"

#include <fstream>
#include <string>
#include <iostream>

struct UserCredentials {
    std::string username;
    std::string password;
};

#include <cstdlib>
#if 0
std::string expandUserPath(const std::string& path) {
    if (!path.empty() && path[0] == '~') {
#ifdef _WIN32
        const char* home = std::getenv("USERPROFILE");
#else
        const char* home = std::getenv("HOME");
#endif
        if (home)
            return std::string(home) + path.substr(1);
    }
    return path;
}


UserCredentials readUsernameFromFile(const std::string& path) {
    std::ifstream file;
    file.open(path, std::ios::in);
    // std::string username;

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open user file");
    }

    UserCredentials cred;
    std::getline(file, cred.username);
    std::getline(file,cred.password);

    if (cred.username.empty()) {
        throw std::runtime_error("Username file is empty");
    }

    return cred;
}
#endif
class ChatApp : public wxApp {
public:
Login* login = nullptr;

    virtual bool OnInit() {
        std::string sTag = "CA:\t";
        // UserCredentials cred = readUsernameFromFile(expandUserPath("~/Documents/user.txt"));
        try{
        login = new Login(
            [=](const std::string& user, const std::string& pass, const std::string& toUser) {
                std::cout << sTag << "__user: " << user << "\t __pass: " << pass << "\n";
                ChatFrame* frame = new ChatFrame(
                            user,
                            pass,
                            toUser,
                            [frame](const std::string& msg) {
                                frame->sendMessage(msg);
                            }
                            );
                frame->setToUser(toUser);

                frame->setLoginCallBack([=](const std::string res){
                    if(res == "true"){
                        frame->Show(true);
                        login->Show(false);
                    }
                });

                // if(frame->getLoginSuccess()){
                //     login->Hide();
                // }
                // frame->Show(true);
                // frame->executeInitiation();
            }
        );
        login->Show(true);
        }
        catch (const std::exception& e) {
        wxMessageBox(e.what(), "Startup Error", wxICON_ERROR);
        return false;
    } catch (...) {
        wxMessageBox("Unknown startup error", "Startup Error", wxICON_ERROR);
        return false;
    }
        
#if 0
        ChatFrame* frame = new ChatFrame(
                            cred.username,
                            cred.password,
                            [frame](const std::string& msg) {
                                frame->sendData(msg);
                            }
                            );
#endif

        // std::string user1 = "bob";
        // frame->setToUser(user1);
        // frame->Show(true);
        // frame->executeInitiation();
        
        
        return true;
    }
};


wxIMPLEMENT_APP(ChatApp);