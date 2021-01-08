#pragma once
#include <iostream>
#include <string.h> 
#include <stdio.h> 
#include <fstream>
#include <vector>
#include <string>
#include "OGLError.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/ext.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <math.h>
using namespace std;

float** loadModelFromOBJ(string fname, int* vCount, int groups = 1, bool tangents = false);