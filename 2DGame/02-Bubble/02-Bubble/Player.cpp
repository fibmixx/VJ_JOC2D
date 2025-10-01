#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, STAND_UP, STAND_DOWN, MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN,
	PUNCH_LEFT, PUNCH_RIGHT, PUNCH_UP, PUNCH_DOWN
};


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/snake_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//									MIDA PLAYER			MIDA SPRITES
	sprite = Sprite::createSprite(glm::ivec2(72, 136), glm::vec2(1.f/15.f, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(1.f / 15.f, 0.5f));

	sprite->setAnimationSpeed(STAND_UP, 8);
	sprite->addKeyframe(STAND_UP, glm::vec2(1.f / 15.f, 0.f));

	sprite->setAnimationSpeed(STAND_DOWN, 8);
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(2.f / 15.f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(4.f / 15.f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(3.f / 15.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(5.f / 15.f, 0.5f));

	sprite->setAnimationSpeed(MOVE_UP, 8);
	sprite->addKeyframe(MOVE_UP, glm::vec2(3.f / 15.f, 0.f));
	sprite->addKeyframe(MOVE_UP, glm::vec2(5.f / 15.f, 0.f));

	sprite->setAnimationSpeed(MOVE_DOWN, 8);
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(2.f / 15.f, 0.f));
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(4.f / 15.f, 0.f));

	sprite->setAnimationSpeed(PUNCH_LEFT, 8);
	sprite->addKeyframe(PUNCH_LEFT, glm::vec2(12.f, 0.5f));

	sprite->setAnimationSpeed(PUNCH_RIGHT, 8);
	sprite->addKeyframe(PUNCH_RIGHT, glm::vec2(13.f / 15.f, 0.5f));

	sprite->setAnimationSpeed(PUNCH_UP, 8);
	sprite->addKeyframe(PUNCH_UP, glm::vec2(13.f / 15.f, 0.f));

	sprite->setAnimationSpeed(PUNCH_DOWN, 8);
	sprite->addKeyframe(PUNCH_DOWN, glm::vec2(12.f / 15.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	// Moviment, només 1 direcció a la vegada
	if (Game::instance().getKey(GLFW_KEY_UP) || Game::instance().getKey(GLFW_KEY_W))
	{
		if (sprite->animation() != MOVE_UP)
			sprite->changeAnimation(MOVE_UP);
		posPlayer.y -= 4;
		if (map->collisionMoveUp(posPlayer, glm::ivec2(64, 64), &posPlayer.y))
		{
			sprite->changeAnimation(STAND_UP);
			posPlayer.y += 4;
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_DOWN) || Game::instance().getKey(GLFW_KEY_S))
	{
		if (sprite->animation() != MOVE_DOWN)
			sprite->changeAnimation(MOVE_DOWN);
		posPlayer.y += 4;
		if (map->collisionMoveDown(posPlayer, glm::ivec2(64, 64), &posPlayer.y))
		{
			sprite->changeAnimation(STAND_DOWN);
			posPlayer.y -= 4;
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_LEFT) || Game::instance().getKey(GLFW_KEY_A))
	{
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 4;
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(64, 64)))
		{
			posPlayer.x += 4;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_RIGHT) || Game::instance().getKey(GLFW_KEY_D))
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 4;
		if (map->collisionMoveRight(posPlayer, glm::ivec2(64, 64)))
		{
			posPlayer.x -= 4;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	// No va
	else if (Game::instance().getKey(GLFW_KEY_Z))
	{
		if (sprite->animation() == MOVE_UP || sprite->animation() == STAND_UP)
			sprite->changeAnimation(PUNCH_UP);
		else if (sprite->animation() == MOVE_DOWN || sprite->animation() == STAND_DOWN)
			sprite->changeAnimation(PUNCH_DOWN);
		else if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT)
			sprite->changeAnimation(PUNCH_LEFT);
		else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT)
			sprite->changeAnimation(PUNCH_RIGHT);
	}
	else
	{
		if (sprite->animation() == MOVE_UP)
			sprite->changeAnimation(STAND_UP);
		else if (sprite->animation() == MOVE_DOWN)
			sprite->changeAnimation(STAND_DOWN);
		else if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}
