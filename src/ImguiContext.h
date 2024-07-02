//
// Created by super on 20.06.2024.
//

#ifndef ARCADEGAMESGLFW_IMGUICONTEXT_H
#define ARCADEGAMESGLFW_IMGUICONTEXT_H

#include "vector"
#include "Context.h"

class ImguiContext : public Context{
    std::vector<BaseGame*> GameList {};
public:
    ImguiContext() = default;

    void init() override;

    ~ImguiContext() override;
    void loop() override;
#ifdef _DEBUG
    bool ImguiDebugLog = false;
#endif
    bool ImguiAbout = false;
    bool ImguiUGuide = false;
    void size_callback(int width, int height) override {}
    void key_callback(int key, int scancode, int action, int mods) override {}
};


#endif //ARCADEGAMESGLFW_IMGUICONTEXT_H
