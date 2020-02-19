#include "GameObject.h"
#include <Engine/Renderer.h>

GameObject::~GameObject()
{
  free();
}

bool GameObject::addSpriteComponent(ASGE::Renderer* renderer,
                                    const std::string& texture_file_name)
{
  free();

  sprite_component = new SpriteComponent();
  if (sprite_component->loadSprite(renderer, texture_file_name))
  {
    return true;
  }

  free();
  return false;
}

void GameObject::free()
{
  delete sprite_component;
  sprite_component = nullptr;
}

SpriteComponent* GameObject::spriteComponent()
{
  return sprite_component;
}

void GameObject::setx(float x)
{
  x_pos = x;
}

float GameObject::getx()
{
  return x_pos;
}

vector2* GameObject::getVector()
{
  return direction;
}

void GameObject::setVector(float x_, float y_)
{
  direction->x = x_;
  direction->y = y_;
}
