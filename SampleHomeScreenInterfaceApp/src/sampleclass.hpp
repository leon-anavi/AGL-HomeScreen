#ifndef SAMPLECLASS_HPP
#define SAMPLECLASS_HPP

#include "libhomescreen.hpp"
#include <list>

class SampleClass
{
public:
    SampleClass();
    ~SampleClass();

    std::list<int> getAllSurfacesOfProcess(int pid);
    sRectangle getLayoutRenderAreaForSurfaceId(int surfaceId);
    int getSurfaceStatus(int surfaceId);
    void hardKeyPressed(int key);
    void renderSurfaceToArea(int surfaceId, const sRectangle &renderArea);
    void requestSurfaceIdToFullScreen(int surfaceId);

private:
    LibHomeScreen *mp_libHomeScreen;
};

#endif // SAMPLECLASS_HPP
