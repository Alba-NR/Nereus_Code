
#include "textures.h"
#include "../utils/image_io.h"

// ------------------------------------------
// --- Texture2D class ---

// Create new texture OpenGL object by loading the image data
Texture2D::Texture2D(string filename)
{
    // load image from file
    unsigned char *data = ImageIO::loadImage(filename, m_width, m_height, m_num_channels);

    // generate OpenGL texture object
    glGenTextures(1, &m_id);

    // set wrapping & filtering parameters
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // bind data to texture & generate mipmap
    bool isRGBA = (m_num_channels == 4);
    glTexImage2D(GL_TEXTURE_2D, 0, (isRGBA ? GL_SRGB_ALPHA : GL_SRGB), m_width, 
                m_height, 0, (isRGBA ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0); 

    // delete img data
    stbi_image_free(data);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_id);
}

GLuint Texture2D::getHandle() const
{
    return m_id;
}


// ------------------------------------------
// --- CubeMapTexture class ---

// Create new cubemap texture OpenGL object, by loading the 6 images for the cubemap faces
// (order of faces in array is: right, left, top, bottom, back, front)
CubeMapTexture::CubeMapTexture(const string filenames[6])
{
    // generate OpenGL cubemap texture object
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

    // for each face, load images & bind data to that cubamap face face 
    for (int i = 0; i < 6; i++)
    {
        // load image from file
        int width, height, num_channels;
        unsigned char *data = ImageIO::loadImage(filenames[i], width, height, num_channels);

        // bind data to appropriate cubemap texture object's face
        bool isRGBA = (num_channels == 4);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, (isRGBA ? GL_SRGB_ALPHA : GL_SRGB), width, height, 
            0, (isRGBA ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);

        // delete img data
        stbi_image_free(data);
    }

    // set wrapping & filtering parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    // unbind texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubeMapTexture::~CubeMapTexture()
{
    glDeleteTextures(1, &m_id);
}

GLuint CubeMapTexture::getHandle() const
{
    return m_id;
}
