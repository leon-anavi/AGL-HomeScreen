#include <iostream>
#include "sampleclass.hpp"

using namespace std;

int main()
{
    cout << "SampleHomeScreenInterfaceApp" << endl;

    SampleClass *sampleClass = new SampleClass();

    sampleClass->hardKeyPressed(100);
    sampleClass->hardKeyPressed(101);
    sampleClass->hardKeyPressed(102);

    delete sampleClass;

    return 0;
}
