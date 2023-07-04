#pragma once
#include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class CubePlayerTP : public IModule {
private:
	int tick = 0;
public:
	int range = 250.f;
	int delay = 0;
	int height = 0;
	bool once = false;
	bool behind = false;
	bool voidCheck = false;
	std::string name = u8"CubePlayerTP";

	virtual const char* getModuleName();
	virtual const char* getRawModuleName();
	virtual void onTick(C_GameMode* gm);
	virtual void onDisable();
	virtual void onEnable();
	CubePlayerTP();
};