#pragma once

// convert value from range min > max to a float scaled between fmin and fmax 
inline float Normalize(const int value, const int min, const int max, const float fmin = -1.0f, const float fmax = 1.0f)
{
    float valueRatio = (float)(value - min) / (max - min);
    float fullRange = fmax - fmin;
    return valueRatio * fullRange + fmin;
}

inline int Denormalize(float value, int min, int max, float fmin = -1.0f, float fmax = 1.0f)
{

}