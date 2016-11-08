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

std::list<int> SampleClass::getAllSurfacesOfProcess(int pid)
{
    return mp_libHomeScreen->getAllSurfacesOfProcess(pid);
}

sRectangle SampleClass::getLayoutRenderAreaForSurfaceId(int surfaceId)
{
    return mp_libHomeScreen->getLayoutRenderAreaForSurfaceId(surfaceId);
}

void SampleClass::hardKeyPressed(int key)
{
    mp_libHomeScreen->hardKeyPressed(key);
}

int SampleClass::getSurfaceStatus(int surfaceId)
{
    return mp_libHomeScreen->getSurfaceStatus(surfaceId);
}

void SampleClass::renderSurfaceToArea(int surfaceId, const sRectangle &renderArea)
{
    mp_libHomeScreen->renderSurfaceToArea(surfaceId, renderArea);
}

void SampleClass::requestSurfaceIdToFullScreen(int surfaceId)
{
    mp_libHomeScreen->requestSurfaceIdToFullScreen(surfaceId);
}
