#include "oxygine-framework.h"
using namespace oxygine;

#include "Plant.h"

#include "GenericEvent.h"
#include <map>
#include "GameActor.h"


	Plant::Plant(int lane, Vector2 srcPos, Resources& gameResources):mLane(lane), mResources(gameResources),mLastUpdate(0)
	{
		setClock( new Clock);
		int duration = 600;//ms
		int loops = -1;//infinity loops

		//animation has 8 columns - frames, check 'res.xml'
		ResAnim* animation = mResources.getResAnim("peeshooter2");

		//add animation tween to sprite
		//TweenAnim would change animation frames
		addTween(Sprite::TweenAnim(animation), duration, loops);

			
		//set sprite initial position
		Vector2 peeShooterSize = getSize();
		srcPos.y = srcPos.y - peeShooterSize.y/2*0.25f;
		srcPos.x = srcPos.x - peeShooterSize.x/2*0.25f;
		setPosition(srcPos);
		setScale(0.25f);
	}


	void Plant::doUpdate(const UpdateState& us)
	{		
		if (mLastUpdate == 0  || (mLastUpdate + 4000) < us.time)
        {
			spMainActor pActor = GetMainActor();
			std::list<spSprite> zombieList = pActor->GetActiveElements(pActor->zombies, mLane);
			if(zombieList.size() == 0)
				return;

			mLastUpdate = us.time;			
			Vector2 position = getPosition();
			ShootPee(position, mLane);
		}
	}

	void Plant::ShootPee(Vector2 pos, int lane)
	{
		float peeSpeed = 0.2;
		spSprite sprite = new Sprite();
		//spMainActor pActor = GetMainActor();
		spActor pActor = (getStage()->getFirstChild());
        pActor->addChild(sprite);

		
		//AddActiveElement(pees, lane, sprite);
		GenericEvent ev(GenericEvent::GEvent::BULLET_ADD);
		ev.lane = mLane;
		ev.sprite = sprite;
		ev.target = ev.currentTarget = pActor;

		dispatchEvent(&ev);
		
              
        ResAnim* animation = mResources.getResAnim("pee");

        //add animation tween to sprite
        //TweenAnim would change animation frames
        sprite->addTween(Sprite::TweenAnim(animation), 1000, -1);

        Vector2 srcPos(pos);
		Vector2 destPos(srcPos);
		destPos.x = getStage()->getSize().x;
		sprite->setPosition(srcPos);
       

		//add another tween: TweenQueue
        //TweenQueue is a collection of tweens
        spTweenQueue tweenQueue = new TweenQueue();
        //tweenQueue->setDelay(1500);
        //first, move sprite to dest position
		float dist = srcPos.distance(destPos);
		int duration = dist/peeSpeed;
        tweenQueue->add(Sprite::TweenPosition(destPos), duration, 1);
        //then fade it out smoothly
        //tweenQueue->add(Sprite::TweenAlpha(0), 500, 1);

        sprite->addTween(tweenQueue);

        //and remove sprite from tree when tweenQueue is empty
        //if you don't hold any references to sprite it would be deleted automatically
        tweenQueue->detachWhenDone();
	}


typedef oxygine::intrusive_ptr<Plant> spPlant;