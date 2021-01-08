#pragma once
#include <GL/glew.h>
#include <iostream>
#define DEBUG
#define USING_MICROSOFT_BRKPTS

using namespace std;

inline void GLAPIENTRY errorCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam) {
    cout << "OPENGL ERROR: " << type << ':' << endl;
    cout << message << endl;
#ifdef USING_MICROSOFT_BRKPTS
    __debugbreak();
#else
    int __r;
    cin >> __r;
#endif
}

//replace with a macro
inline void applicationErrorCallback(const string message) {
    //cout << "message" << endl;
#ifdef DEBUG
    cout << "Application Error: " << endl;
    cout << message << endl;
#ifdef USING_MICROSOFT_BRKPTS 
    __debugbreak();
#else
    int __r;
    cin >> __r;
#endif
#endif
}


inline void startGLDebug() { 
#ifdef DEBUG
    glDebugMessageCallback(&errorCallback, nullptr);
#endif 
}