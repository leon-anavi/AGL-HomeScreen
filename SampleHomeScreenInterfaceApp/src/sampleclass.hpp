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
    int getSurfaceStatus(int surfaceId);
    void hardKeyPressed(int key);
    void renderSurfaceToArea(int surfaceId, int layoutArea);
    bool renderSurfaceToAreaAllowed(int surfaceId, int layoutArea);
    void requestSurfaceIdToFullScreen(int surfaceId);

private:
    LibHomeScreen *mp_libHomeScreen;
};

#endif // SAMPLECLASS_HPP
