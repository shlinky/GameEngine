#include "OGLShaderProgram.h"


OGLShaderProgram::OGLShaderProgram(string vs, string fs, int numUniforms)
{
	pid = CreateShaderProgram(vs, fs);
    uniforms = new OGLUniform*[numUniforms];
    uniformsAdded = 0;
    this->numUniforms = numUniforms;
}

OGLShaderProgram::~OGLShaderProgram()
{
    glDeleteProgram(pid);
    delete[] uniforms;
}

void OGLShaderProgram::bindShaderProgram()
{
    glUseProgram(pid);
}

void OGLShaderProgram::updateUniformData(string name, void* buffer)
{
    glUseProgram(pid);
    int uniformIndex = uniformNameToLocation[name];
    uniforms[uniformIndex]->setUniformData(buffer);
    glUseProgram(0);
}

unsigned int OGLShaderProgram::CreateShaderProgram(string vs, string fs)
{
    char* vsSource;
    char* fsSource;

    readShaderFile(vs, &vsSource);
    readShaderFile(fs, &fsSource);
    cout << "Vertex Shader: " << vs << " loaded" << endl;
    //cout << vsSource << endl;
    cout << "Fragment Shader: " << fs << " loaded" << endl;
    //cout << fsSource << endl;

    unsigned int vsid = CreateShader(vsSource, GL_VERTEX_SHADER);
    unsigned int fsid = CreateShader(fsSource, GL_FRAGMENT_SHADER);
    delete vsSource;
    delete fsSource;

    unsigned int programid = glCreateProgram();
    glAttachShader(programid, vsid);
    glAttachShader(programid, fsid);
    glLinkProgram(programid);
    glValidateProgram(programid);

    glDeleteShader(vsid);
    glDeleteShader(fsid);

    return programid;
}

unsigned int OGLShaderProgram::CreateShader(char* source, unsigned int type)
{
    unsigned int shaderid = glCreateShader(type);
    glShaderSource(shaderid, 1, &source, nullptr);
    glCompileShader(shaderid);

    int compileResult;
    glGetShaderiv(shaderid, GL_COMPILE_STATUS, &compileResult);
    if (!compileResult) {
        cout << "compile failed" << endl;
        int messagelength;
        glGetShaderiv(shaderid, GL_INFO_LOG_LENGTH, &messagelength);
        char* message = new char[messagelength];
        glGetShaderInfoLog(shaderid, messagelength, &messagelength, message);
        cout << message << endl;
        return 0;
    }
    return shaderid;
}

void OGLShaderProgram::readShaderFile(string fname, char** contents)
{
    std::ifstream fin(fname);
    if (!fin) {
        applicationErrorCallback("No such shader file.");
    }
    fin.seekg(0, ios::end);
    int length = fin.tellg();
    fin.seekg(0, ios::beg);
    *contents = new char[length];
    for (int i = 0; i < length; i++) {
        (*contents)[i] = '\0';
    }
    fin.read(*contents, length);
    fin.close();
}