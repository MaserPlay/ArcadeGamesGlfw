//
// Created by super on 22.07.2024.
//

#ifndef ARCADEGAMES_BUTTON_H
#define ARCADEGAMES_BUTTON_H


#include "MergedRender.h"

struct Button {
    Button(const std::function<void(void)> &func, const std::string &name) : func(func), name(name) {}
    std::unique_ptr<MergedRender> render {new MergedRender()};
    std::function<void(void)> func {};
    std::string name {};
    std::vector<std::unique_ptr<MergedRender>> nameRender {};
};
namespace ButtonVBox{
    void Init(std::vector<Button>& v);
    void Show(std::vector<Button>& v, glm::mat4 mat);
    void Click(std::vector<Button>& v);
}


#endif //ARCADEGAMES_BUTTON_H
