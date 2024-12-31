#pragma once
#ifdef IMAGEOBJECT_EXPORTS
#define IMAGEOBJECT_API __declspec(dllexport)
#else
#define IMAGEOBJECT_API __declspec(dllimport)
#endif
#include <iostream>

class IMAGEOBJECT_API ImageObject {
public:
    ImageObject() {};
    ~ImageObject();

public:
    bool ImageLoad(std::string _strPath, std::string& _strError);
    bool CovertToMat(cv::Mat& _dst);

public:
    int m_nWidth;
    int m_nHeight;
    unsigned char* m_buffer;
};