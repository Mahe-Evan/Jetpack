/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Asset manager implementation
*/

#include "assetManager.hpp"
#include <iostream>

AssetManager::AssetManager()
{}

AssetManager::~AssetManager()
{}

bool AssetManager::loadAssets()
{
    bool success = true;

    success &= loadTexture("background", "assets/background.png");
    success &=
        loadTexture("player_normal", "assets/player_normal.png");
    success &=
        loadTexture("player_flying", "assets/player_normal.png");
    success &= loadTexture("coin", "assets/coin.png");
    success &= loadTexture("electric", "assets/electric.png");

    success &= loadSound("jetpack", "assets/jetpack.ogg");
    success &= loadSound("coin_pickup", "assets/coin_pickup_1.wav");
    success &= loadSound("zapper", "assets/dud_zapper_pop.wav");

    success &= loadFont("assets/font.ttf");

    return success;
}

bool AssetManager::loadTexture(
    const std::string &name, const std::string &filename)
{
    sf::Texture texture;
    if (!texture.loadFromFile(filename)) {
        std::cerr << "Failed to load texture: " << filename
                  << std::endl;
        return false;
    }

    textures[name] = texture;
    return true;
}

bool AssetManager::loadSound(
    const std::string &name, const std::string &filename)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)) {
        std::cerr << "Failed to load sound: " << filename
                  << std::endl;
        return false;
    }

    soundBuffers[name] = buffer;
    sounds[name].setBuffer(soundBuffers[name]);
    return true;
}

bool AssetManager::loadFont(const std::string &filename)
{
    if (!font.loadFromFile(filename)) {
        std::cerr << "Failed to load font: " << filename << std::endl;
        return false;
    }
    return true;
}

sf::Sprite AssetManager::getBackgroundSprite() const
{
    sf::Sprite sprite;
    auto it = textures.find("background");
    if (it != textures.end()) {
        sprite.setTexture(it->second);
    }
    return sprite;
}

sf::Sprite AssetManager::getPlayerNormalSprite() const
{
    sf::Sprite sprite;
    auto it = textures.find("player_normal");
    if (it != textures.end()) {
        sprite.setTexture(it->second);
    }
    return sprite;
}

sf::Sprite AssetManager::getPlayerFlyingSprite() const
{
    sf::Sprite sprite;
    auto it = textures.find("player_flying");
    if (it != textures.end()) {
        sprite.setTexture(it->second);
    }
    return sprite;
}

sf::Sprite AssetManager::getCoinSprite() const
{
    sf::Sprite sprite;
    auto it = textures.find("coin");
    if (it != textures.end()) {
        sprite.setTexture(it->second);
    }
    return sprite;
}

sf::Sprite AssetManager::getElectricSprite() const
{
    sf::Sprite sprite;
    auto it = textures.find("electric");
    if (it != textures.end()) {
        sprite.setTexture(it->second);
    }
    return sprite;
}

const sf::Font &AssetManager::getFont() const
{
    return font;
}

void AssetManager::playJetpackSound()
{
    auto it = sounds.find("jetpack");
    if (it != sounds.end()) {
        it->second.play();
    }
}

void AssetManager::playCoinPickupSound()
{
    auto it = sounds.find("coin_pickup");
    if (it != sounds.end()) {
        it->second.play();
    }
}

void AssetManager::playZapperSound()
{
    auto it = sounds.find("zapper");
    if (it != sounds.end()) {
        it->second.play();
    }
}
