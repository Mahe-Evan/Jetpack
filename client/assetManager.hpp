/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Asset manager for client
*/

#ifndef ASSET_MANAGER_HPP_
#define ASSET_MANAGER_HPP_

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class AssetManager {
  public:
    AssetManager();
    ~AssetManager();

    bool loadAssets();

    sf::Sprite getBackgroundSprite() const;
    sf::Sprite getPlayerNormalSprite() const;
    sf::Sprite getPlayerFlyingSprite() const;
    sf::Sprite getCoinSprite() const;
    sf::Sprite getElectricSprite() const;
    const sf::Font &getFont() const;

    void playJetpackSound();
    void playCoinPickupSound();
    void playZapperSound();

  private:
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::SoundBuffer> soundBuffers;
    std::map<std::string, sf::Sound> sounds;
    sf::Font font;

    bool loadTexture(
        const std::string &name, const std::string &filename);
    bool loadSound(
        const std::string &name, const std::string &filename);
    bool loadFont(const std::string &filename);
};

#endif /* !ASSET_MANAGER_HPP_ */
