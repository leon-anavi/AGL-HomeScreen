#include "sampleclass.hpp"

SampleClass::SampleClass() :
    mp_libHomeScreen(0)
{
    mp_libHomeScreen = new LibHomeScreen();
}

SampleClass::~SampleClass()
{
    delete mp_libHomeScreen;
}

void SampleClass::hardKeyPressed(int key)
{
    mp_libHomeScreen->hardKeyPressed(key);
}

void SampleClass::toggleFullScreen()
{
    mp_libHomeScreen->toggleFullScreen();
}
