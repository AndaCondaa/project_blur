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
    else if (matImg.channels() > 1)
    {
        std::cout << "[ImageObject] Not Grayscale Image : " + m_strPath << std::endl;
        return false;
    }
    else
    {
        m_nWidth = matImg.cols;
        m_nHeight = matImg.rows;
        m_nPixelBytes = matImg.elemSize();
        m_nImageType = matImg.type();

        int size = m_nWidth * m_nHeight * m_nPixelBytes;
        unsigned char* src = matImg.data;
        unsigned char* dst = new unsigned char[size];
        std::memcpy(dst, src, size);
        m_pBuffer = dst;
    }
    return true;
}

bool ImageObject::ImageSave()
{
    std::filesystem::path dirPath = std::filesystem::path(m_strPath).parent_path();

    if (!std::filesystem::exists(dirPath)) {
        if (!std::filesystem::create_directories(dirPath)) {
            std::cerr << "디렉토리 생성 실패: " << dirPath << std::endl;
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
    unsigned char* src = _src.data;
    unsigned char* dst = new unsigned char[size];
    std::memcpy(dst, src, size);
    m_pBuffer = dst;

    return true;
}