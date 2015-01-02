#pragma once

#include "cinder/Vector.h"

#include <vector>

class Path3D
{
public:
    struct Point
    {
        ci::Vec3f position;
        ci::Vec3f left;
        ci::Vec3f forward;
        
        Point(const ci::Vec3f &position, const ci::Vec3f &left)
        :
        position(position),
        left(left)
        {}
    };
    
    struct Value
    {
        ci::Vec3f position;
        ci::Vec3f left;
        ci::Vec3f forward;
        ci::Vec3f up;
        float offset;
    };
    
    std::vector<Point> points;
    std::vector<float> lengths;
    
    Path3D() = default;
    Path3D(int capacity);

    int size() const;
    bool empty() const;
    
    void begin();
    void end();
    
    float getLength() const;
    void add(const ci::Vec3f &position, const ci::Vec3f &left);
    
    Path3D::Value offset2Value(float offset) const;
    static void applyValue(const Path3D::Value &value, float *m);
};
