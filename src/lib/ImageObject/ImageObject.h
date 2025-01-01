#pragma once
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"

#ifdef IMAGEOBJECT_EXPORTS
#define IMAGEOBJECT_API __declspec(dllexport)
#else
#define IMAGEOBJECT_API __declspec(dllimport)
#endif

class IMAGEOBJECT_API ImageObject {
public:
    ImageObject() {};
    ImageObject(std::string _path) {
        m_strPath = _path;
    };
    ~ImageObject();

public:
    void SetPath(std::string _strPath);
    bool ImageLoad();
    bool ImageSave();
    bool MatToObj(cv::Mat _src);

public:
    std::string m_strPath = "";
    int m_nWidth;
    int m_nHeight;
    int m_nPixelBytes;
    int m_nImageType;
    unsigned char* m_pBuffer;
    
};