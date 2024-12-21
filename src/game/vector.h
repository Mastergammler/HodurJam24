#pragma once

#include "imports.h"

struct v2
{
    int x, y;
};

inline const v2 operator+(const v2 vector, const v2 other)
{
    return {vector.x + other.x, vector.y + other.y};
}

inline const v2 operator-(const v2 vector, const v2 other)
{
    return {vector.x - other.x, vector.y - other.y};
}

inline bool operator==(const v2 vector, const v2 other)
{
    return vector.x == other.x && vector.y == other.y;
}

inline bool operator!=(const v2 vector, const v2 other)
{
    return vector.x != other.x || vector.y != other.y;
}

inline bool operator<=(const v2 vector, const int value)
{
    return vector.x <= value && vector.y <= value;
}

inline bool operator>(const v2 vector, const int value)
{
    return vector.x > value && vector.y > value;
}

inline int walk_distance(const v2 vector, const v2 other)
{
    v2 distance = vector - other;
    return abs(distance.x) + abs(distance.y);
}
