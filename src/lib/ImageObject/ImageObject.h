#pragma once
#ifdef IMAGEOBJECT_EXPORTS
#define IMAGEOBJECT_API __declspec(dllexport)
#else
#define IMAGEOBJECT_API __declspec(dllimport)
#endif

class IMAGEOBJECT_API ImageObject {
public:
    ImageObject();
    ~ImageObject();
};