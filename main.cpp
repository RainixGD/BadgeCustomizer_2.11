#include "./includes.h"


#define public_cast(value, member) [](auto* v) { \
	class FriendClass__; \
	using T = std::remove_pointer<decltype(v)>::type; \
	class FriendeeClass__: public T { \
	protected: \
		friend FriendClass__; \
	}; \
	class FriendClass__ { \
	public: \
		auto& get(FriendeeClass__* v) { return v->member; } \
	} c; \
	return c.get(reinterpret_cast<FriendeeClass__*>(v)); \
}(value)
bool operator==(const cocos2d::CCPoint& a, const cocos2d::CCPoint& b) { return a.x == b.x && a.y == b.y; }
bool operator==(const cocos2d::CCRect& a, const cocos2d::CCRect& b) { return a.origin == b.origin && a.size == b.size; }

class BadgeCustomizerManager {
	enum DataLoadingResult {
		OK,
		FileNotFound,
		ParsingError
	};
	std::unordered_map<unsigned int, std::string> badges;
	DataLoadingResult loadingStatus;
	static BadgeCustomizerManager* instance;

	void init() {
		loadingStatus = loadData();
	}

	bool can_convert_to_int(const std::string& str) {
		try {
			std::stoi(str);
			return true;
		}
		catch (const std::invalid_argument& e) {
			return false;
		}
		catch (const std::out_of_range& e) {
			return false;
		}
	}

	DataLoadingResult loadData() {
		std::ifstream file("Resources/badgeCustomizer.json");
		if (!file) return FileNotFound;
		std::ostringstream buffer;
		buffer << file.rdbuf();
		std::string fileContent = buffer.str();

		file.close();
		try {
			auto root = nlohmann::json::parse(fileContent);

			if (!root.is_object()) return ParsingError;

			for (const auto& [key, value] : root.items()) {
				if (!can_convert_to_int(key)) return ParsingError;

				int badgeId = std::stoi(key);
				if (badgeId < 1 || !value.is_string()) return ParsingError;

				badges[badgeId] = value.get<std::string>();
			}
		}
		catch (const std::exception& e) {
			return ParsingError;
		}
		return OK;
	}

	void createErrorLabel(CCLayer* layer) {
		std::string errorText;
		switch (loadingStatus) {
		case BadgeCustomizerManager::FileNotFound:
			errorText = "Can't find 'badgeCustomizer.json' in ./Resources";
			break;
		case BadgeCustomizerManager::ParsingError:
			errorText = "Can't parse 'badgeCustomizer.json'";
			break;
		}

		auto size = CCDirector::sharedDirector()->getWinSize();

		auto errorLabel = CCLabelBMFont::create(errorText.c_str(), "bigFont.fnt");
		errorLabel->setColor({ 255, 0, 0 });
		errorLabel->setScale(0.6);
		errorLabel->setPosition({ size.width / 2, size.height - 10 });
		layer->addChild(errorLabel);
	}

	BadgeCustomizerManager() {};
public:

	void onMenuLayer(CCLayer* layer) {
		if (loadingStatus != OK) {
			createErrorLabel(layer);
			return;
		}
	}

	void onProfilePage_loadPageFromUserInfo(ProfilePage* self, GJUserScore* score) {
		if (loadingStatus != OK) return;

		int modValue = score->modBadge_;
		CCPoint modBadgePos;

		auto newSelfObj = self->getChildren()->objectAtIndex(0);
		auto newSelf = dynamic_cast<CCNode*>(newSelfObj);

		if (modValue > 0) {
			CCObject* obj;
			CCARRAY_FOREACH(newSelf->getChildren(), obj) {
				auto NodeObj = reinterpret_cast<CCNode*>(obj);
				if (auto sprite_node = dynamic_cast<CCSprite*>(obj); sprite_node) {
					auto* texture = sprite_node->getTexture();
					CCDictElement* el;
					auto* frame_cache = CCSpriteFrameCache::sharedSpriteFrameCache();
					auto* cached_frames = public_cast(frame_cache, m_pSpriteFrames);
					const auto rect = sprite_node->getTextureRect();
					CCDICT_FOREACH(cached_frames, el) {
						auto* frame = static_cast<CCSpriteFrame*>(el->getObject());
						if (frame->getTexture() == texture && frame->getRect() == rect) {
							if (std::string(el->getStrKey()) == std::string("modBadge_02_001.png") || std::string(el->getStrKey()) == std::string("modBadge_01_001.png")) {
								modBadgePos = NodeObj->getPosition();
								NodeObj->removeFromParent();
							}
							break;
						}
					}
				}
			}
			if (badges.find(modValue) == badges.end()) return;
			auto newBadge = CCSprite::create(badges[modValue].c_str());
			if (newBadge == nullptr)
				newBadge = CCSprite::createWithSpriteFrameName("edit_delBtnSmall_001.png");
			newBadge->setPosition(modBadgePos);
			newSelf->addChild(newBadge);
		}
	}

