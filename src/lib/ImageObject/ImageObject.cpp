#include "pch.h"
#include "ImageObject.h"
#include "opencv2/opencv.hpp"
#include <filesystem>

ImageObject::~ImageObject() {

}

void ImageObject::SetPath(std::string _strPath)
{
    m_strPath = _strPath;
}

bool ImageObject::ImageLoad()
{
    if (m_strPath == "")
    {
        std::cout << "[ImageObject] Not Setting Path" << std::endl;
        return false;
    }

    cv::Mat matImg = cv::imread(m_strPath, cv::IMREAD_UNCHANGED);

    if (matImg.empty())
    {
        std::cout << "[ImageObject] Fail Path Error : " + m_strPath << std::endl;
        return false;
    }
    else
    {
        m_nWidth = matImg.cols;
        m_nHeight = matImg.rows;
        m_nPixelBytes = matImg.elemSize();
        m_nImageType = matImg.type();

        int pixelCount = m_nWidth * m_nHeight;


        // 타입확인
        if (m_nImageType == CV_8UC1)
        {
            unsigned char* buffer = new unsigned char[pixelCount];
            std::memcpy(buffer, matImg.data, pixelCount * sizeof(unsigned char));
            m_pBuffer = buffer;
        }
        else if (m_nImageType == CV_16UC1)
        {
            unsigned short* buffer = new unsigned short[pixelCount];
            std::memcpy(buffer, matImg.data, pixelCount * sizeof(unsigned short));
            m_pBuffer = buffer;
        }
        else if (m_nImageType == CV_32FC1)
        {
            float* buffer = new float[pixelCount];
            std::memcpy(buffer, matImg.data, pixelCount * sizeof(float));
            m_pBuffer = buffer;
        }
        else
        {
            std::cout << "[ImageObject] Not Support color image" << std::endl;
            return false;
        }
    }

    return true;
}

bool ImageObject::ImageSave()
{
    std::filesystem::path dirPath = std::filesystem::path(m_strPath).parent_path();

    if (!std::filesystem::exists(dirPath)) 
    {
        if (!std::filesystem::create_directories(dirPath)) 
        {
            std::cout << "[ImageObject] Making Dir Error : " << dirPath << std::endl;
            return false;
        }
    }

    cv::Mat newMat(m_nHeight, m_nWidth, m_nImageType, m_pBuffer);
    bool success = cv::imwrite(m_strPath, newMat);
   
    return success;
}

bool ImageObject::MatToObj(cv::Mat _src)
{
    m_nWidth = _src.cols;
    m_nHeight = _src.rows;
    m_nPixelBytes = _src.elemSize();
    m_nImageType = _src.type();

    int size = m_nWidth * m_nHeight * m_nPixelBytes;
    unsigned char* dst = new unsigned char[size];
    std::memcpy(dst, _src.data, size);
    m_pBuffer = dst;

    return true;
}