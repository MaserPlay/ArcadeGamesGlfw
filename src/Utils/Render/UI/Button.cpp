//
// Created by super on 22.07.2024.
//

#include "Button.h"
#include "UI.h"


void ButtonVBox::Init(std::vector<Button>& v) {
    unsigned short max = std::size(v);
    for (unsigned short i = 0; i < max; ++i) {
        v[i].render->quard = new MergedRender::Quard({-.5,(2.f / (max + 1.f)) * (i + 1.f) - 1.f},1,.2);
        v[i].render->quard->color = {1.,1.,0.,1.};
        v[i].render->setSpeed(MergedRender::SpeedContent::STATIC);
        v[i].render->load();
    }
}

void ButtonVBox::Show(std::vector<Button>& v, glm::mat4 mat) {
    for (int i = 0; i < v.size(); ++i) {
        v[i].render->use(mat);
        auto size = .003f;
        Font::RenderText(v[i].name, {Font::TextWidth(v[i].name) * -size / 2,(2.f / (v.size() + 1.f)) * (i + 1.f) - .95f}, mat, v[i].nameRender, size, {0,0,0,1});
    }
}

void ButtonVBox::Click(std::vector<Button>& v) {
    double xpos, ypos;
    glfwGetCursorPos(getwindow(), &xpos, &ypos);
    auto vec = UI::vector_to_uicoords(xpos, ypos);
    for (int i = 0; i < v.size(); ++i) {
        if (vec.x >= -.5 && vec.x <= .5 && vec.y >= (2.f / (v.size() + 1.f)) * (i + 1.f) - 1.f && vec.y <= (2.f / (v.size() + 1.f)) * (i + 1.f) - 1.f + .2f)
        {
            v[i].func();
            return;
        }
    }
}
