#pragma once
#include "MathUtilities.h"
#include <vector>

struct SamplePoint
{
    float t;
    float d;
};

using SpeedTable = std::vector<std::vector<SamplePoint>>;
using Points = std::vector<Vector3>;

struct Spline
{
    Points points;
    size_t point = 0;
    size_t sample = 0;
    float distance = 0.0f;
    SpeedTable speedTable;
};

inline Vector3 NearestProjection(const Vector3& position, const Points& points)
{
    Vector3 nearest;
    float shortestDistance = std::numeric_limits<float>::max();
    for (size_t i = 1; i <= points.size(); i++)
    {
        Vector3 projection = Project(points[i - 1], points[i % points.size()], position);
        float distance = (position - projection).LengthSquared();
        if (distance < shortestDistance)
        {
            shortestDistance = distance;
            nearest = projection;
        }
    }
    return nearest;
}

inline void IndexCatmull(const Points& points, size_t i, Vector3& p0, Vector3& p1, Vector3& p2, Vector3& p3)
{
	size_t n = points.size();
    p0 = points[(i - 1 + n) % n];
    p1 = points[i % n];
    p2 = points[(i + 1) % n];
    p3 = points[(i + 2) % n];
}

inline Vector3 Catmull(float t, size_t i, const Points& points)
{
    Vector3 p0, p1, p2, p3;
    IndexCatmull(points, i, p0, p1, p2, p3);
    return Vector3::CatmullRom(p0, p1, p2, p3, t);
}

inline SpeedTable CreateSpeedTable(const Points& points, size_t samplesPerPoint = 16)
{
    SpeedTable speedTable;
    for (size_t i = 0; i < points.size(); i++)
    {
        std::vector<SamplePoint> samples;
        samples.push_back({ 0.0f, 0.0f });
        float arcLength = 0.0f;
        float step = 1.0f / (float)samplesPerPoint;
        for (float t = step; t <= 1.0f; t += step)
        {
            arcLength += (Catmull(t - step, i, points) - Catmull(t, i, points)).Length();
            samples.push_back({ t, arcLength });
        }
        speedTable.push_back(samples);
    }
    return speedTable;
}

inline float DistanceToInterpolation(const Spline& spline)
{
    const size_t sampleCount = spline.speedTable[0].size();
    SamplePoint current = spline.speedTable[spline.point][spline.sample];
    SamplePoint next = spline.speedTable[spline.point][(spline.sample + 1) % sampleCount];
    return Lerp(current.t, next.t, (spline.distance - current.d) / (next.d - current.d));
}

inline void UpdateCatmull(Spline& spline)
{
    const size_t sampleCount = spline.speedTable[0].size();
    while (spline.distance > spline.speedTable[spline.point][(spline.sample + 1) % sampleCount].d)
    {
        if (++spline.sample >= sampleCount)
        {
            ++spline.point %= spline.speedTable.size();
            spline.sample = 0;
            spline.distance = 0.0f;
        }
    }
}
