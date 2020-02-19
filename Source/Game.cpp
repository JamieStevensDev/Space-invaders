#include <string>

#include "Game.h"
#include "Utility/Rect.h"
#include "Utility/Vector2.h"
#include "math.h"
#include <Engine/DebugPrinter.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Sprite.h>

/**
 *   @brief   Default Constructor.
 *   @details Consider setting the game's width and height
 *            and even seeding the random number generator.
 */
SpaceInvadersGame::SpaceInvadersGame()
{
  game_name = "Space Invaders: Gotta Pwn Them All";
}

/**
 *   @brief   Destructor.
 *   @details Remove any non-managed memory and callbacks.
 */
SpaceInvadersGame::~SpaceInvadersGame()
{
  this->inputs->unregisterCallback(static_cast<unsigned int>(key_callback_id));
  this->inputs->unregisterCallback(
    static_cast<unsigned int>(mouse_callback_id));
}

/**
 *   @brief   Initialises the game.
 *   @details The game window is created and all assets required to
 *            run the game are loaded. The keyHandler and clickHandler
 *            callback should also be set in the initialise function.
 *   @return  True if the game initialised correctly.
 */
bool SpaceInvadersGame::init()
{
  setupResolution();
  if (!initAPI())
  {
    return false;
  }

  toggleFPS();
  renderer->setClearColour(ASGE::COLOURS::BLACK);
  renderer->setWindowTitle("Space Invaders!");

  // init game objects within a function to declutter (pass path, xpos, ypos)

  // initate aliens within 2 for loops.

  float last_position = 0;
  float row = 100;

  for (int i = 0; i < aliens_init; ++i)
  {
    if (aliens[i].addSpriteComponent(renderer.get(),
                                     "data/Textures/shipBlue.png"))
    {
      ASGE::Sprite* alienSprite = aliens[i].spriteComponent()->getSprite();
      alienSprite->xPos(0 + last_position);
      alienSprite->height(70);
      alienSprite->width(70);
      last_position = last_position + alienSprite->width();
      alienSprite->yPos(row);
      aliens[i].visibility = true;
    }
  }

  if (ship.addSpriteComponent(renderer.get(),
                              "data/Textures/playerShip1_red.png"))
  {
    ASGE::Sprite* shipSprite = ship.spriteComponent()->getSprite();
    shipSprite->height(70);
    shipSprite->width(70);
    shipSprite->xPos((640 / 2.f) - (shipSprite->width() / 2.f));
    shipSprite->yPos(700);
  }

  for (int i = 0; i < shots_max; ++i)
  {
    if (ship_laser[i].addSpriteComponent(renderer.get(),
                                         "data/Textures/laserRed01.png"))
    {
      ASGE::Sprite* laserSprite = ship_laser[i].spriteComponent()->getSprite();
      laserSprite->xPos(ship.spriteComponent()->getSprite()->xPos());
      laserSprite->yPos(ship.spriteComponent()->getSprite()->yPos());

      // init game objects within a function to declutter (pass path, xpos,
      // ypos)

      // initate aliens within 2 for loops.

      float last_position = 0;
      float row = 100;

      for (int i = 0; i < aliens_init; ++i)
      {
        if (aliens[i].addSpriteComponent(renderer.get(),
                                         "data/Textures/shipBlue.png"))
        {
          ASGE::Sprite* alienSprite = aliens[i].spriteComponent()->getSprite();
          alienSprite->xPos(0 + last_position);
          alienSprite->height(70);
          alienSprite->width(70);
          last_position = last_position + alienSprite->width();
          alienSprite->yPos(row);
          aliens[i].visibility = true;
        }
      }

      if (ship.addSpriteComponent(renderer.get(),
                                  "data/Textures/playerShip1_red.png"))
      {
        ASGE::Sprite* shipSprite = ship.spriteComponent()->getSprite();
        shipSprite->height(70);
        shipSprite->width(70);
        shipSprite->xPos((640 / 2.f) - (shipSprite->width() / 2.f));
        shipSprite->yPos(700);
      }

      for (int i = 0; i < shots_max; ++i)
      {
        if (ship_laser[i].addSpriteComponent(renderer.get(),
                                             "data/Textures/laserRed01.png"))
        {
          ASGE::Sprite* laserSprite =
            ship_laser[i].spriteComponent()->getSprite();
          laserSprite->xPos(float(ship.spriteComponent()->getSprite()->xPos()));
          laserSprite->yPos(float(ship.spriteComponent()->getSprite()->yPos()));

          ship_laser[i].visibility = false;
        }
      }
      // input handling functions
      inputs->use_threads = false;

      key_callback_id = inputs->addCallbackFnc(
        ASGE::E_KEY, &SpaceInvadersGame::keyHandler, this);

      mouse_callback_id = inputs->addCallbackFnc(
        ASGE::E_MOUSE_CLICK, &SpaceInvadersGame::clickHandler, this);

      return true;
    }
  }
}
/**
 *   @brief   Sets the game window resolution
 *   @details This function is designed to create the window size, any
 *            aspect ratio scaling factors and safe zones to ensure the
 *            game frames when resolutions are changed in size.
 *   @return  void
 */
