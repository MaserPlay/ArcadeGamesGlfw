//
// Created by super on 15.06.2024.
//

#ifndef ARCADEGAMES_OTHER_H
#define ARCADEGAMES_OTHER_H


enum Directions{
    Up,
    Left,
    Down,
    Right
};
struct Coords{
    explicit Coords(unsigned int xy) : Coords(xy,xy) {}
    Coords(unsigned int x, unsigned int y) : x(x), y(y) {}

    bool operator<(const Coords &rhs) const {
        if (x < rhs.x)
            return true;
        if (rhs.x < x)
            return false;
        return y < rhs.y;
    }

    bool operator>(const Coords &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const Coords &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const Coords &rhs) const {
        return !(*this < rhs);
    }

    bool operator==(const Coords &rhs) const {
        return x == rhs.x &&
               y == rhs.y;
    }

    bool operator!=(const Coords &rhs) const {
        return !(rhs == *this);
    }

    Coords operator+(const Coords &rhs) const {
        return {rhs.x + this->x, rhs.y + this->y};
    }

    unsigned int x;
    unsigned int y;
};

#endif //ARCADEGAMES_OTHER_H
