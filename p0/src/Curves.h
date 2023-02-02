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

inline float DistanceToInterpolation(float distance, const SpeedTable& speedTable, size_t point, size_t sample)
{
    const size_t sampleCount = speedTable[0].size();
    SamplePoint current = speedTable[point][sample];
    SamplePoint next = speedTable[point][(sample + 1) % sampleCount];
    return Lerp(current.t, next.t, (distance - current.d) / (next.d - current.d));
}

inline void UpdateCatmull(float& distance, size_t& point, size_t& sample,
    const Points& points, const SpeedTable& speedTable)
{
    const size_t sampleCount = speedTable[0].size();
    while (distance > speedTable[point][(sample + 1) % sampleCount].d)
    {
        if (++sample >= sampleCount)
        {
            ++point %= speedTable.size();
            sample = 0;
            distance = 0.0f;
        }
    }
}
