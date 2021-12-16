#include "BaseObject.h"
#include "Engine.h"

BaseObject bases[BASEAMOUNT_MAX];

void BaseObject::spawn(int inPosX, int inPosY, int teamId)
{
	posX = inPosX;
	posY = inPosY;

	baseTeamId = teamId;
}

void BaseObject::draw(int setPosX, int setPosY, int teamId)
{
	engPrint("Spawned a base for team %d", teamId);

#if CLIENT
	engSetColor(255,255,255, 255);
	engFillRect(setPosX, setPosY, 10, 10);
#endif
}