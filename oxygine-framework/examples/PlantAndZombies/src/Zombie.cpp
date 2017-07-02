#include "oxygine-framework.h"
using namespace oxygine;
#include "Zombie.h"

//Zombie::Zombie():life(LIFE), speed(SPEED), mGameResources(0)
//{
//}
Zombie::Zombie(int lane, Vector2 srcPos, Vector2 destPos, Resources& gameResources):life(LIFE), speed(SPEED), mGameResources(gameResources)
{
	SetAnimation();

    Vector2 size = this->getSize();
	srcPos.y = srcPos.y - size.y/2*0.65;
	destPos.y = destPos.y - size.y/2*0.65;
    this->setPosition(srcPos);
	this->setScale(0.75);
	mDestPos = destPos;
	SetSpeed();
	
}

void Zombie::SetAnimation()
{
	int duration = ANIMFACTOR/speed;//ms
    int loops = -1;//infinity loops

    //animation has 8 columns - frames, check 'res.xml'
    ResAnim* animation = mGameResources.getResAnim("anim3");

	//add animation tween to sprite
    //TweenAnim would change animation frames
	this->addTween(Sprite::TweenAnim(animation), duration, loops);
}

void Zombie::SetSpeed()
{
	if(NULL !=  tweenQueue)
	{
		removeTween(tweenQueue);
	}

	//add another tween: TweenQueue
    //TweenQueue is a collection of tweens
    tweenQueue = new TweenQueue();
    //tweenQueue->setDelay(1500);
    //first, move sprite to dest position
	Vector2 srcPos = getPosition();
	int duration = srcPos.distance(mDestPos)/speed;
    tweenQueue->add(Sprite::TweenPosition(mDestPos), duration, 1);
    //then fade it out smoothly
    //tweenQueue->add(Sprite::TweenAlpha(0), 500, 1);

    this->addTween(tweenQueue);

    //and remove sprite from tree when tweenQueue is empty
    //if you don't hold any references to sprite it would be deleted automatically
    tweenQueue->detachWhenDone();
}

Zombie::~Zombie(void)
{
}

int Zombie::BulletHit()
{
	//Reduce life
	{
		life = life-10;		
	}

	if(life < 0)
		life =0;
	if(0 != life)
	{
		speed = speed/2;
		SetSpeed();
		SetAnimation();
	}
	return life;
}