#include <iostream>
#include "sampleclass.hpp"

using namespace std;

int main()
{
    cout << "SampleHomeScreenInterfaceApp" << endl;
    cout << "v0.6.0" << endl;

    SampleClass *sampleClass = new SampleClass();

    //sRectangle test = sampleClass->getLayoutRenderAreaForSurfaceId(1);

    sampleClass->hardKeyPressed(100);
    sampleClass->hardKeyPressed(101);
    sampleClass->hardKeyPressed(102);

    /*sRectangle renderArea;
    renderArea.x = 0;
    renderArea.y = 1;
    renderArea.width = 12;
    renderArea.height = 13;
    sampleClass->renderSurfaceToArea(1, renderArea);*/

    //sampleClass->requestSurfaceIdToFullScreen(1);

    delete sampleClass;

    return 0;
}
