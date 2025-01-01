#include "pch.h"
#include "OpencvDLL.h"
#include "opencv2/opencv.hpp"

#include <iostream>

namespace OpencvDLL {
    bool ImageBlur(const ImageObject* src, ImageObject* dst, const int kernelSize)
    {
        cv::Mat matSrc(src->m_nHeight, src->m_nWidth, src->m_nImageType, src->m_pBuffer);
        cv::Mat matDst(src->m_nHeight, src->m_nWidth, src->m_nImageType);
        cv::blur(matSrc, matDst, cv::Size(kernelSize, kernelSize));
        dst->MatToObj(matDst);
        return true;
    }
}