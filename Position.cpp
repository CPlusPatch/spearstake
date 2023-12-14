/**
 * Copyright 2023 Gaspard Wierzbinski
 * @file Position.cpp
 * @brief Stores world position as a 3D vector
 * @details This file contains the class for a 3D vector that stores the world position of an object
 */

#include <GL/glew.h>

class Position
{
public:
    Position(const float &x, const float &y, const float &z)
        : x(x), y(y), z(z)
    {
    }

    ~Position()
    {
    }

    Position operator+(const Position &other) const
    {
        return Position(x + other.x, y + other.y, z + other.z);
    }

    Position operator-(const Position &other) const
    {
        return Position(x - other.x, y - other.y, z - other.z);
    }

    void update()
    {
    }

    void render()
    {
    }

    float getX()
    {
        return x;
    }

    float getY()
    {
        return y;
    }

    float getZ()
    {
        return z;
    }

private:
    float x;
    float y;
    float z;
};