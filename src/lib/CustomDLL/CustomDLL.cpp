#include "pch.h"
#include "CustomDLL.h"
#include <iostream>
#include <cmath>


template <typename T>
void ProcBlur(T* buffer, int width, int height, int kernelSize)
{

    if (kernelSize % 2 == 0)
        kernelSize += 1;

    int padSize = kernelSize / 2;
    int size = width * height;


    std::vector<unsigned long long> vecSum(size, 0);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned long long current = buffer[x + y * width];

            unsigned long long left = 0;
            if (x > 0) {
                left = vecSum[x - 1 + y * width];
            }

            unsigned long long top = 0;
            if (y > 0) {
                top = vecSum[x + (y - 1) * width];
            }

            unsigned long long topLeft = 0;
            if (x > 0 && y > 0) {
                topLeft = vecSum[x - 1 + (y - 1) * width];
            }

            vecSum[x + y * width] = current + left + top - topLeft;
        }
    }

    T* output = new T[size];

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int l = x - padSize;
            if (l < 0)
                l = 0;

            int r = x + padSize;
            if (r >= width)
                r = width - 1;

            int t = y - padSize;
            if (t < 0)
                t = 0;

            int b = y + padSize;
            if (b >= height)
                b = height - 1;

            unsigned long long sum = vecSum[r + b * width];

            if (l > 0)
                sum -= vecSum[l - 1 + b * width];

            if (t > 0)
                sum -= vecSum[r + (t - 1) * width];

            if (l > 0 && t > 0)
                sum += vecSum[l - 1 + (t - 1) * width];

            int blurValue = (r - l + 1) * (b - t + 1);
            output[x + y * width] = static_cast<T>(std::round(static_cast<double>(sum) / blurValue));
        }
    }

    std::memcpy(buffer, output, size * sizeof(T));
    delete[] output;
}


namespace CustomDLL {
    bool ImageBlur(const ImageObject* src, ImageObject* dst, const int kernelSize)
    {
        dst->m_nWidth = src->m_nWidth;
        dst->m_nHeight = src->m_nHeight;
        dst->m_nPixelBytes = src->m_nPixelBytes;
        dst->m_nImageType = src->m_nImageType;

        int pixelSize = dst->m_nWidth * dst->m_nHeight;
        int imageSize = pixelSize * dst->m_nPixelBytes;
        unsigned char* inputBuffer = new unsigned char[imageSize];
        std::memcpy(inputBuffer, src->m_pBuffer, imageSize);

        unsigned char* result = new unsigned char[imageSize];

        if (dst->m_nImageType == CV_8UC1)
        {
            unsigned char* buffer = reinterpret_cast<unsigned char*>(inputBuffer);
            ProcBlur(buffer, dst->m_nWidth, dst->m_nHeight, kernelSize);
            std::memcpy(result, buffer, imageSize);
        }
        else if (dst->m_nImageType == CV_16UC1)
        {
            unsigned short* buffer = reinterpret_cast<unsigned short*>(inputBuffer);
            ProcBlur(buffer, dst->m_nWidth, dst->m_nHeight, kernelSize);
            std::memcpy(result, buffer, imageSize);
        }
        else if (dst->m_nImageType == CV_32FC1)
        {
            float* buffer = reinterpret_cast<float*>(inputBuffer);
            ProcBlur(buffer, dst->m_nWidth, dst->m_nHeight, kernelSize);
            std::memcpy(result, buffer, imageSize);
        }

        dst->m_pBuffer = result;
        delete[] inputBuffer;
        return true;
    }
}