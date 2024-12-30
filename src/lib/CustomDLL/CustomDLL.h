#pragma once
#include "ImageObject/ImageObject.h"

#ifdef CUSTOMDLL_EXPORTS
#define CUSTOMDLL_API __declspec(dllexport)
#else
#define CUSTOMDLL_API __declspec(dllimport)
#endif

// �Լ� ����
namespace CustomDLL {
	CUSTOMDLL_API bool ImageBlur(const ImageObject* src, ImageObject* dst, const int kernelSize);
}

