#ifndef SAMPLECLASS_HPP
#define SAMPLECLASS_HPP

#include "libhomescreen.hpp"
#include <vector>

class SampleClass
{
public:
    SampleClass();
    ~SampleClass();

    std::vector<int> getAllSurfacesOfProcess(int pid);
    int getSurfaceStatus(int surfaceId);
    void hardKeyPressed(int key);
    void renderSurfaceToArea(int surfaceId, int layoutArea);
    bool renderAppToAreaAllowed(int appCategory, int layoutArea);
    void requestSurfaceIdToFullScreen(int surfaceId);

private:
    LibHomeScreen *mp_libHomeScreen;
};

#endif // SAMPLECLASS_HPP
