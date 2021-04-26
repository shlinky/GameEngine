#include "OGLCubeMapTexture.h"

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

//OGLCubeMapTexture::OGLCubeMapTexture(int w)
//{
    /*SceneMeshObject cube = SceneMeshObject(0, 0, 0);
    cube.setMesh("res/models/cube.txt");
    cube.createShader("res/shaders/hdr_conv.vert", "res/shaders/hdr_conv.frag");

    Camera cam = Camera(0, 0, 0, 1, 1);
    cam.setFOV(90.0f);
    OGLFrameBuffer fb;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    for (unsigned int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


    for (unsigned int i = 0; i < 6; i++)
    {
        //renderViews[3 * i], renderViews[(3 * i) + 1], renderViews[(3 * i) + 2]
        cam.setRotation(0, 0);
        fb.resetColorTextures();
        fb.attachColorTextureCM(this, i);
        fb.bind();
        fb.clear();
        cube.render(&cam);
    }
    fb.unbind();*/
//}

void OGLCubeMapTexture::bindTexture(unsigned int textureSlot)
{
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
}
