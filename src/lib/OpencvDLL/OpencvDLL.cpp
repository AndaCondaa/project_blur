#include "pch.h"
#include "OpencvDLL.h"
#include "opencv2/opencv.hpp"

#include <iostream>

namespace OpencvDLL {
    bool ImageBlur(const ImageObject* src, ImageObject* dst, const int kernelSize)
    {
        std::cout << "opencv" << std::endl;
        return true;
    }
}