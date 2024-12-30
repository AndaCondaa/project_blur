#pragma once
#include "ImageObject/ImageObject.h"

#ifdef OPENCVDLL_EXPORTS
#define OPENCVDLL_API __declspec(dllexport)
#else
#define OPENCVDLL_API __declspec(dllimport)
#endif

// �Լ� ����
namespace OpencvDLL {
    OPENCVDLL_API bool ImageBlur(const ImageObject* src, ImageObject* dst, const int kernelSize);
}

