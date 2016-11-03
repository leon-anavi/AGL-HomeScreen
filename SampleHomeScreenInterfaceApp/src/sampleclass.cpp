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


sRectangle SampleClass::getLayoutRenderAreaForSurfaceId(int surfaceId)
{
    mp_libHomeScreen->getLayoutRenderAreaForSurfaceId(surfaceId);
}

void SampleClass::hardKeyPressed(int key)
{
    mp_libHomeScreen->hardKeyPressed(key);
}

void SampleClass::renderSurfaceToArea(int surfaceId, const sRectangle &renderArea)
{
    mp_libHomeScreen->renderSurfaceToArea(surfaceId, renderArea);
}

void SampleClass::requestSurfaceIdToFullScreen(int surfaceId)
{
    mp_libHomeScreen->requestSurfaceIdToFullScreen(surfaceId);
}
