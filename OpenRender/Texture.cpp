#include "Texture.h"
#include <SDL2/SDL_image.h>

Texture::Texture()
    :mID(0)
{

}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, mID);
}

bool Texture::Load(const std::string& path)
{
    SDL_Surface* textureData = IMG_Load(path.c_str());
    if (!textureData)
    {
        printf("Couldn't find texture at location: %s\n", path.c_str());
        return false;
    }
    GLsizei width = textureData->w;
    GLsizei height = textureData->h;
    Uint8* pixelData = (Uint8*)textureData->pixels;
    Uint8 depth = textureData->format->BytesPerPixel;
    GLint format = ((depth == 4) ? GL_RGBA : GL_RGB);

    glGenTextures(1, &mID);
    glBindTexture(GL_TEXTURE_2D, mID);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixelData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(textureData);
    return true;
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Unload()
{
    glDeleteTextures(1, &mID);
}
