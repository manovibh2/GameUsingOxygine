#pragma once

DECLARE_SMART(Block, spBlock);

class Block:public Sprite
{
public:
	bool bClicked;
	spSprite sprite;
	int mLane;
	Block(int lane, Vector2 pos, Vector2 size, Resources& gameResources);
	
private:
	Resources& mResources;
	void onSpriteClicked(Event* event1);
	
};
