#include "oxygine-framework.h"
#include <functional>
#include <map>
#include <list>

using namespace oxygine;

#ifdef OXYGINE_SDL
#include "SDL_main.h"
#include "SDL.h"
#endif


#include "GameActor.h"
#include "GenericEvent.h"

#include "Block.h"
#include "Plant.h"
#include "Zombie.h"

MainActor::MainActor(Resources& gameResources):previousUS(0),collisionUS(0),bClicked(false),mResources(gameResources)
    {
		setClock( new Clock());
		createOutline();
		//addPeeShooters();

		
        //create button Sprite
        spSprite button = new Sprite();


        //setup it:
        //set button.png image. Resource 'button' defined in 'res.xml'
        button->setResAnim(mResources.getResAnim("button"));

        //centered button at screen
		Vector2 pos(110+5,5);
        button->setPosition(pos);
		button->setSize(40, 40);

        //register  click handler to button
        EventCallback cb = CLOSURE(this, &MainActor::buttonClicked);
        button->addEventListener(TouchEvent::CLICK, cb);

#ifdef CLOSURE_FUNCTION //if your compiler supports lambda

        button->addEventListener(TouchEvent::CLICK, [](Event * e)->void
        {
            log::messageln("button clicked");
        });

#endif

        //attach button as child to current actor
        addChild(button);
        _button = button;



        //create TextField Actor
        spTextField text = new TextField();
        //attach it as child to button
        text->attachTo(button);
        //centered in button
        text->setPosition(button->getSize() / 2);

        //initialize text style
       // TextStyle style = TextStyle(mResources.getResFont("main")).withColor(Color::White).alignMiddle();
        //text->setStyle(style);
        text->setText("Pee-Shooter");

        _text = text;

		
    }

	void MainActor::bulletCreated(Event* ev)
    {
		GenericEvent* ev1 = dynamic_cast<GenericEvent*>(ev);
		AddActiveElement(pees, ev1->lane, ev1->sprite);
	}

	void MainActor::plantCreated(Event* ev)
    {
		GenericEvent* ev1 = dynamic_cast<GenericEvent*>(ev);
		AddActiveElement(peeShooters, ev1->lane, ev1->sprite);

		//register  click handler to button
		EventCallback bulletAdded = CLOSURE(this, &MainActor::bulletCreated);
		ev1->sprite->addEventListener(GenericEvent::BULLET_ADD, bulletAdded);
	}

	void MainActor::doUpdate(const UpdateState& us)
	{
		if (previousUS == 0  || (previousUS + 4000) < us.time)
        {
			previousUS = us.time;
			runSprite();
			//ShootPees();
		}
		if(collisionUS + 10 < us.time)
		{
			collisionUS = us.time;
			CheckCollision();
		}
	}

	void MainActor::explode(spSprite sprite)
	{
		////hit by rocket
		//_hp--;
		//if (_hp == 0)
		{
			//dead, hide it with alpha tween
			//_dead = true;
			sprite->addTween(Actor::TweenAlpha(0), 300)->detachWhenDone();
		}
	}

	void MainActor::CheckCollision()
	{
		ActiveElements::iterator it;
		for(it = peeShooters.begin();it!=peeShooters.end();it++)
		{

reStartLoop:

			std::list<spSprite> zombieList = GetActiveElements(zombies, it->first);
			if(zombieList.size() == 0)
				continue;
			std::list<spSprite> peeList = GetActiveElements(pees, it->first);
			if(peeList.size() == 0)
				continue;

			
			std::list<spSprite>::iterator itZombie;
			for(itZombie = zombieList.begin(); itZombie != zombieList.end(); itZombie++)
			{
				spZombie zombie = safeSpCast<Zombie>(*itZombie);
				if(NULL == zombie)
					continue;
				Vector2 zombiePos = zombie->getPosition();
				std::list<spSprite>::iterator itPee;
				for(itPee = peeList.begin(); itPee != peeList.end(); itPee++)
				{
					spSprite pee = *itPee;
					Vector2 peePos = pee->getPosition();
					Vector2 d = peePos - zombiePos;
					if (d.length() < 20)
					{
						if(zombie->BulletHit() == 0)
						{
							explode(zombie);
							RemoveActiveElement(zombies, it->first, zombie);
						}
						
						explode(pee);						
						RemoveActiveElement(pees, it->first, pee);
						goto reStartLoop;
					}
				}
			}
		}
	}

	void MainActor::ShootPees()
	{
		ActiveElements::iterator it;
		for(it = peeShooters.begin();it!=peeShooters.end();it++)
		{
			int lane = it->first;
			std::list<spSprite> zombieList = GetActiveElements(zombies, lane);
			if(zombieList.size() == 0)
				continue;
			std::list<spSprite> peeShooterList = it->second;
			std::list<spSprite>::iterator it2;
			for(it2 = peeShooterList.begin();it2!=peeShooterList.end();it2++)
			{
				spSprite peeShooter = *it2;
				Vector2 position = peeShooter->getPosition();
				Vector2 size = peeShooter->getSize();
				ShootPee(position, lane);
			}
		}
	}
	void MainActor::ShootPee(Vector2 pos, int lane)
	{
		float peeSpeed = 0.2;
		spSprite sprite = new Sprite();
        addChild(sprite);
		AddActiveElement(pees, lane, sprite);

              
        ResAnim* animation = mResources.getResAnim("pee");

        //add animation tween to sprite
        //TweenAnim would change animation frames
        sprite->addTween(Sprite::TweenAnim(animation), 1000, -1);

        Vector2 srcPos(pos);
		Vector2 destPos(srcPos);
		destPos.x = getStage()->getSize().x;
		sprite->setPosition(srcPos);
       

		/*Vector2 size = sprite->getSize();
		srcPos.y = srcPos.y - size.y/2*0.65;
		destPos.y = destPos.y - size.y/2*0.65;
        sprite->setPosition(srcPos);*/
		//sprite->setScale(0.75);

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
    void MainActor::buttonClicked(Event* event)
    {
		bClicked = true;

		//Create cursor using bitmap
		/*SDL_Surface *surface = NULL;
		SDL_Cursor *cursor = NULL;
		SDL_bool error = SDL_TRUE;
		surface = SDL_LoadBMP("cursor.bmp");
		if (!surface) {
			return;
		}
		cursor = SDL_CreateColorCursor(surface, 0, 0);
		if (!cursor) {
			return;
		}
		SDL_SetCursor(cursor);*/

        //user clicked to button

        ////animate button by chaning color
        //_button->setColor(Color::White);
        //_button->addTween(Sprite::TweenColor(Color::Green), 500, 1, true);

        ////animate text by scaling
        //_text->setScale(1.0f);
        //_text->addTween(Actor::TweenScale(1.1f), 500, 1, true);

        ////and change text
        //_text->setText("Clicked!");

        ////lets create and run sprite with simple animation
        //runSprite();
    }

	

	//Stage size 960 x 640
	//Left side with position(0, 0) and size(110, 640) for house
	//Right Side with pos(920, 40) and size(40, 600) for Zombie initial pos
	//Top side with pos(110, 0) and size(850, 40) for buttons
	//Field having 5 lanes each lane having 9 blocks to be place in middle with pos(110, 40) and
	//size (90			x 9 = 810	, 120			x 5 = 600	)
	//     (blockwidth	x xDir block, blockHeight	x yDir blocks)
	void MainActor::createOutline()
	{
		int width = 960, height = 640;
		blockWidth = 90, blockHeight = 120;
		int k=1;
		for(int j = 40;j<height;j= j+121)
		{
			std::list<Vector2> positionList;
			std::list<Vector2> centerPositionList;
			for(int i = 110;i<width - 40;i=i+91)
			{
				positionList.push_back(Vector2(i,j));
				centerPositionList.push_back(Vector2(i+blockWidth/2,j+blockHeight/2));
				spSprite b = new Block(k, Vector2(i,j), Vector2(blockWidth, blockHeight), mResources);
				addChild(b);
				//register  click handler to button
				EventCallback plantAdded = CLOSURE(this, &MainActor::plantCreated);
				b->addEventListener(GenericEvent::PLANT_ADD, plantAdded);
			
			}
			blockPositions.insert(BlockPositionPair(k, positionList));
			blockCenterPositions.insert(BlockPositionPair(k, centerPositionList));
			k++;
		
		}
	}
	void MainActor::addPeeShooters()
    {
		BlockPositions::iterator it;
		for(it = blockCenterPositions.begin();it!=blockCenterPositions.end();it++)
		{
			std::list<Vector2> centerPosList = it->second;
			Vector2 srcPos = *(centerPosList.begin());
			spSprite sprite = new Sprite();
			addChild(sprite);
			
			AddActiveElement(peeShooters, it->first, sprite);

			int duration = 600;//ms
			int loops = -1;//infinity loops

			//animation has 8 columns - frames, check 'res.xml'
			ResAnim* animation = mResources.getResAnim("peeshooter2");

			//add animation tween to sprite
			//TweenAnim would change animation frames
			sprite->addTween(Sprite::TweenAnim(animation), duration, loops);

			
			//set sprite initial position
			Vector2 peeShooterSize = sprite->getSize();
			srcPos.y = srcPos.y - peeShooterSize.y/2*0.25f;
			srcPos.x = srcPos.x - peeShooterSize.x/2*0.25f;
			sprite->setPosition(srcPos);
			sprite->setScale(0.25f);

			//Vector2 srcPos = getStage()->getSize() - sprite->getSize();
			//srcPos.y = scalar::randFloat(0.0f, srcPos.y);
			//Vector2 destPos = Vector2(0, srcPos.y);
			////set sprite initial position
			//sprite->setPosition(srcPos);

			////add another tween: TweenQueue
			////TweenQueue is a collection of tweens
			//spTweenQueue tweenQueue = new TweenQueue();
			////tweenQueue->setDelay(1500);
			////first, move sprite to dest position
			//tweenQueue->add(Sprite::TweenPosition(destPos), 2500, 1);
			////then fade it out smoothly
			//tweenQueue->add(Sprite::TweenAlpha(0), 500, 1);

			//sprite->addTween(tweenQueue);

			////and remove sprite from tree when tweenQueue is empty
			////if you don't hold any references to sprite it would be deleted automatically
			//tweenQueue->detachWhenDone();
		}
    }

	std::list<spSprite> MainActor::GetActiveElements(ActiveElements& elements, int lane)
	{
		ActiveElements::iterator it =  elements.find(lane);
		if(it == elements.end())
		{
			std::list<spSprite> elementList;
			elements.insert(ActiveElementPair(lane, elementList));
			return elementList;
		}
		else
		{
			return it->second;
		}
	}

	void MainActor::RemoveActiveElement(ActiveElements& elements, int lane, spSprite sprite)
	{
		ActiveElements::iterator it =  elements.find(lane);
		std::list<spSprite> elementList;
		if(it != elements.end())
		{
			elementList = it->second;
			elementList.remove(sprite);
		}
		
		elements[lane] = elementList;
	}

	void MainActor::AddActiveElement(ActiveElements& elements, int lane, spSprite sprite)
	{
		ActiveElements::iterator it =  elements.find(lane);
		std::list<spSprite> elementList;
		if(it != elements.end())
		{
			elementList = it->second;
		}
		elementList.push_back(sprite);
		elements[lane] = elementList;
	}

    void MainActor::runSprite()
    {
		Vector2 srcPos = getStage()->getSize();// - sprite->getSize();
		srcPos.y = scalar::randFloat(0.0f, srcPos.y);			
        Vector2 destPos = Vector2(0, srcPos.y);
        //set sprite initial position
		int lane = scalar::randFloat(1.0f, blockPositions.size());
		std::list<Vector2> centerPosList = blockPositions[lane];
		srcPos = centerPosList.back();
		srcPos.x = srcPos.x + blockWidth;
		srcPos.y = srcPos.y + blockHeight/2;
		destPos = centerPosList.front();
		destPos.y = destPos.y + blockHeight/2;

        spZombie sprite = new Zombie(lane, srcPos, destPos, mResources);
        addChild(sprite);

        //Add to Zombies
		AddActiveElement(zombies, lane, sprite);        
    }
