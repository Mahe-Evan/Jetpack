/*
** EPITECH PROJECT, 2025
** jetpack_client
** File description:
** Asset manager implementation
*/

#include "assetManager.hpp"
#include "gameState.hpp"
#include <iostream>

AssetManager::AssetManager()
{
    if (!loadAssets()) {
        std::cerr << "Failed to load assets" << std::endl;
        running = false;
    }
}

bool AssetManager::loadAssets()
{
    sf::Texture normalTexture, flyingTexture;
    if (!loadTexture(normalTexture, "assets/player_normal.png") ||
        !loadTexture(flyingTexture, "assets/player_normal.png")) {
        return false;
    }

    playerTextures[PLAYER_NORMAL] = normalTexture;
    playerTextures[PLAYER_FLYING] = flyingTexture;

    if (!loadTexture(coinTexture, "assets/coin.png") ||
        !loadTexture(electricTexture, "assets/electric.png") ||
        !loadTexture(backgroundTexture, "assets/background.png")) {
        return false;
    }

    if (!font.loadFromFile("assets/font.ttf")) {
        if (!font.loadFromFile(
                "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            std::cerr << "Warning: Failed to load font, text may not "
                         "display"
                      << std::endl;
        }
    }

    return true;
}

bool AssetManager::loadTexture(
    sf::Texture &texture, const std::string &path)
{
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return false;
    }
    return true;
}

const sf::Texture &AssetManager::getPlayerTexture(
    PlayerState state) const
{
    auto it = playerTextures.find(state);
    if (it != playerTextures.end()) {
        return it->second;
    }

    return playerTextures.at(PLAYER_NORMAL);
}

const sf::Texture &AssetManager::getCoinTexture() const
{
    return coinTexture;
}

const sf::Texture &AssetManager::getElectricTexture() const
{
    return electricTexture;
}

const sf::Texture &AssetManager::getBackgroundTexture() const
{
    return backgroundTexture;
}

const sf::Font &AssetManager::getFont() const
{
    return font;
}
