#ifndef LIBHOMESCREEN_HPP
#define LIBHOMESCREEN_HPP

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
    sRectangle getLayoutRenderAreaForSurfaceId(int surfaceId);
    void hardKeyPressed(int key);
    void renderSurfaceToArea(int surfaceId, const sRectangle &renderArea);
    void requestSurfaceIdToFullScreen(int surfaceId);

private:
    LibHomeScreenHomescreen *mp_libHomeScreenHomescreen_Proxy;
};

#endif // LIBHOMESCREEN_HPP
