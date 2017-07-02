#include "oxygine-framework.h"
using namespace oxygine;

#include "Block.h"

#include "Plant.h"

#include "GenericEvent.h"
#include <map>
#include "GameActor.h"

	Block::Block(int lane, Vector2 pos, Vector2 size, Resources& gameResources):mResources(gameResources)
	{
		mLane = lane;
		bClicked = false;
		sprite = new ColorRectSprite;
		sprite->setPosition(pos.x, pos.y);
		sprite->setSize(size.x, size.y);
		sprite->setColor(Color(Color::AliceBlue));
		//sprite->attachTo(getStage()); 
		addChild(sprite);
		//EventCallback callback = MainActor::onSpriteClicked;
		EventCallback callback = CLOSURE(this, &Block::onSpriteClicked);
		sprite->addEventListener(TouchEvent::CLICK, callback );
	}

	void Block::onSpriteClicked(Event* event1)
	{
		printf("clicked to child");
		/*if(false == bClicked)
			return;*/
		bClicked = false;
		//TouchEvent* event2 = safeCast<TouchEvent, Event>(event1);
		TouchEvent* event2 = (TouchEvent*)(event1);
		if(event2 == NULL)
			return;
		Vector2 srcPos(sprite->getPosition());
		srcPos.x += sprite->getSize().x/2;
		srcPos.y += sprite->getSize().y/2;
		//srcPos = getStage()->local2stage(srcPos);
		spPlant peeshooter = new Plant(mLane, srcPos, mResources);
		addChild(peeshooter);
		
		GenericEvent ev(GenericEvent::GEvent::PLANT_ADD);
		ev.lane = mLane;
		ev.sprite = peeshooter;
		ev.target = ev.currentTarget = this->getParent();

		dispatchEvent(&ev);
		
	}
