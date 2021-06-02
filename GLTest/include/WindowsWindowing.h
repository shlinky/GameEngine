#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "OGLError.h"
using namespace std;

class WindowsWindowing
{
public:
	WindowsWindowing(int sizex, int sizey, string title, bool fullscreen = false);
	~WindowsWindowing();
	bool isWindowClosing();
	void prepareForNextFrame();
	int getSizeX();
	int getSizeY();
	//enable get key in the same way
	//build input class on top of all hardware specific classes
	void getMousePos(double* mPos);
	//make static
	//callback input
	void setKeyPress(GLFWkeyfun K);
	bool isKeyPressed(int key);
	bool isMouseButtonPressed(int mouseButton);

	float getTime();
	void resetTime();
private:
	GLFWwindow* window;
	int sizex;
	int sizey;
};

