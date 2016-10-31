#ifndef SAMPLECLASS_HPP
#define SAMPLECLASS_HPP

#include "libhomescreen.hpp"

class SampleClass
{
public:
    SampleClass();
    ~SampleClass();

    void hardKeyPressed(int key);
    void toggleFullScreen();

private:
    LibHomeScreen *mp_libHomeScreen;
};

#endif // SAMPLECLASS_HPP
