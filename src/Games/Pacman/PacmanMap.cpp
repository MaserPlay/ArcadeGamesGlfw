//
// Created by super on 22.06.2024.
//

#include <fstream>
#include "PacmanMap.h"
#include "debug.h"

PacmanMap* PacmanMap::load(const std::string& name) {
    auto m = new PacmanMap();
    std::ifstream str(name, std::ios::in);
    if (str.is_open())
    {
        std::string word;
        try {
            str >> word;
            m->field.x = std::stoi(word);
            str >> word;
            m->field.y = std::stoi(word);
            str >> word;
            m->StartPos.x = std::stoi(word);
            str >> word;
            m->StartPos.y = std::stoi(word);
            str >> word;
            m->Map.clear();
            for (auto& e:word) {
                m->Map.push_back(static_cast<Tile>((int) (e - '0')));
            };
        } catch (const std::invalid_argument& ia) {
            str.close();
            SPDLOG_ERROR("Failed to open map std::invalid_argument");
            return nullptr;
        } catch (const std::out_of_range& oor) {
            str.close();
            SPDLOG_ERROR("Failed to open map std::out_of_range");
            return nullptr;
        }
    } else {
        spdlog::error("Cannot open " + name);
        return nullptr;
    }
    str.close();
    if (m->getField().x <= 0 || m->getField().y <= 0)
    {
        SPDLOG_ERROR("field.x <= 0 || field.y <= 0");
        return nullptr;
    }
    if (m->getField().y != m->getField().x)
    {
        SPDLOG_ERROR("field.y != field.x, which is unsupported");
    }
    return m;
}

PacmanMap::PacmanMap() {
    field = glm::vec<2, unsigned int>(10);
    for (int i = 0; i < field.x * field.y; ++i) {
        Map.push_back(Tile::None);
    }
}

void PacmanMap::save(const std::string &name, PacmanMap *map) {
    std::ofstream s(name);
    if (s.is_open())
    {
        s << map->getField().x << " " << map->getField().y << " " << map->getStartPos().x << " " << map->getStartPos().y << " ";
        for (auto& m : map->getMap()) {
            s << m;
        }
        spdlog::info("{} saved", name);
        s.close();
    } else {
        spdlog::warn("Cannot saved {}: !s.is_open()", name);
    }
}
