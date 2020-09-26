#pragma once
#include <iostream>
#include <string.h> 
#include <stdio.h> 
#include <fstream>
#include <vector>
#include <string>
#include "OGLError.h"
using namespace std;

float** loadModelFromOBJ(string fname, int* vCount, int groups = 1);