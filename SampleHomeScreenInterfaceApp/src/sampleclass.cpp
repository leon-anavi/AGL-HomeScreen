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

std::vector<int> SampleClass::getAllSurfacesOfProcess(int pid)
{
    return mp_libHomeScreen->getAllSurfacesOfProcess(pid);
}

void SampleClass::hardKeyPressed(int key)
{
    mp_libHomeScreen->hardKeyPressed(key);
}

int SampleClass::getSurfaceStatus(int surfaceId)
{
    return mp_libHomeScreen->getSurfaceStatus(surfaceId);
}

void SampleClass::renderSurfaceToArea(int surfaceId, int layoutArea)
{
    mp_libHomeScreen->renderSurfaceToArea(surfaceId, layoutArea);
}

bool SampleClass::renderAppToAreaAllowed(int appCategory, int layoutArea)
{
    return mp_libHomeScreen->renderAppToAreaAllowed(appCategory, layoutArea);
}

void SampleClass::requestSurfaceIdToFullScreen(int surfaceId)
{
    mp_libHomeScreen->requestSurfaceIdToFullScreen(surfaceId);
}
