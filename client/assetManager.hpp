/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Asset manager for client
*/

#ifndef ASSET_MANAGER_H_
#define ASSET_MANAGER_H_

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class AssetManager {
 public:
  AssetManager();
  ~AssetManager();

  bool LoadAssets();

  sf::Sprite GetBackgroundSprite() const;
  sf::Sprite GetPlayerNormalSprite() const;
  sf::Sprite GetPlayerFlyingSprite() const;
  sf::Sprite GetCoinSprite() const;
  sf::Sprite GetElectricSprite() const;
  const sf::Font& GetFont() const;

  void PlayJetpackSound();
  void PlayCoinPickupSound();
  void PlayZapperSound();

 private:
  std::map<std::string, sf::Texture> textures_;
  std::map<std::string, sf::SoundBuffer> sound_buffers_;
  std::map<std::string, sf::Sound> sounds_;
  sf::Font font_;

  bool LoadTexture(const std::string& name, const std::string& filename);
  bool LoadSound(const std::string& name, const std::string& filename);
  bool LoadFont(const std::string& filename);
};

#endif  // ASSET_MANAGER_H_
