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
        m->field = glm::vec<2, unsigned int>(std::stoi(word), std::stoi(word));
        str >> word;
        m->defaultSnakeSize = std::stoi(word);
        str >> word;
        m->Map.clear();
        for (auto& e:word) {
            m->Map.push_back(static_cast<Tile>((int) (e - '0')));
        };
    } else {
        Error(("Cannot open " + name).c_str())
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
