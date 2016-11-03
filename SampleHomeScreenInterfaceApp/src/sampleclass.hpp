#ifndef SAMPLECLASS_HPP
#define SAMPLECLASS_HPP

#include "libhomescreen.hpp"

class SampleClass
{
public:
    SampleClass();
    ~SampleClass();

    sRectangle getLayoutRenderAreaForSurfaceId(int surfaceId);
    void hardKeyPressed(int key);
    void renderSurfaceToArea(int surfaceId, const sRectangle &renderArea);
    void requestSurfaceIdToFullScreen(int surfaceId);

private:
    LibHomeScreen *mp_libHomeScreen;
};

#endif // SAMPLECLASS_HPP
