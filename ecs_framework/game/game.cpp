#include <game\game.h>

std::unordered_map<std::string, ECS::System*> Game::systems_ = {};
bool Game::running_ = true;
bool Game::pause_ = false;