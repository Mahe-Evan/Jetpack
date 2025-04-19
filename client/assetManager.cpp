/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Asset manager implementation
*/

#include "assetManager.hpp"

#include <iostream>

AssetManager::AssetManager() {}

AssetManager::~AssetManager() {}

bool AssetManager::LoadAssets() {
  bool success = true;

  success &= LoadTexture("background", "assets/background.png");
  success &= LoadTexture("player_normal", "assets/player_normal.png");
  success &= LoadTexture("player_flying", "assets/player_normal.png");
  success &= LoadTexture("coin", "assets/coin.png");
  success &= LoadTexture("electric", "assets/electric.png");

  success &= LoadSound("jetpack", "assets/jetpack.ogg");
  success &= LoadSound("coin_pickup", "assets/coin_pickup_1.wav");
  success &= LoadSound("zapper", "assets/dud_zapper_pop.wav");

  success &= LoadFont("assets/font.ttf");

  return success;
}

bool AssetManager::LoadTexture(const std::string& name,
                               const std::string& filename) {
  sf::Texture texture;
  if (!texture.loadFromFile(filename)) {
    std::cerr << "Failed to load texture: " << filename << std::endl;
    return false;
  }

  textures_[name] = texture;
  return true;
}

bool AssetManager::LoadSound(const std::string& name,
                             const std::string& filename) {
  sf::SoundBuffer buffer;
  if (!buffer.loadFromFile(filename)) {
    std::cerr << "Failed to load sound: " << filename << std::endl;
    return false;
  }

  sound_buffers_[name] = buffer;
  sounds_[name].setBuffer(sound_buffers_[name]);
  return true;
}

bool AssetManager::LoadFont(const std::string& filename) {
  if (!font_.loadFromFile(filename)) {
    std::cerr << "Failed to load font: " << filename << std::endl;
    return false;
  }
  return true;
}

sf::Sprite AssetManager::GetBackgroundSprite() const {
  sf::Sprite sprite;
  auto it = textures_.find("background");
  if (it != textures_.end()) {
    sprite.setTexture(it->second);
  }
  return sprite;
}

sf::Sprite AssetManager::GetPlayerNormalSprite() const {
  sf::Sprite sprite;
  auto it = textures_.find("player_normal");
  if (it != textures_.end()) {
    sprite.setTexture(it->second);
  }
  return sprite;
}

sf::Sprite AssetManager::GetPlayerFlyingSprite() const {
  sf::Sprite sprite;
  auto it = textures_.find("player_flying");
  if (it != textures_.end()) {
    sprite.setTexture(it->second);
  }
  return sprite;
}

sf::Sprite AssetManager::GetCoinSprite() const {
  sf::Sprite sprite;
  auto it = textures_.find("coin");
  if (it != textures_.end()) {
    sprite.setTexture(it->second);
  }
  return sprite;
}

sf::Sprite AssetManager::GetElectricSprite() const {
  sf::Sprite sprite;
  auto it = textures_.find("electric");
  if (it != textures_.end()) {
    sprite.setTexture(it->second);
  }
  return sprite;
}

const sf::Font& AssetManager::GetFont() const { return font_; }

void AssetManager::PlayJetpackSound() {
  auto it = sounds_.find("jetpack");
  if (it != sounds_.end()) {
    it->second.play();
  }
}

void AssetManager::PlayCoinPickupSound() {
  auto it = sounds_.find("coin_pickup");
  if (it != sounds_.end()) {
    it->second.play();
  }
}

void AssetManager::PlayZapperSound() {
  auto it = sounds_.find("zapper");
  if (it != sounds_.end()) {
    it->second.play();
  }
}
