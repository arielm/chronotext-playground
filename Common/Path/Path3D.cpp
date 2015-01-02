#include "Path3D.h"

#include "chronotext/Context.h"
#include "chronotext/utils/MathUtils.h"

using namespace std;
using namespace ci;
using namespace chr;

Path3D::Path3D(int capacity)
{
    points.reserve(capacity);
    lengths.reserve(capacity);
}

int Path3D::size() const
{
    return points.size();
}

bool Path3D::empty() const
{
    return points.empty();
}

void Path3D::begin()
{
    points.clear();
    lengths.clear();
}

void Path3D::end()
{
    int end = size();
    
    if (end > 1)
    {
        points[end - 1].forward = points[end - 2].forward;
    }
}

float Path3D::getLength() const
{
    if (!empty())
    {
        return lengths.back();
    }
    else
    {
        return 0;
    }
}

void Path3D::add(const Vec3f &position, const Vec3f &left)
{
    if (!empty())
    {
        auto delta = position - points.back().position;
        auto length = delta.length(); // ASSERT: length > EPSILON
        
        lengths.push_back(lengths.back() + length);
        points.back().forward = delta / length;
    }
    else
    {
        lengths.push_back(0);
    }

    points.emplace_back(position, left);
}

Path3D::Value Path3D::offset2Value(float offset) const
{
    if (!empty())
    {
        auto index = utils::search(lengths, offset, 1, size());
        auto &p0 = points[index];
        auto &p1 = points[index + 1];
        
        float u0 = (offset - lengths[index]) / (lengths[index + 1] - lengths[index]);
        float u1 = 1 - u0;
        
        Value value;
        value.position = p0.position * u1 + p1.position * u0;
        value.left = p0.left * u1 + p1.left * u0;
        value.forward = p0.forward * u1 + p1.forward * u0;
        value.up = -value.left.cross(value.forward);
        value.offset = offset;
        
        return value;
    }
    else
    {
        return Value();
    }
}

void Path3D::applyValue(const Value &value, float *m)
{
    m[0] = value.forward.x; m[4] = value.left.x; m[ 8] = value.up.x; m[12] = value.position.x;
    m[1] = value.forward.y; m[5] = value.left.y; m[ 9] = value.up.y; m[13] = value.position.y;
    m[2] = value.forward.z; m[6] = value.left.z; m[10] = value.up.z; m[14] = value.position.z;
    m[3] = 0;               m[7] = 0;            m[11] = 0;          m[15] = 1;
}
