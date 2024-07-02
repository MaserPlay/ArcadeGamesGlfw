//
// Created by super on 22.06.2024.
//

#ifndef ARCADEGAMESGLFW_SNAKEMAP_H
#define ARCADEGAMESGLFW_SNAKEMAP_H


#include <glm/detail/type_vec2.hpp>
#include <vector>

class PacmanMap final {
public:
    PacmanMap();

    enum Tile{
        None,
        Wall,
        Point,
        BigPoint
    };
    [[nodiscard]] const glm::vec<2, unsigned int> &getField() const{
        return field;
    }

    [[nodiscard]] const std::vector<Tile> &getMap() const {
        return Map;
    }

    [[nodiscard]] const glm::vec<2, unsigned int> &getGhostSpawnZone() const {
        return GhostSpawnZone;
    }

    static PacmanMap* load(const std::string& name);

    static void save(const std::string& name, PacmanMap* map);

private:
    glm::vec<2, unsigned int> field {10};
    glm::vec<2, unsigned int> GhostSpawnZone {0};
    std::vector<Tile> Map {};
};


#endif //ARCADEGAMESGLFW_SNAKEMAP_H
