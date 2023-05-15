#include "ChatSuffix.h"
#include <locale>
#include <codecvt>
ChatSuffix::ChatSuffix() : IModule(0, Category::OTHER, "Adds the client suffix") {
	registerBoolSetting("Bypass", &bypass, bypass);
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
		if (bypass) {
			int utf8Value = random(262144, 917503);
			std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
			std::string bypassStr = converter.to_bytes(static_cast<char32_t>(utf8Value));
			end += bypassStr;
		}
		Sentence = funy->message.getText() + end;
		funy->message.resetWithoutDelete();
		funy->message = Sentence;
	}
}

