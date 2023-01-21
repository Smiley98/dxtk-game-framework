#pragma once
#include "MathUtilities.h"
#include <vector>

struct SamplePoint
{
    float t;
    float d;
};

using SpeedTable = std::vector<std::vector<SamplePoint>>;
using namespace DirectX::SimpleMath;

inline void IndexCatmull(const std::vector<Vector3>& points, size_t i, Vector3& p0, Vector3& p1, Vector3& p2, Vector3& p3)
{
	size_t n = points.size();
    p0 = points[(i - 1 + n) % n];
    p1 = points[i % n];
    p2 = points[(i + 1) % n];
    p3 = points[(i + 2) % n];
}

inline Vector3 Catmull(float t, size_t i, const std::vector<Vector3>& points)
{
    Vector3 p0, p1, p2, p3;
    IndexCatmull(points, i, p0, p1, p2, p3);
    return Vector3::CatmullRom(p0, p1, p2, p3, t);
}

inline SpeedTable CreateSpeedTable(const std::vector<Vector3>& points, size_t intervals)
{
    SpeedTable speedTable;
    for (size_t i = 0; i < points.size(); i++)
    {
        std::vector<SamplePoint> samples;
        samples.push_back({ 0.0f, 0.0f });
        float arcLength = 0.0f;
        float step = 1.0f / (float)intervals;
        for (float t = step; t <= 1.0f; t += step)
        {
            arcLength += (Catmull(t - step, i, points) - Catmull(t, i, points)).Length();
            samples.push_back({ t, arcLength });
        }
        speedTable.push_back(samples);
    }
    return speedTable;
}

inline float DistanceToInterpolation(float distance, const SpeedTable& speedTable, size_t interval, size_t sample)
{
    const size_t sampleCount = speedTable[0].size();
    SamplePoint current = speedTable[interval][sample];
    SamplePoint next = speedTable[interval][(sample + 1) % sampleCount];
    return Lerp(current.t, next.t, (distance - current.d) / (next.d - current.d));
}

inline void UpdateCatmull(float& distance, size_t& interval, size_t& sample,
    const std::vector<Vector3>& points, const SpeedTable& speedTable)
{
    const size_t sampleCount = speedTable[0].size();
    while (distance > speedTable[interval][(sample + 1) % sampleCount].d)
    {
        if (++sample >= sampleCount)
        {
            ++interval %= speedTable.size();
            sample = 0;
            distance = 0.0f;
        }
    }
}
