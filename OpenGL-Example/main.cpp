#pragma once
#include <iostream>

#include "../OpenGL-Example/Framework/Framework.h"

// Example
#include "../OpenGL-Example/Example/FieldOfView/FieldOfView2D.h"
#include "../OpenGL-Example/Example/Parabola/Parabola2D.h"
using namespace std;

int main(int argc, char* argv[])
{
	if (Framework* FWork = new FieldOfView2D())
		FWork->MainFunc(argc, argv);

	/*if (Framework* FWork = new Parabola2D())
		FWork->MainFunc(argc, argv);*/

	return 0;
}

