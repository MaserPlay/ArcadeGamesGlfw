//
// Created by super on 22.06.2024.
//

#ifndef ARCADEGAMESGLFW_SNAKEMAP_H
#define ARCADEGAMESGLFW_SNAKEMAP_H


#include <string>
#include <glm/vec2.hpp>
#include "vector"

class SnakeMap final {
public:
    SnakeMap();

    enum Tile{
        None,
        Wall
    };
    [[nodiscard]] const glm::vec<2, unsigned int> &getField() const{
        return field;
    }

    [[nodiscard]] const std::vector<Tile> &getMap() const {
        return Map;
    }

    [[nodiscard]] unsigned short getDefaultSnakeSize() const {
        return defaultSnakeSize;
    }

    static SnakeMap* load(const std::string& name);

private:
    glm::vec<2, unsigned int> field {10};
    unsigned short defaultSnakeSize = 4;
    std::vector<Tile> Map {};
};


#endif //ARCADEGAMESGLFW_SNAKEMAP_H
