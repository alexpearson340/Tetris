#include "engine/Texture.h"

Texture::Texture()
    : mTexture { nullptr }
    , mRenderer { nullptr }
    , mFont { nullptr }
    , mWidth { 0 }
    , mHeight { 0 }
{
}

Texture::Texture(SDL_Renderer* renderer)
    : mTexture { nullptr }
    , mRenderer { renderer }
    , mFont { nullptr }
    , mWidth { 0 }
    , mHeight { 0 }
{
}

Texture::Texture(SDL_Renderer* renderer, TTF_Font* font)
    : mTexture { nullptr }
    , mRenderer { renderer }
    , mFont { font }
    , mWidth { 0 }
    , mHeight { 0 }
{
}

Texture::~Texture()
{
    // Deallocate
    reset();
}

bool Texture::loadFromFile(const std::string& path)
{
    // The final texture
    SDL_Texture* newTexture { nullptr };

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }

    // Color key image
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

    // Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
    if (newTexture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return false;
    }

    // Get image dimensions
    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

    mTexture.reset(newTexture);
    return true;
}

bool Texture::loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Color backgroundColour)
{   
    // The final texture
    SDL_Texture* newTexture { nullptr };

    // Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Shaded(mFont, textureText.c_str(), textColor, backgroundColour);
    if (textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    // Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
    if (newTexture == NULL)
    {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Get image dimensions
    mWidth = textSurface->w;
    mHeight = textSurface->h;

    // Get rid of old surface
    SDL_FreeSurface(textSurface);

    mTexture.reset(newTexture);
    return true;
}

void Texture::reset()
{
    mTexture.reset();
    mWidth = 0;
    mHeight = 0;
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    // Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    // Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // Render to screen
    SDL_RenderCopyEx(mRenderer, mTexture.get(), clip, &renderQuad, angle, center, flip);
}