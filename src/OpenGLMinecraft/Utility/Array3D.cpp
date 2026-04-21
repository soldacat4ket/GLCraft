#include "Array3D.h"

#include "OpenGLMinecraft/Debug.h"

template<typename T>
Array3D<T>::Array3D(const size_t p_SizeX, const size_t p_SizeY, const size_t p_SizeZ)
    : m_Data(p_SizeX * p_SizeY * p_SizeZ),
    m_TotalSize(p_SizeX * p_SizeY * p_SizeZ),
    m_SizeX(p_SizeX),
    m_SizeY(p_SizeY),
    m_SizeZ(p_SizeZ)
{
}

template<typename T>
T& Array3D<T>::operator()(const size_t p_X, const size_t p_Y, const size_t p_Z)
{
    return m_Data.at((p_X * m_SizeY * m_SizeZ) + (p_Y * m_SizeZ) + p_Z);
}

template<typename T>
const T& Array3D<T>::operator()(const size_t p_X, const size_t p_Y, const size_t p_Z) const
{
    return m_Data.at((p_X * m_SizeY * m_SizeZ) + (p_Y * m_SizeZ) + p_Z);
}

template<typename T>
void Array3D<T>::Fill(T& p_FillValue)
{
    for(size_t i = 0; i < m_TotalSize; i++)
    {
        m_Data[i] = p_FillValue;
    }
}

template<typename T>
void Array3D<T>::Fill(const T& p_FillValue)
{
    for(size_t i = 0; i < m_TotalSize; i++)
    {
        m_Data[i] = p_FillValue;
    }
}

// all fundamental types
//template class Array3D<bool>; // causes problems
template class Array3D<unsigned char>;
template class Array3D<char>;
template class Array3D<wchar_t>;
template class Array3D<unsigned short>;
template class Array3D<short>;
template class Array3D<unsigned int>;
template class Array3D<int>;
template class Array3D<unsigned long>;
template class Array3D<long>;
template class Array3D<unsigned long long>;
template class Array3D<long long>;
template class Array3D<float>;
template class Array3D<long double>;