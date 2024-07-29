//
// Created by super on 22.06.2024.
//

#include <fstream>
#include "SnakeMap.h"
#include "debug.h"
#include "Localization.h"

SnakeMap* SnakeMap::load(const std::string& name) {
    auto m = new SnakeMap();
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
            m->DefaultSnakePos.x = std::stoi(word);
            str >> word;
            m->DefaultSnakePos.y = std::stoi(word);
            str >> word;
            m->DefaultSnakeDirection = static_cast<Directions>(std::stoi(word));
            str >> word;
            m->tickSpeed = std::stof(word);
            str >> word;
            m->defaultSnakeSize = std::stoi(word);
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
        SPDLOG_INFO("{} loaded", name.substr(name.find_last_of('\\'),name.size() - name.find_last_of('\\')));
    } else {
        SPDLOG_WARN("Cannot open {}", name);
        return nullptr;
    }
    str.close();
    //CHECK
    SNAKE_MAP_ERROR(m->getField().x <= 0 || m->getField().y <= 0)
    SNAKE_MAP_ERROR(m->getDefaultSnakePos().x <= 0 || m->getDefaultSnakePos().y <= 0)
    SNAKE_MAP_ERROR(m->getDefaultSnakePos().x >= m->getField().x || m->getDefaultSnakePos().y >= m->getField().y)
    SNAKE_MAP_ERROR(m->getDefaultSnakeDirection() >= DIRECTION_LAST)
    SNAKE_MAP_ERROR(m->getField().y != m->getField().x)
    SNAKE_MAP_ERROR(m->getDefaultSnakeSize() < 2)
    SNAKE_MAP_ERROR(m->getTickSpeed() <= 0)
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
        s << map->getField().x << ' ' << map->getField().y << ' '
        << map->getDefaultSnakePos().x << ' ' << map->getDefaultSnakePos().y << ' '
        << map->getDefaultSnakeDirection() << ' '
        << map->getTickSpeed() << ' ' << map->getDefaultSnakeSize() << ' ';
        for (auto& m : map->getMap()) {
            s << m;
        }
        SPDLOG_INFO("{} saved", name);
        s.close();
    } else {
        SPDLOG_WARN("Cannot saved {}: !s.is_open()", name);
    }
}
