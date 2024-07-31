//
// Created by super on 22.06.2024.
//

#ifndef ARCADEGAMESGLFW_SNAKEMAP_H
#define ARCADEGAMESGLFW_SNAKEMAP_H

#include <string>
#include <glm/detail/type_vec2.hpp>
#include "vector"
#include "Engine.h"

#define SNAKE_MAP_ERROR(b) \
        if (b) \
        { \
        SPDLOG_WARN(#b); boxer::show(#b, _c("Snake map Error"), boxer::Style::Warning); return nullptr; \
        }

class SnakeMap final {
public:
    SnakeMap();

    enum Tile{
        None,
        Wall
    };
    [[nodiscard]] const Coords<unsigned int> &getField() const{
        return field;
    }

    [[nodiscard]] const std::vector<Tile> &getMap() const {
        return Map;
    }

    [[nodiscard]] unsigned short getDefaultSnakeSize() const {
        return defaultSnakeSize;
    }

    [[nodiscard]] float getTickSpeed() const {
        return tickSpeed;
    }

    [[nodiscard]] const Coords<> &getDefaultSnakePos() const {
        return DefaultSnakePos;
    }

    [[nodiscard]] Directions getDefaultSnakeDirection() const {
        return DefaultSnakeDirection;
    }

    static SnakeMap* load(const std::string& name);

    static void save(const std::string& name, SnakeMap* map);

private:
    Coords<unsigned int> field {10};
    unsigned short defaultSnakeSize = 4;
    std::vector<Tile> Map {};
    float tickSpeed = .4;
    Coords<> DefaultSnakePos {1,1};
    Directions DefaultSnakeDirection = Down;
};


#endif //ARCADEGAMESGLFW_SNAKEMAP_H
