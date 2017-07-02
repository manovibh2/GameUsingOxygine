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

//it is our resources
//in real project you would have more than one Resources declarations.
//It is important on mobile devices with limited memory and you would load/unload them
Resources gameResources;




    

//you could use DECLARE_SMART preprocessor definition it does the same:
//DECLARE_SMART(MainActor, spMainActor)

void example_preinit() {}

//called from main.cpp
void example_init()
{
    //load xml file with resources definition
    gameResources.loadXML("res.xml");


    //lets create our client code simple actor
    //spMainActor was defined above as smart intrusive pointer (read more: http://www.boost.org/doc/libs/1_60_0/libs/smart_ptr/intrusive_ptr.html)
    spMainActor actor = new MainActor(gameResources);

    //and add it to Stage as child
    getStage()->addChild(actor);
}

spMainActor GetMainActor()
{
	spMainActor actor = safeSpCast<MainActor>(getStage()->getFirstChild());
	return actor;
}

//called each frame from main.cpp
void example_update()
{
}

//called each frame from main.cpp
void example_destroy()
{
    //free previously loaded resources
    gameResources.free();
}
