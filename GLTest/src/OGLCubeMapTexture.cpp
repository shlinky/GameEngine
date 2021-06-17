#include "OGLCubeMapTexture.h"
#include "SceneMeshObject.h"
#include "OGLFrameBuffer.h"
#include <Windows.h>

OGLCubeMapTexture::OGLCubeMapTexture()
{

}

//create constructors with no params and id param
OGLCubeMapTexture::OGLCubeMapTexture(string* texturePaths)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned char* data;
        loadTexture(texturePaths[i], &width, &height, &data);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
        );
        freeTexture(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    textureId = textureID;
}

//add real-time construction (render cubemap function in this class or OGLCubemap with all six views, just needs a scene)
//constructor from images (use parent constructor)
OGLCubeMapTexture::OGLCubeMapTexture(string path, int w)
{
    float* img;
    unsigned int ht;
    //make this loading of hdr texture an option in OGLImageTexture constructor
    loadTextureHDR(path, &width, &height, &img);

    glGenTextures(1, &ht);
    glBindTexture(GL_TEXTURE_2D, ht);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    freeTexture(img);

    SceneMeshObject cube = SceneMeshObject(0, 0, 0);
    cube.setMesh("res/models/cube.txt");
    cube.createShader("res/shaders/hdr_conv.vert", "res/shaders/hdr_conv.frag");
    OGLImageTexture* hdrt = new OGLImageTexture(ht);
    cube.getShader()->addTexture(hdrt);

    Camera cam = Camera(0, 0, 0, 1, 1);
    cam.setFOV(90.0f);
    OGLFrameBuffer fb;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    this->width = w;
    height = w;
    cout << "her: " << width << ' ' << height << endl;
    for (unsigned int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    float renderViews[30] = { 0,  0, 0, -1, 0,
                            180,  0, 0, -1, 0,
                              0, 90, 0, 0, 1,
                              0,-90, 0, 0, -1,
                              90,0, 0, -1, 0,
                              -90, 0, 0, -1, 0 };


    for (unsigned int i = 0; i < 6; i++)
    {
        cam.setUpDir(renderViews[(5 * i) + 2], renderViews[(5 * i) + 3], renderViews[(5 * i) + 4]);
        cam.setRotation(renderViews[5 * i], renderViews[(5 * i) + 1]);
        fb.resetColorTextures();
        fb.attachColorTextureCM(this, i);
        fb.bind();
        fb.clear();
        cube.render(&cam);
    }
    fb.unbind();

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    //int p[2] = { 0 };
    //glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_WIDTH, p);
    //glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_HEIGHT, p + 1);
    //cout << p[0] << ' ' << p[1];
}

OGLCubeMapTexture::OGLCubeMapTexture(int w, bool mipmap)
{
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    this->width = w;
    height = w;
    cout << width << ' ' << height << endl;
    for (unsigned int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr
        );
    }
    if (mipmap) {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
    else
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void OGLCubeMapTexture::save(string fname)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    for (int i = 0; i < 6; i++) {
        unsigned char* saveImg = new unsigned char[width * height * 3];
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            saveImg);

        string fnm = fname;
        fnm += (char)(48 + i);
        fnm += ".jpg";
        saveTexture(fnm, width, height, saveImg);
        delete[] saveImg;
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

OGLCubeMapTexture* OGLCubeMapTexture::createIrradianceMap(int w)
{
    int irWidth = w;
    if (w == 0) {
        irWidth = width;
    }

    SceneMeshObject cube = SceneMeshObject(0, 0, 0);
    cube.setMesh("res/models/cube.txt");
    cube.createShader("res/shaders/hdr_conv.vert", "res/shaders/irradiance.frag");
    cube.getShader()->addTexture(this);

    Camera cam = Camera(0, 0, 0, 1, 1);
    cam.setFOV(90.0f);
    OGLFrameBuffer fb;

    OGLCubeMapTexture* irMap = new OGLCubeMapTexture(irWidth);

    float renderViews[30] = {  0,  0, 0, -1,  0,
                             180,  0, 0, -1,  0,
                               0, 90, 0,  0,  1,
                               0,-90, 0,  0, -1,
                              90,  0, 0, -1,  0,
                             -90,  0, 0, -1,  0};

    for (unsigned int i = 0; i < 6; i++)
    {
        cam.setUpDir(renderViews[(5 * i) + 2], renderViews[(5 * i) + 3], renderViews[(5 * i) + 4]);
        cam.setRotation(renderViews[5 * i], renderViews[(5 * i) + 1]);
        fb.resetColorTextures();
        fb.attachColorTextureCM(irMap, i);
        fb.bind();
        fb.clear();
        cube.render(&cam);

    }
    fb.unbind();
    return (irMap);
}

OGLCubeMapTexture* OGLCubeMapTexture::createPrefilteredSpec(int w)
{
    int sWidth = w;
    if (w == 0) {
        sWidth = width;
    }
    SceneMeshObject cube = SceneMeshObject(0, 0, 0);
    cube.setMesh("res/models/cube.txt");
    cube.createShader("res/shaders/hdr_conv.vert", "res/shaders/sprefilter.frag", 6);
    cube.getShader()->addTexture(this);
    cube.getShader()->addUniform<OGLUniformFloat>("r");

    Camera cam = Camera(0, 0, 0, 1, 1);
    cam.setFOV(90.0f);
    OGLFrameBuffer fb;

    OGLCubeMapTexture* sMap = new OGLCubeMapTexture(sWidth, true);


    float renderViews[30] = {  0,  0, 0, -1,  0,
                             180,  0, 0, -1,  0,
                               0, 90, 0,  0,  1,
                               0,-90, 0,  0, -1,
                              90,  0, 0, -1,  0,
                             -90,  0, 0, -1,  0 };

    for (int m = 0; m < 5; m++)
    {
        for (unsigned int f = 0; f < 6; f++) {
            cam.setUpDir(renderViews[(5 * f) + 2], renderViews[(5 * f) + 3], renderViews[(5 * f) + 4]);
            cam.setRotation(renderViews[5 * f], renderViews[(5 * f) + 1]);
            fb.resetColorTextures();
            fb.attachColorTextureCM(sMap, f, m);
            fb.bind();
            fb.clear();

            float roughness = ((float)m) / (float)(4);
            cube.getShader()->updateUniformData("r", &roughness);
            cube.render(&cam);
            cout << "pone" << endl;
        }

    }
    fb.unbind();

    return (sMap);

}

void OGLCubeMapTexture::bindTexture(unsigned int textureSlot)
{
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
}
