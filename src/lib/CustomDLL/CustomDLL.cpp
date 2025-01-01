#include "pch.h"
#include "CustomDLL.h"
#include <iostream>
#include <cmath>

namespace CustomDLL {
    template <typename T>
    void ProcBlur(T* buffer, int width, int height, int kernelSize) {
        std::cout << "start" << std::endl;

        // kernel 크기 조정 (홀수로 만들어줌)
        if (kernelSize % 2 == 0)
            kernelSize = kernelSize + 1;

        int size = width * height;
        int totalKernelSize = kernelSize * kernelSize;
        int padSize = (kernelSize - 1) / 2;
        T* output = new T[size];
        T sum = 0;
        int ix, iy, idx;

        std::cout << "for start" << std::endl;
        for (int y = 0; y < height; y++) 
        {
            if (y % 100 == 0)
                std::cout << y << std::endl;
            for (int x = 0; x < width; x++) 
            {
                sum = 0;

                for (int ky = -padSize; ky <= padSize; ky++) 
                {
                    for (int kx = -padSize; kx <= padSize; kx++) 
                    {
                        iy = y + ky;
                        ix = x + kx;


                        if (ix < 0 || ix >= width || iy < 0 || iy >= height) 
                        {
                            sum += 0;
                        }
                        else
                        {
                            sum += std::round(static_cast<double>(buffer[ix + iy * width]) / totalKernelSize);
                        }
                    }
                }

                output[x + y * width] = sum;
            }
        }

        std::cout << "for end" << std::endl;
        std::memcpy(buffer, output, size * sizeof(T));

        std::cout << "end" << std::endl;
        delete[] output;
    }

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

        if (dst->m_nPixelBytes == 1) 
        {// 8bit
            unsigned char* buffer = inputBuffer;

            // Blur
            ProcBlur(buffer, dst->m_nWidth, dst->m_nHeight, kernelSize);
            std::memcpy(result, buffer, imageSize);

            delete[] buffer;
        }
        else if (dst->m_nPixelBytes == 2) 
        {// 16bit
            unsigned short* buffer = new unsigned short[pixelSize];

            // 8bit to 16bit
            for (int i = 0; i < pixelSize; ++i) 
            {
                buffer[i] = (static_cast<unsigned short>(inputBuffer[2 * i]) << 8) | inputBuffer[2 * i + 1];
            }

            // Blur
            ProcBlur(buffer, dst->m_nWidth, dst->m_nHeight, kernelSize);

            // 16bit to 8bit
            for (int i = 0; i < pixelSize; ++i) 
            {
                result[2 * i] = static_cast<unsigned char>(buffer[i] >> 8);
                result[2 * i + 1] = static_cast<unsigned char>(buffer[i] & 0xFF);
            }

            delete[] buffer;
        }
        else if (dst->m_nPixelBytes == 4) 
        {// 32bit
            unsigned int* buffer = new unsigned int[pixelSize];

            // 8bit to 32bit
            for (int i = 0; i < pixelSize; ++i) 
            {
                buffer[i] = (static_cast<unsigned int>(inputBuffer[4 * i]) << 24) |
                            (static_cast<unsigned int>(inputBuffer[4 * i + 1]) << 16) |
                            (static_cast<unsigned int>(inputBuffer[4 * i + 2]) << 8) |
					        inputBuffer[4 * i + 3];
            }

            // Blur
            ProcBlur(buffer, dst->m_nWidth, dst->m_nHeight, kernelSize);

            // 32bit to 8bit
            for (int i = 0; i < pixelSize; ++i) 
            {
                result[4 * i] = static_cast<unsigned char>(buffer[i] >> 24);  // 32bit -> 8bit로 변환 (상위 바이트)
                result[4 * i + 1] = static_cast<unsigned char>(buffer[i] >> 16);
                result[4 * i + 2] = static_cast<unsigned char>(buffer[i] >> 8);
                result[4 * i + 3] = static_cast<unsigned char>(buffer[i] & 0xFF);  // 하위 바이트
            }

            delete[] buffer;
        }

        dst->m_pBuffer = result;
        return true;
    }


}