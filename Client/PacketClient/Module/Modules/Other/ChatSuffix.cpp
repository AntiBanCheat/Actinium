#include "ChatSuffix.h"

ChatSuffix::ChatSuffix() : IModule(0, Category::OTHER, "Adds the client suffix") {
}

ChatSuffix::~ChatSuffix() {
}

const char* ChatSuffix::getModuleName() {
	return "ChatSuffix";
}

void ChatSuffix::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_TextPacket>()) {
		C_TextPacket* funy = reinterpret_cast<C_TextPacket*>(packet);
		std::string Sentence;
		std::string end;
		int i = randomFloat(1, 40);
		if (i == 1) end = " | Aceolus";
		if (i == 2) end = " | Skidders V3";
		if (i == 3) end = " | Radium V2";
		if (i >= 4 && i <= 6) end = " | A C T I N I U M";
		if (i > 6) end = " | Actinium";
#ifdef _DEBUG
		end += " Beta";
#endif // _DEBUG

		Sentence = funy->message.getText() + end;
		funy->message.resetWithoutDelete();
		funy->message = Sentence;
	}
}

