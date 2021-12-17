#include "Player.h"
#include "Client.h"
#include "Engine.h"
#include "MessageType.h"
#include "Network.h"
#include <cmath>
#include "Projectile.h"
#include "BaseObject.h"

#define clamp(a, min, max) (a < min ? min : (a > max ? max : a))

Player players[PLAYER_MAX];
#if CLIENT
int possessedPlayerId = -1;
#endif

void Player::netReceivePosition(float newX, float newY)
{
	newX = clamp(newX, 0.f + playerRadius, 800.f - playerRadius);
	errorX = newX - x;
}

void Player::spawn(int id, int spawnX, int spawnY)
{
	this->id = id;
	alive = true;
	x = (float) spawnX;
	y = id % 2 == 0 ? 50 : 550;
	this->teamId = id % 2 == 0;

	if (teamId == 0)
		engPrint("%d joined team GREEN", this->name);
	else
		engPrint("User %d joined team RED", this->name);
}

void Player::destroy()
{
	alive = false;
}

bool Player::hasControl()
{
#if SERVER
	return false;
#else
	return id == possessedPlayerId;
#endif
}

void Player::update()
{
#if CLIENT
	if(hasControl())
	{
		if(!gameHasStarted)
		CreateHelpText();

		const char* teamMessage = teamId == 0 ? "YOU'RE ON THE GREEN TEAM" : "YOU'RE ON THE RED TEAM";
		const int teamMessagePosY = teamId == 0 ? 450 : 100;
		teamId == 0 ? engSetColor(0,255,0,255) : engSetColor(255,0,0,255);
		engText(325, teamMessagePosY, teamMessage);

		int frameInputX = 0;

		sprintMultiplier = engKeyDown(Key::Space) ? 5 : 1;

		if (engKeyDown(Key::Left)) frameInputX -= 1 * sprintMultiplier;
		if (engKeyDown(Key::Right)) frameInputX += 1 * sprintMultiplier;


		if (frameInputX != inputX)
		{
			NetMessage msg;
			msg.write<MessageType>(MessageType::PlayerInput);
			msg.write<int>(id);
			msg.write<float>(x);
			msg.write<float>(y);
			msg.write<char>(frameInputX);

			clientSend(msg);
			msg.free();
		}

		if (engKeyDown(Key::C))
		{
			NetMessage fireMsg;
			fireMsg.write<MessageType>(MessageType::PlayerRequestFire);
			clientSend(fireMsg);
			fireMsg.free();
		}

		inputX = frameInputX;
	}
#endif

	
	if(!hasControl())
	{
		float errorDeltaX = errorX * playerErrorCorrectionStrength * engDeltaTime();
		x += errorDeltaX;
		errorX -= errorDeltaX;
	}

	x += inputX * playerSpeed * engDeltaTime();
	x = clamp(x, 0.f + playerRadius, 800.f - playerRadius);
}

void Player::draw()
{				  //TEAMMATES GREEN			//TEAMMATES RED
	teamId == 0 ? engSetColor(0xADDEBEEF) : engSetColor(0xDEADBEEF); 

#if CLIENT
	if (hasControl()) //PLAYER GREEN			//PLAYER RED
		teamId == 0 ? engSetColor(0,255,0,255) : engSetColor(255,0,0,255);

#endif
	engFillRect((int)x - playerRadius, (int)y - playerRadius, 32, 32);
	engText((int)x - playerRadius, (int)y - playerRadius - 16, name);
}

void Player::CreateHelpText()
{
	engSetColor(255, 255, 255, 255);
	engText(300, 250, "Move to the sides and press 'C' to fire");
	engText(325, 270, "Hold 'SPACE' to move faster");
	engText(335, 285, "Last team standing win");
}
