//
// Created by super on 22.06.2024.
//

#include <fstream>
#include "SnakeMap.h"
#include "debug.h"

SnakeMap* SnakeMap::load(const std::string& name) {
    auto m = new SnakeMap();
    std::ifstream str(name, std::ios::in);
    if (str.is_open())
    {
        std::string word;
        str >> word;
        m->field.x = std::stoi(word);
        str >> word;
        m->field.y = std::stoi(word);
        str >> word;
        m->defaultSnakeSize = std::stoi(word);
        str >> word;
        m->Map.clear();
        for (auto& e:word) {
            m->Map.push_back(static_cast<Tile>((int) (e - '0')));
        };
        spdlog::info("{} loaded", name);
    } else {
        spdlog::warn("Cannot open {}", name);
        return nullptr;
    }
    str.close();
    return m;
}

SnakeMap::SnakeMap() {
    field = glm::vec<2, unsigned int>(10);
    for (int i = 0; i < field.x * field.y; ++i) {
        Map.push_back(Tile::None);
    }
}

void SnakeMap::save(const std::string &name, SnakeMap *map) {
    std::ofstream s(name);
    if (s.is_open())
    {
        s << map->getField().x << ' ' << map->getField().y << ' ' << map->getDefaultSnakeSize() << ' ';
        for (auto& m : map->getMap()) {
            s << m;
        }
        spdlog::info("{} saved", name);
        s.close();
    } else {
        spdlog::warn("Cannot saved {}: !s.is_open()", name);
    }
}
