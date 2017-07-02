#pragma once

class GenericEvent: public Event
{
public:
    enum GEvent
    {
		PLANT_ADD = sysEventID('G', 'P', 'A'),
        PLANT_REMOVE = sysEventID('G', 'P', 'R'),
		BULLET_ADD = sysEventID('B', 'P', 'A'),
        BULLET_REMOVE = sysEventID('B', 'P', 'R')      
    };

    GenericEvent(GEvent ev) : Event(ev) {}
    int lane;
	spSprite sprite;
};