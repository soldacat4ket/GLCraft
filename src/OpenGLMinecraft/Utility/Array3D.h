#pragma once

#include <vector>

// macro for instanciating a new array3d template since its being really annoying
#define INST_A3D(type) template class Array3D<type>

template<typename T>
class Array3D
{
public:
    Array3D(size_t p_SizeX, size_t p_SizeY, size_t p_SizeZ);

    // access the array
    T& operator()(size_t p_X, size_t p_Y, size_t p_Z);
    const T& operator()(size_t p_X, size_t p_Y, size_t p_Z) const;

    void Fill(T& p_FillValue);
    void Fill(const T& p_FillValue);

    inline size_t Size() const { return m_TotalSize; }
    inline size_t SizeX() const { return m_SizeX; }
    inline size_t SizeY() const { return m_SizeY; }
    inline size_t SizeZ() const { return m_SizeZ; }

    // get unabstracted 1d array
    inline T* Data() { return m_Data.data(); };
    inline const T* Data() const { return m_Data.data(); };

protected:
    size_t m_TotalSize;
    size_t m_SizeX;
    size_t m_SizeY;
    size_t m_SizeZ;
    std::vector<T> m_Data;
};
