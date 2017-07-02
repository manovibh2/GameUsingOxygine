#pragma once

const float SPEED = 0.04;
const int ANIMFACTOR = 40;
const int LIFE = 40;

DECLARE_SMART(Zombie, spZombie);
class Zombie :	public Sprite
{
private://Members
	int life;
	spTweenQueue tweenQueue;
	
	float speed;
	Vector2 mDestPos;
	Resources& mGameResources;
	
private://functions
	void SetSpeed();
	void SetAnimation();

public:
	//Zombie();
	Zombie(int lane, Vector2 srcPos, Vector2 destPos, Resources& gameResources);
	~Zombie(void);

	int BulletHit();
};

