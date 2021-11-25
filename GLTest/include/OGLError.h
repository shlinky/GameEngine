#pragma once
#include <GL/glew.h>
#include <iostream>
#define DEBUG

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
#ifdef USING_MICROSOFT_BRKPTS_GOOD
    __debugbreak();
#else
    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        int __r;
        cin >> __r;
    }
#endif
}

inline void applicationErrorCallback(const string message) {
#ifdef DEBUG
    cout << endl;
    cout << "Application Error: " << endl;
    cout << message << endl;
#ifdef USING_MICROSOFT_BRKPTS_GOOD 
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