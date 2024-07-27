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
    if (m->getField().x <= 0 || m->getField().y <= 0)
    {
        SPDLOG_ERROR("field.x <= 0 || field.y <= 0"); return nullptr;
    }
    if (m->getDefaultSnakePos().x <= 0 || m->getDefaultSnakePos().y <= 0)
    {
        SPDLOG_ERROR("DefaultSnakePos.x <= 0 || DefaultSnakePos.y <= 0"); return nullptr;
    }
    if (m->getDefaultSnakePos().x >= m->getField().x || m->getDefaultSnakePos().y >= m->getField().y)
    {
        SPDLOG_ERROR("m->getDefaultSnakePos().x >= m->getField().x || m->getDefaultSnakePos().y >= m->getField().y"); return nullptr;
    }
    if (m->getDefaultSnakeDirection() >= DIRECTION_LAST)
    {
        SPDLOG_ERROR("m->getDefaultSnakeDirection() >= DIRECTION_LAST"); return nullptr;
    }
    if (m->getField().y != m->getField().x)
    {
        SPDLOG_ERROR("field.y != field.x, which is unsupported"); return nullptr;
    }
    if (m->getDefaultSnakeSize() < 2)
    {
        SPDLOG_ERROR("DefaultSnakeSize < 2"); return nullptr;
    }
    if (m->getTickSpeed() <= 0)
    {
        SPDLOG_ERROR("TickSpeed <= 0"); return nullptr;
    }
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
