#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

// Texture wrapper class
class Texture
{
public:
    Texture();

    Texture(SDL_Renderer*);

    Texture(SDL_Renderer*, TTF_Font*);

    ~Texture();

    // Loads image at specified path
    bool loadFromFile(const std::string & path);

    // Creates image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Color backgroundColour);

    // Deallocates texture
    void free();

    // Renders texture at given point
    void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
    // The actual hardware texture
    SDL_Texture* mTexture;
    SDL_Renderer* mRenderer;
    TTF_Font* mFont;

    // Image dimensions
    int mWidth;
    int mHeight;
};

#endif