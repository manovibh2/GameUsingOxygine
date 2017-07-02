#pragma once

class MainActor: public Actor
{
public:
    spTextField _text;
    spSprite    _button;
	timeMS previousUS;
	timeMS collisionUS;
	typedef std::map<int, std::list<Vector2>>BlockPositions;
	typedef std::pair<int, std::list<Vector2>> BlockPositionPair;
	typedef std::map<int, std::list<spSprite>> ActiveElements;
	typedef std::pair<int, std::list<spSprite>> ActiveElementPair;
	
	int blockWidth, blockHeight;
	BlockPositions blockPositions;
	BlockPositions blockCenterPositions;
	ActiveElements peeShooters;
	ActiveElements zombies;
	ActiveElements pees;
	//Resources& mResources;

	bool bClicked;


	MainActor(Resources& gameResources);
	std::list<spSprite> GetActiveElements(ActiveElements& elements, int lane);
	void bulletCreated(Event* ev);
	void plantCreated(Event* ev);
private:
	Resources& mResources;
	void doUpdate(const UpdateState& us);
	void explode(spSprite sprite);
	void CheckCollision();
	void ShootPees();
	void ShootPee(Vector2 pos, int lane);
	void buttonClicked(Event* event);
	void createOutline();
	void addPeeShooters();
	
	void RemoveActiveElement(ActiveElements& elements, int lane, spSprite sprite);
	void AddActiveElement(ActiveElements& elements, int lane, spSprite sprite);
	void runSprite();

};

//declare spMainActor as intrusive_ptr holder of MainActor
//class MainActor;
typedef oxygine::intrusive_ptr<MainActor> spMainActor;
spMainActor GetMainActor();