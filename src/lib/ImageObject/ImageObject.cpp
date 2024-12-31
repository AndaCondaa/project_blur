#include "pch.h"
#include "ImageObject.h"
#include "opencv2/opencv.hpp"

ImageObject::~ImageObject() {

}

bool ImageObject::ImageLoad(std::string _strPath, std::string& _strError)
{
    cv::Mat matImg = cv::imread(_strPath, cv::IMREAD_UNCHANGED);

    if (matImg.empty())
    {
        _strError = "[ImageObject] Fail Path Error : " + _strPath;
        return false;
    }
    else if (matImg.channels() > 1)
    {
        _strError = "[ImageObject] Not Grayscale Image : " + _strPath;
        return false;
    }
    else
    {
        m_nWidth = matImg.cols;
        m_nHeight = matImg.rows;
    }


    std::cout << m_nWidth << std::endl;
    std::cout << m_nHeight << std::endl;

    return true;
}

bool ImageObject::CovertToMat(cv::Mat& _dst)
{
    return true;
}