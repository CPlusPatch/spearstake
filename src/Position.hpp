#ifndef POSITION_HPP
#define POSITION_HPP

#include <glm/glm.hpp>

class Position
{
public:
    Position(const float &x, const float &y, const float &z);
    ~Position();

    Position operator+(const Position &other) const;
    Position operator-(const Position &other) const;

    void update();
    void render();

    float getX();
    float getY();
    float getZ();

    glm::vec3 toVec3() const;

private:
    float x;
    float y;
    float z;
};

#endif // POSITION_HPP