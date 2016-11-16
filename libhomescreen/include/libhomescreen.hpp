#ifndef LIBHOMESCREEN_HPP
#define LIBHOMESCREEN_HPP

#include <vector>

// forward declarations
struct _LibHomeScreenHomescreen;
typedef struct _LibHomeScreenHomescreen LibHomeScreenHomescreen;

typedef struct
{
    int x;
    int y;
    int width;
    int height;
} sRectangle;

class LibHomeScreen
{
public:
    LibHomeScreen();
    ~LibHomeScreen();

    // these are representing the D-Bus methods:
    std::vector<int> getAllSurfacesOfProcess(int pid);
    int getSurfaceStatus(int surfaceId);
    void hardKeyPressed(int key);
    void renderSurfaceToArea(int surfaceId, int layoutArea);
    bool renderAppToAreaAllowed(int appCategory, int layoutArea);
    void requestSurfaceIdToFullScreen(int surfaceId);
private:
    LibHomeScreenHomescreen *mp_libHomeScreenHomescreen_Proxy;
};

#endif // LIBHOMESCREEN_HPP

