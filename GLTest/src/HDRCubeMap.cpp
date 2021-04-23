#include "HDRCubeMap.h"

//add real-time construction (render cubemap function in this class or OGLCubemap with all six views, just needs a scene)
//constructor from images (use parent constructor)
HDRCubeMap::HDRCubeMap(string path, int w)
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
    cout << width << ' ' << height << endl;
    for (unsigned int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    float renderViews[18] = { 1, 0, 0,
                            -1, 0, 0,
                             0, 1, 0,
                             0,-1, 0,
                             0, 0, 1,
                             0, 0,-1 };

    for (unsigned int i = 0; i < 6; i++)
    {
        cam.lookAtDir(renderViews[3 * i], renderViews[(3 * i) + 1], renderViews[(3 * i) + 2]);
        fb.resetColorTextures();
        fb.attachColorTextureCM(this, i);
        fb.bind();
        fb.clear();
        cube.render(&cam);
    }
    fb.unbind();

    //int p[2] = { 0 };
    //glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_WIDTH, p);
    //glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_HEIGHT, p + 1);
    //cout << p[0] << ' ' << p[1];
}

HDRCubeMap::HDRCubeMap(int w)
{
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    this->width = w;
    height = w;
    cout << width << ' ' << height << endl;
    for (unsigned int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void HDRCubeMap::save(string fname)
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

       // for (int i = 0; i < 30; i++) {
       //     cout << (int)saveImg[i] << endl;
       // }
        string fnm = fname;
        fnm += (char)(48 + i);
        fnm += ".jpg";
        saveTexture(fnm, width, height, saveImg);
        delete[] saveImg;
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

HDRCubeMap* HDRCubeMap::createIrradianceMap()
{
    SceneMeshObject cube = SceneMeshObject(0, 0, 0);
    cube.setMesh("res/models/cube.txt");
    cube.createShader("res/shaders/hdr_conv.vert", "res/shaders/irradiance.frag");
    cube.getShader()->addTexture(this);

    Camera cam = Camera(0, 0, 0, 1, 1);
    cam.setFOV(90.0f);
    OGLFrameBuffer fb;

    HDRCubeMap* irMap = new HDRCubeMap(width);

    float renderViews[18] = { 1, 0, 0,
                            -1, 0, 0,
                             0, 1, 0,
                             0,-1, 0,
                             0, 0, 1,
                             0, 0,-1 };

    for (unsigned int i = 0; i < 6; i++)
    {
        cam.lookAtDir(renderViews[3 * i], renderViews[(3 * i) + 1], renderViews[(3 * i) + 2]);
        fb.resetColorTextures();
        fb.attachColorTextureCM(irMap, i);
        fb.bind();
        fb.clear();
        cube.render(&cam);
    }
    fb.unbind();

    return (irMap);
}
