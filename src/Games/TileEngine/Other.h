//
// Created by super on 15.06.2024.
//

#ifndef ARCADEGAMES_OTHER_H
#define ARCADEGAMES_OTHER_H


#include <glm/vec4.hpp>
#include <glm/vec2.hpp>

enum Directions{
    Up,
    Left,
    Down,
    Right
};
#define DIRECTION_LAST 5
typedef glm::vec<2, unsigned short> Coords;
typedef glm::vec2 ExtendedCoords;
typedef glm::vec4 Color;

#endif //ARCADEGAMES_OTHER_H
