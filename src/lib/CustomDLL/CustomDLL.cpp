#include "pch.h"
#include "CustomDLL.h"
#include <iostream>

namespace CustomDLL {
    bool ImageBlur(const ImageObject* src, ImageObject* dst, const int kernelSize)
    {
        std::cout << "custom" << std::endl;
        return true;
    }
}