	void onCommentCell_loadFromComment(CommentCell* cell, GJComment* comment) {
		if (loadingStatus != OK) return;

		int modValue = comment->m_modBadge;
		CCPoint modBadgePos;

		auto newSelfObj = cell->getChildren()->objectAtIndex(1);
		auto newSelf = dynamic_cast<CCNode*>(newSelfObj);

		if (modValue > 0) {
			CCObject* obj;
			CCARRAY_FOREACH(newSelf->getChildren(), obj) {
				auto NodeObj = reinterpret_cast<CCNode*>(obj);
				if (auto sprite_node = dynamic_cast<CCSprite*>(obj); sprite_node) {
					auto* texture = sprite_node->getTexture();
					CCDictElement* el;
					auto* frame_cache = CCSpriteFrameCache::sharedSpriteFrameCache();
					auto* cached_frames = public_cast(frame_cache, m_pSpriteFrames);
					const auto rect = sprite_node->getTextureRect();
					CCDICT_FOREACH(cached_frames, el) {
						auto* frame = static_cast<CCSpriteFrame*>(el->getObject());
						if (frame->getTexture() == texture && frame->getRect() == rect) {
							if (std::string(el->getStrKey()) == std::string("modBadge_02_001.png") || std::string(el->getStrKey()) == std::string("modBadge_01_001.png")) {
								modBadgePos = NodeObj->getPosition();
								NodeObj->removeFromParent();
							}
							break;
						}
					}
				}
			}
			if (badges.find(modValue) == badges.end()) return;
			auto newBadge = CCSprite::create(badges[modValue].c_str());
			if (newBadge == nullptr)
				newBadge = CCSprite::createWithSpriteFrameName("edit_delBtnSmall_001.png");
			newBadge->setPosition(modBadgePos);
			newSelf->addChild(newBadge);
		}
	}

	static BadgeCustomizerManager* getInstance() {
		if (!instance) {
			instance = new BadgeCustomizerManager();
			instance->init();
		}
		return instance;
	}
};
BadgeCustomizerManager* BadgeCustomizerManager::instance = nullptr;

bool(__thiscall* MenuLayer_init)(MenuLayer* self);
bool __fastcall MenuLayer_init_H(MenuLayer* self, void*) {
	if (!MenuLayer_init(self)) return false;
	BadgeCustomizerManager::getInstance()->onMenuLayer(self);
	return true;
}

void(__thiscall* ProfilePage_loadPageFromUserInfo)(ProfilePage* self, GJUserScore* score);
void __fastcall ProfilePage_loadPageFromUserInfo_H(ProfilePage* self, void*, GJUserScore* score) {
	ProfilePage_loadPageFromUserInfo(self, score);
	BadgeCustomizerManager::getInstance()->onProfilePage_loadPageFromUserInfo(self, score);
}

void(__thiscall* CommentCell_loadFromComment)(CommentCell* cell, GJComment* comment);
void __fastcall CommentCell_loadFromComment_H(CommentCell* cell, void*, GJComment* comment) {
	CommentCell_loadFromComment(cell, comment);
	BadgeCustomizerManager::getInstance()->onCommentCell_loadFromComment(cell, comment);
}

void inject() {
#if _WIN32
	auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x1907b0),
		reinterpret_cast<void*>(&MenuLayer_init_H),
		reinterpret_cast<void**>(&MenuLayer_init)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x210040),
		reinterpret_cast<void*>(&ProfilePage_loadPageFromUserInfo_H),
		reinterpret_cast<void**>(&ProfilePage_loadPageFromUserInfo)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x5f3d0),
		reinterpret_cast<void*>(&CommentCell_loadFromComment_H),
		reinterpret_cast<void**>(&CommentCell_loadFromComment)
	);

	MH_EnableHook(MH_ALL_HOOKS);
#endif
}

#if _WIN32
WIN32CAC_ENTRY(inject)
#endif