void SpaceInvadersGame::setupResolution()
{
  // how will you calculate the game's resolution?
  // will it scale correctly in full screen? what AR will you use?
  // how will the game be framed in native 16:9 resolutions?
  // here are some arbitrary values for you to adjust as you see fit
  // https://www.gamasutra.com/blogs/KenanBolukbasi/20171002/306822/Scaling_and_MultiResolution_in_2D_Games.php
  game_width = 640;
  game_height = 920;
}

/**
 *   @brief   Processes any key inputs
 *   @details This function is added as a callback to handle the game's
 *            keyboard input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as
 *            you see fit.
 *   @param   data The event data relating to key input.
 *   @see     KeyEvent
 *   @return  void
 */
void SpaceInvadersGame::keyHandler(const ASGE::SharedEventData data)
{
  auto key = static_cast<const ASGE::KeyEvent*>(data.get());

  if (key->key == ASGE::KEYS::KEY_ESCAPE)
  {
    signalExit();
  }

  if (in_menu && key->key == ASGE::KEYS::KEY_ENTER)
  {
    in_menu = false;
    movement = true;
  }

  if (key->key == ASGE::KEYS::KEY_A && key->action == ASGE::KEYS::KEY_PRESSED)
  {
    ship_left = true;
  }
  if (key->key == ASGE::KEYS::KEY_A && key->action == ASGE::KEYS::KEY_RELEASED)
  {
    ship_left = false;
  }
  if (key->key == ASGE::KEYS::KEY_D && key->action == ASGE::KEYS::KEY_PRESSED)
  {
    ship_right = true;
  }
  if (key->key == ASGE::KEYS::KEY_D && key->action == ASGE::KEYS::KEY_RELEASED)
  {
    ship_right = false;
  }

  if (key->key == ASGE::KEYS::KEY_SPACE &&
      key->action == ASGE::KEYS::KEY_PRESSED)
  {
    fired = true;
  }
  if (key->key == ASGE::KEYS::KEY_1 && key->action == ASGE::KEYS::KEY_PRESSED)
  {
    movement = false;
    alien_movement = 1;
    playing = true;
  }
  if (key->key == ASGE::KEYS::KEY_2 && key->action == ASGE::KEYS::KEY_PRESSED)
  {
    movement = false;
    alien_movement = 2;
    playing = true;
  }
  if (key->key == ASGE::KEYS::KEY_3 && key->action == ASGE::KEYS::KEY_PRESSED)
  {
    movement = false;
    alien_movement = 3;
    playing = true;
  }
  if (key->key == ASGE::KEYS::KEY_4 && key->action == ASGE::KEYS::KEY_PRESSED)
  {
    movement = false;
    alien_movement = 4;
    playing = true;

    if (key->key == ASGE::KEYS::KEY_SPACE &&
        key->action == ASGE::KEYS::KEY_PRESSED)
    {
      fired = true;
    }
    if (key->key == ASGE::KEYS::KEY_1 && key->action == ASGE::KEYS::KEY_PRESSED)
    {
      movement = false;
      playing = true;
    }
    if (key->key == ASGE::KEYS::KEY_2 && key->action == ASGE::KEYS::KEY_PRESSED)
    {
      movement = false;
      playing = true;
    }
    if (key->key == ASGE::KEYS::KEY_3 && key->action == ASGE::KEYS::KEY_PRESSED)
    {
      movement = false;
      playing = true;
    }
    if (key->key == ASGE::KEYS::KEY_4 && key->action == ASGE::KEYS::KEY_PRESSED)
    {
      movement = false;
      playing = true;
    }
  }
}

/**
 *   @brief   Processes any click inputs
 *   @details This function is added as a callback to handle the game's
 *            mouse button input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as you
 *            see fit.
 *   @param   data The event data relating to key input.
 *   @see     ClickEvent
 *   @return  void
 */
