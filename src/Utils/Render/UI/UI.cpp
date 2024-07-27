//
// Created by super on 23.07.2024.
//

#include <glm/ext/matrix_clip_space.hpp>
#include "UI.h"

glm::mat4 UI::Matrix(int width, int height) {
    if ((float) width > (float) height)
        return glm::ortho(-1.0 * ((float) width/(float) height), 1.0 * ((float) width/(float) height), -1.0, 1.0, -1.0, 1.0);
    else
        return glm::ortho(-1.0, 1.0, -1.0 * ((float) height/(float) width), 1.0 * ((float) height/(float) width), -1.0, 1.0);
}

void UI::DarkerBackground(MergedRender* m) {
    m->quard = new MergedRender::Quard({-2,-2},4,4);
    m->quard->color = {0.,0.,0.,.5};
    m->setSpeed(MergedRender::SpeedContent::STATIC);
}

glm::vec2 UI::vector_to_uicoords(double xpos, double ypos){
    if ((float) getWidth() > (float) getHeight())
        return {((float) getWidth()/(float) getHeight() * 2) /((float) getWidth() / xpos) - ((float) getWidth()/(float) getHeight()),ypos / getHeight() * -2 + 1};
    else
        return {xpos / getWidth() * 2 - 1,(((float) getHeight()/(float) getWidth() * 2) /((float) getHeight() / ypos) - ((float) getHeight()/(float) getWidth())) * -1};
}
