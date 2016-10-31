#ifndef LIBHOMESCREEN_HPP
#define LIBHOMESCREEN_HPP

// forward declarations
struct _LibHomeScreenHomescreen;
typedef struct _LibHomeScreenHomescreen LibHomeScreenHomescreen;

class LibHomeScreen
{
public:
    LibHomeScreen();
    ~LibHomeScreen();

    // these are representing the D-Bus methods:
    void hardKeyPressed(int key);
    void toggleFullScreen();

private:
    LibHomeScreenHomescreen *mp_libHomeScreenHomescreen_Proxy;
};

#endif // LIBHOMESCREEN_HPP