void SpaceInvadersGame::clickHandler(const ASGE::SharedEventData data)
{
  auto click = static_cast<const ASGE::ClickEvent*>(data.get());

  double x_pos = click->xpos;
  double y_pos = click->ypos;

  ASGE::DebugPrinter{} << "x_pos: " << x_pos << std::endl;
  ASGE::DebugPrinter{} << "y_pos: " << y_pos << std::endl;
}

/**
 *   @brief   Updates the scene
 *   @details Prepares the renderer subsystem before drawing the
 *            current frame. Once the current frame is has finished
 *            the buffers are swapped accordingly and the image shown.
 *   @return  void
 */
void SpaceInvadersGame::update(const ASGE::GameTime& game_time)
{
  // auto dt_sec = game_time.delta.count() / 1000.0;;
  // make sure you use delta time in any movement calculations!

  if (!in_menu)
  {
    alienMovement(game_time);
    shipMovement(game_time);
    laserMovement(game_time);

    for (int i = 0; i < aliens_init; ++i)
    {
      if (aliens[i].spriteComponent()->getBoundingBox().isInside(
            ship.spriteComponent()->getBoundingBox()) &&
          aliens[i].visibility)
      {
        playing = false;
        game_lose = true;
      }
    }

    if (aliens_remaining == 0)
    {
      game_won = true;
      playing = false;
    }

    for (int i = 0; i < aliens_init; ++i)
    {
      if (aliens[i].spriteComponent()->getBoundingBox().isInside(
            ship.spriteComponent()->getBoundingBox()) &&
          aliens[i].visibility)
      {
        playing = false;
        game_lose = true;
      }
    }

    if (aliens_remaining == 0)
    {
      game_won = true;
      playing = false;
    }
  }
}

/**
 *   @brief   Renders the scene
 *   @details Renders all the game objects to the current frame.
 *            Once the current frame is has finished the buffers are
 *            swapped accordingly and the image shown.
 *   @return  void
 */
void SpaceInvadersGame::render(const ASGE::GameTime&)
{
  renderer->setFont(0);

  if (in_menu)
  {
    std::string welcome = "Press enter to start the game";

    renderer->renderText(welcome, 150, 360, 1.0, ASGE::COLOURS::WHITE);
  }
  else if (movement)
  {
    std::string move = "Please select alien movement";
    std::string normal = "1 : Normal";
    std::string gravity = "2 : Gravity";
    std::string quadratic = "3 : Quadratic";
    std::string sine = "4 : Sine";

    renderer->renderText(move, 150, 300, 1.0, ASGE::COLOURS::WHITE);
    renderer->renderText(normal, 150, 350, 1.0, ASGE::COLOURS::WHITE);
    renderer->renderText(gravity, 150, 375, 1.0, ASGE::COLOURS::WHITE);
    renderer->renderText(quadratic, 150, 400, 1.0, ASGE::COLOURS::WHITE);
    renderer->renderText(sine, 150, 425, 1.0, ASGE::COLOURS::WHITE);
  }
  else if (playing)
  {
    std::string score_str = "Score:" + std::to_string(score);
    renderer->renderText(score_str, 500, 75, 1.0, ASGE::COLOURS::WHITE);
    renderer->renderSprite(*ship.spriteComponent()->getSprite());

    for (int i = 0; i < aliens_init; ++i)
    {
      if (aliens[i].visibility)
      {
        renderer->renderSprite(*aliens[i].spriteComponent()->getSprite());
      }
    }
    for (int i = 0; i < shots_max; ++i)
    {
      if (ship_laser[i].visibility)
      {
        renderer->renderSprite(*ship_laser[i].spriteComponent()->getSprite());
        ship_laser[i].visibility = true;
      }
    }
  }
  else if (game_lose)
  {
    renderer->renderText(
      "You Lose", game_width / 3, game_height / 2, 2.0, ASGE::COLOURS::WHITE);
  }
  else if (game_won)
  {
    renderer->renderText("Congratulations",
                         game_width / 3,
                         game_height / 2,
                         2.0,
                         ASGE::COLOURS::WHITE);
  }
}

