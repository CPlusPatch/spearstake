/**
 * Copyright 2023 Gaspard Wierzbinski
 * @file Position.cpp
 * @brief Stores world position as a 3D vector
 * @details This file contains the class for a 3D vector that stores the world position of an object
 */

#include "Position.hpp"
#include <glm/glm.hpp>

Position::Position(const float &x, const float &y, const float &z)
    : x(x), y(y), z(z)
{
}

Position::~Position()
{
}

Position Position::operator+(const Position &other) const
{
    return Position(x + other.x, y + other.y, z + other.z);
}

Position Position::operator-(const Position &other) const
{
    return Position(x - other.x, y - other.y, z - other.z);
}

void Position::update()
{
}

void Position::render()
{
}

float Position::getX()
{
    return x;
}

float Position::getY()
{
    return y;
}

float Position::getZ()
{
    return z;
}

glm::vec3 Position::toVec3() const
{
    return glm::vec3(x, y, z);
}