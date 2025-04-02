/*
** EPITECH PROJECT, 2025
** jetpack_client
** File description:
** Asset manager definition
*/

#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

enum PlayerState {
    PLAYER_NORMAL,
    PLAYER_FLYING
};

class AssetManager {
  public:
    AssetManager();
    ~AssetManager() = default;

    const sf::Texture &getPlayerTexture(PlayerState state) const;
    const sf::Texture &getCoinTexture() const;
    const sf::Texture &getElectricTexture() const;
    const sf::Texture &getBackgroundTexture() const;
    const sf::Font &getFont() const;

  private:
    bool loadAssets();
    bool loadTexture(sf::Texture &texture, const std::string &path);

    std::map<PlayerState, sf::Texture> playerTextures;
    sf::Texture coinTexture;
    sf::Texture electricTexture;
    sf::Texture backgroundTexture;
    sf::Font font;
};

#endif      // ASSET_MANAGER_HPP