void SpaceInvadersGame::alienMovement(const ASGE::GameTime& game_time)
{
  for (int i = 0; i < aliens_init; ++i)
  {
    float x_pos = aliens[i].spriteComponent()->getSprite()->xPos();
    float y_pos = aliens[i].spriteComponent()->getSprite()->yPos();
    float multiply = 2.0f;
    auto delta_time = game_time.delta.count();

    if (alien_left)
    {
      velocity.setx(-1);

      if (aliens[i].visibility && x_pos <= 0)
      {
        alien_left = false;

        for (int j = 0; j < aliens_init; ++j)
        {
          y_pos = ((aliens[j].spriteComponent()->getSprite()->yPos()) +
                   (aliens[j].spriteComponent()->getSprite()->height() / 2));
          aliens[j].spriteComponent()->getSprite()->yPos(y_pos);
        }
      }
      else
      {
        x_pos += 60 * velocity.getx() * static_cast<float>(delta_time / 1000.f);
        aliens[i].spriteComponent()->getSprite()->xPos(x_pos);
      }
      if (alien_movement == 2)
      {
        y_pos += static_cast<float>(
          9.8f * aliens[i].spriteComponent()->getSprite()->yPos() * delta_time /
          100000.f);
        aliens[i].spriteComponent()->getSprite()->yPos(y_pos);
      }
      else if (alien_movement == 3)
      {
        float middle_x =
          float(game_width) / 2 -
          (aliens[i].spriteComponent()->getSprite()->width() / 2);
        float offset = x_pos - middle_x;

        x_pos += aliens[i].spriteComponent()->getSprite()->xPos() * multiply;
        y_pos = (-1 * offset / 20 * offset / 20 +
                 aliens[i].spriteComponent()->getSprite()->height() * 4);
        aliens[i].spriteComponent()->getSprite()->yPos(y_pos);
      }
      else if (alien_movement == 4)
      {
        alien_sine[i].normalise();
        y_pos = aliens[i].spriteComponent()->getSprite()->yPos();

        aliens[i].spriteComponent()->getSprite()->xPos(
          alien_sine[i].x -
          (100.f * static_cast<float>(
                     sin(static_cast<long double>(delta_time)) / 150)) +
          x_pos);

        y_pos += static_cast<float>(30 * (delta_time / 1000.f));
        aliens[i].spriteComponent()->getSprite()->yPos(y_pos);
      }
    }

    if (!alien_left)
    {
      velocity.setx(1);

      if (aliens[i].visibility && x_pos >= 640)
      {
        alien_left = true;

        for (int j = 0; j < aliens_init; ++j)
        {
          y_pos = ((aliens[j].spriteComponent()->getSprite()->yPos()) +
                   (aliens[j].spriteComponent()->getSprite()->height() / 2));
          aliens[j].spriteComponent()->getSprite()->yPos(y_pos);
        }
      }
      else
      {
        x_pos += 60 * velocity.getx() *
                 static_cast<float>(game_time.delta.count() / 1000.f);
        aliens[i].spriteComponent()->getSprite()->xPos(x_pos);
      }

      if (alien_movement == 2)
      {
        y_pos += static_cast<float>(
          9.8f * aliens[i].spriteComponent()->getSprite()->yPos() * delta_time /
          100000.f);
        aliens[i].spriteComponent()->getSprite()->yPos(y_pos);
      }
      else if (alien_movement == 3)
      {
        float middle_x =
          float(game_width) / 2 -
          (aliens[i].spriteComponent()->getSprite()->width() / 2);
        float offset = x_pos - middle_x;

        x_pos += aliens[i].spriteComponent()->getSprite()->xPos() * multiply;
        y_pos = (-1 * offset / 20 * offset / 20 +
                 aliens[i].spriteComponent()->getSprite()->height() * 4);
        aliens[i].spriteComponent()->getSprite()->yPos(y_pos);
      }
      else if (alien_movement == 4)
      {
        alien_sine[i].normalise();
        y_pos = aliens[i].spriteComponent()->getSprite()->yPos();

        aliens[i].spriteComponent()->getSprite()->xPos(
          alien_sine[i].x +
          (100.f * static_cast<float>(
                     sin(static_cast<long double>(delta_time)) / 150)) +
          x_pos);

        y_pos += static_cast<float>(30 * (delta_time / 1000.f));
        aliens[i].spriteComponent()->getSprite()->yPos(y_pos);
      }
    }
  }
}

void SpaceInvadersGame::shipMovement(const ASGE::GameTime& game_time)
{
  float x_pos = ship.spriteComponent()->getSprite()->xPos();

  if (ship_left)
  {
    velocity.setx(-1);

    if (x_pos <= 0)
    {
      ship_left = false;
    }
    else
    {
      x_pos += 600 * velocity.getx() *
               static_cast<float>(game_time.delta.count() / 1000.f);
      ship.spriteComponent()->getSprite()->xPos(x_pos);
    }
  }

  if (ship_right)
  {
    velocity.setx(1);

    if (x_pos >= 640 - ship.spriteComponent()->getSprite()->width())
    {
      ship_right = false;
    }
    else
    {
      x_pos += 600 * velocity.getx() *
               static_cast<float>(game_time.delta.count() / 1000.f);
      ship.spriteComponent()->getSprite()->xPos(x_pos);
    }
  }
}

