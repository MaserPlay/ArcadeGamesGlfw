//
// Created by super on 15.06.2024.
//

#ifndef ARCADEGAMES_OTHER_H
#define ARCADEGAMES_OTHER_H


#include <glm/detail/type_vec2.hpp>

enum Directions{
    Up,
    Left,
    Down,
    Right
};
#define DIRECTION_LAST 5
typedef glm::vec<2, unsigned short> Coords;

#endif //ARCADEGAMES_OTHER_H
