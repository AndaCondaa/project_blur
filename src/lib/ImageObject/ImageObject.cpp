#include "pch.h"
#include "ImageObject.h"
#include "opencv2/opencv.hpp"

ImageObject::ImageObject() {
    std::cout << "IMAGEOBJECT" << std::endl;

    cv::Mat image = cv::imread("C:\\Users\\Simon\\Desktop\\project_blur\\test_bench\\image/1.png", cv::IMREAD_COLOR);

    // 이미지 로드 실패 확인
    if (image.empty())
    {
        printf("error\n");
    }
    else
    {
        std::cout << image.cols << std::endl;
        std::cout << image.rows << std::endl;
    }
}

ImageObject::~ImageObject() {

}