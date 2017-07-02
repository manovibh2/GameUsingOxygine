#pragma once

DECLARE_SMART(Plant, spPlant);
class Plant: public Sprite
{
public:
	Plant(int lane, Vector2 srcPos, Resources& gameResources);

private://Methods
	void doUpdate(const UpdateState& us);
	void ShootPee(Vector2 pos, int lane);

private:
	Resources& mResources;
	int mLane;
	timeMS mLastUpdate;
};

