//
// Created by super on 20.06.2024.
//

#ifndef ARCADEGAMESGLFW_IMGUICONTEXT_H
#define ARCADEGAMESGLFW_IMGUICONTEXT_H

#include "vector"
#include "Context.h"
//CPR
#include "cpr/cpr.h"

class ImguiContext : public Context{
    std::vector<BaseGame*> GameList {};
public:
    ImguiContext() = default;

    void init() override;

    ~ImguiContext() override;
    void loop() override;
    void size_callback(int width, int height) override {}
    void key_callback(int key, int scancode, int action, int mods) override {}
private:
    void GetUpdateInfo (const cpr::Response& r);
#ifdef _DEBUG
    bool ImguiDebugLog = false;
    bool ImguiDemo = false;
#endif
    std::string UpdateUrl;
    bool ImguiAbout = false;
    bool ImguiUGuide = false;
    bool SendError = false;
    std::string ErrorText;
};


#endif //ARCADEGAMESGLFW_IMGUICONTEXT_H
