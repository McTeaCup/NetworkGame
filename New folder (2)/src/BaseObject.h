#pragma once
#define BASEAMOUNT_MAX 5

class BaseObject
{
public:
	int baseTeamId;

	int posX;
	int posY;

	void spawn(int posX, int posY, int teamId);
	void draw(int setPosX, int setPosY, int teamId);
	void destroy();
};

extern BaseObject bases[BASEAMOUNT_MAX];