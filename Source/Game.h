#pragma once
#include <Engine/OGLGame.h>
#include <string>

#include "Components/GameObject.h"
#include "Utility/Rect.h"
#include "Utility/Vector2.h"

/**
 *  An OpenGL Game based on ASGE.
 */
class SpaceInvadersGame : public ASGE::OGLGame
{
 public:
  SpaceInvadersGame();
  ~SpaceInvadersGame();
  virtual bool init() override;

 private:
  void keyHandler(const ASGE::SharedEventData data);
  void clickHandler(const ASGE::SharedEventData data);
  void setupResolution();
  void alienMovement(const ASGE::GameTime& game_time);
  void shipMovement(const ASGE::GameTime& game_time);
  void laserMovement(const ASGE::GameTime& game_time);

  void update(const ASGE::GameTime&) override;
  void render(const ASGE::GameTime&) override;

  int key_callback_id = -1;   /**< Key Input Callback ID. */
  int mouse_callback_id = -1; /**< Mouse Input Callback ID. */

  // Add your GameObjects

  GameObject ship;
  GameObject aliens[7];
  GameObject ship_laser[3];
  GameObject velocity;

  vector2 alien_sine[7] = { vector2(0, 0), vector2(0, 0), vector2(0, 0),
                            vector2(0, 0), vector2(0, 0), vector2(0, 0),
                            vector2(0, 0) };

  bool in_menu = true;
  bool playing = false;
  bool ship_left = false;
  bool ship_right = false;
  bool game_lose = false;
  bool game_won = false;
  bool fired = false;
  bool alien_left = false;
  bool movement = false;

  int score = 0;
  int shots_max = 3;
  int shots_remaining = 3;
  int aliens_init = 7;
  int aliens_remaining = 7;
  int alien_movement = 0;
};