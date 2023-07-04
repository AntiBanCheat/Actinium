#include "CubePlayerTP.h"
#include "../pch.h"
#include <algorithm>
using namespace std;
CubePlayerTP::CubePlayerTP() : IModule(0, Category::COMBAT, "DHUAUN!DHUAUN!DHUAUN!DHUAUN!DHUAUN!DHUAUN!DHUAUN!DHUAUN!") {
	registerIntSetting("Range", &range, range, 5, 250);
	registerIntSetting("Delay", &delay, delay, 0, 20);
	registerIntSetting("Height", &height, height, 0, 5);
	registerBoolSetting("Once", &once, once);
	registerBoolSetting("Behind", &behind, behind);
	registerBoolSetting("voidCheck", &voidCheck, voidCheck);
}

const char* CubePlayerTP::getRawModuleName() {
	return "CubePlayerTP";
}

const char* CubePlayerTP::getModuleName() {
	name = string("CubePlayerTP ") + GRAY + string(to_string(range));
	return name.c_str();
}

static vector<C_Entity*> targetList;
void findEntTPA2(C_Entity* currentEntity, bool isRegularEntity) {
	static auto cubePlayerTP = moduleMgr->getModule<CubePlayerTP>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() == 80 || currentEntity->getEntityTypeId() == 69)  // XP and Arrows
		return;

	if (!TargetUtil::isValidTarget(currentEntity))
		return;

	if (currentEntity->getEntityTypeId() == 51 || currentEntity->getEntityTypeId() == 1677999)  // Villagers and NPCS
		return;

	if (cubePlayerTP->voidCheck) {
		vec3_t pos = currentEntity->currentPos;
		vec3_t blockPos = vec3_t(floorf(pos.x), floorf(pos.y) + -1.0f, floorf(pos.z));
		int voidDistance = -(-64 - (int)floorf(pos.y) - 1);
		bool isVoid = true;
		for (size_t i = 0; i < min(voidDistance, 20); i++)
		{
			blockPos = vec3_t(blockPos.x, blockPos.y - 1, blockPos.z);
			if (blockPos.y < -64)
				break;

			int id = currentEntity->region->getBlock(blockPos)->toLegacy()->blockId;
			if (id != 0)
			{
				isVoid = false;
			}
		}
		if (isVoid)
			return;
	}
	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < cubePlayerTP->range) targetList.push_back(currentEntity);
}

struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void CubePlayerTP::onEnable() {
	targetList.clear();
}

void CubePlayerTP::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	if (!once) {
		tick++;
		if (tick < delay)
			return;
		tick = 0;
	}
	targetList.clear();
	g_Data.forEachEntity(findEntTPA2);
	sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
	if (g_Data.canUseMoveKeys() && !targetList.empty()) {
		vec3_t pos = player->currentPos;
		vec3_t pos2 = targetList[0]->currentPos;
		float yawRad = (targetList[0]->yaw + 90) * (PI / 180);
		float pitchRad = (targetList[0]->pitch) * -(PI / 180);
		if (behind)
			pos2 = pos2.add(
				(cos(yawRad) * cos(pitchRad)) * -1.5f,
				0,
				(sin(yawRad) * cos(pitchRad)) * -1.5f
			);
		pos2 = pos2.add(0, height, 0);
		float dist = pos.dist(pos2);
		for (int i = 0; i < dist * 1.f; i++)
		{
			vec3_t pos3 = pos.lerp(pos2, i / (float)(dist * 1.f));
			PlayerAuthInputPacket p = PlayerAuthInputPacket(pos3, player->pitch, player->yaw, player->yawUnused1);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
			player->setPos(pos3);
		}
	}
	if (once)
		setEnabled(false);
}

void CubePlayerTP::onDisable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
}