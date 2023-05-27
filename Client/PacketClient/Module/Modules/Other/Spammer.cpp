#include "Spammer.h"
#include "../pch.h"

Spammer::Spammer() : IModule(0, Category::OTHER, "Spams a message in a specified delay") {
	registerIntSetting("delay", &delay, delay, 0, 30);
}

const char* Spammer::getModuleName() {
	return ("Spammer");
}

void Spammer::onTick(C_GameMode* gm) {
	Odelay++;
	if (Odelay > delay * 20) {
		C_TextPacket textPacket;
		textPacket.message.setText(message);
		textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
		Odelay = 0;
	}
}