void SpaceInvadersGame::laserMovement(const ASGE::GameTime& game_time)
{
  auto delta_time = game_time.delta.count();

  float y_pos = ship_laser[0].spriteComponent()->getSprite()->yPos();
  float y_pos1 = ship_laser[1].spriteComponent()->getSprite()->yPos();
  float y_pos2 = ship_laser[2].spriteComponent()->getSprite()->yPos();

  double dt_sec = delta_time / 1000.f;

  if (fired && (shots_remaining == 3))
  {
    ship_laser[0].visibility = true;
    shots_remaining--;
    fired = false;
    ship_laser[0].spriteComponent()->getSprite()->xPos(
      ship.spriteComponent()->getSprite()->xPos() + 36);
    ship_laser[0].spriteComponent()->getSprite()->yPos(
      ship.spriteComponent()->getSprite()->yPos() - 46);
  }

  if (fired && (shots_remaining == 2))
  {
    ship_laser[1].visibility = true;
    shots_remaining--;
    fired = false;
    ship_laser[1].spriteComponent()->getSprite()->xPos(
      ship.spriteComponent()->getSprite()->xPos() + 36);
    ship_laser[1].spriteComponent()->getSprite()->yPos(
      ship.spriteComponent()->getSprite()->yPos() - 46);
  }

  if (fired && (shots_remaining == 1))
  {
    ship_laser[2].visibility = true;
    shots_remaining--;
    fired = false;
    ship_laser[2].spriteComponent()->getSprite()->xPos(
      ship.spriteComponent()->getSprite()->xPos() + 36);
    ship_laser[2].spriteComponent()->getSprite()->yPos(
      ship.spriteComponent()->getSprite()->yPos() - 46);
  }

  // reloads shots when all laser sprites shot are gone
  if (shots_remaining == 0 && !ship_laser[0].visibility &&
      !ship_laser[1].visibility && !ship_laser[2].visibility)
  {
    shots_remaining = 3;
  }

  if (ship_laser[0].visibility) // laser movement
  {
    y_pos += float(200 * ship_laser[0].getVector()->y * dt_sec);
    ship_laser[0].spriteComponent()->getSprite()->yPos(y_pos);
  }
  if (ship_laser[1].visibility) // laser movement
  {
    y_pos1 += float(200 * ship_laser[1].getVector()->y * dt_sec);
    ship_laser[1].spriteComponent()->getSprite()->yPos(y_pos1);
  }
  if (ship_laser[2].visibility) // laser movement
  {
    y_pos2 += float(200 * ship_laser[2].getVector()->y * dt_sec);
    ship_laser[2].spriteComponent()->getSprite()->yPos(y_pos2);
  }

  // stops rendering laser sprites that have gone off screen and repositions
  for (int i = 0; i <= 2; i++)
  {
    if (ship_laser[i].spriteComponent()->getSprite()->yPos() < 0)
    {
      fired = false;
      ship_laser[i].visibility = false;
      ship_laser[i].spriteComponent()->getSprite()->xPos(
        ship.spriteComponent()->getSprite()->xPos() + 36);
      ship_laser[i].spriteComponent()->getSprite()->yPos(
        ship.spriteComponent()->getSprite()->yPos() - 46);
    }
  }

  for (int i = 0; i < shots_max; ++i) // collision detection
  {
    for (int j = 0; j < aliens_init; ++j)
    {
      if (ship_laser[i].spriteComponent()->getBoundingBox().isInside(
            aliens[j].spriteComponent()->getBoundingBox()) &&
          aliens[j].visibility)
      {
        ship_laser[i].visibility = false;
        ship_laser[i].spriteComponent()->getSprite()->xPos(
          ship.spriteComponent()->getSprite()->xPos() + 36);
        ship_laser[i].spriteComponent()->getSprite()->yPos(
          ship.spriteComponent()->getSprite()->yPos() - 46);
        aliens[j].visibility = false;
        score += 10;
        aliens_remaining--;
      }
      else if (ship_laser->spriteComponent()->getSprite()->yPos() < 100)
      {
        ship_laser[i].visibility = false;
        ship_laser[i].spriteComponent()->getSprite()->xPos(
          ship.spriteComponent()->getSprite()->xPos() + 36);
        ship_laser[i].spriteComponent()->getSprite()->yPos(
          ship.spriteComponent()->getSprite()->yPos() - 46);
      }
    }
  }
}
