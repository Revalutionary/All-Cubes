#include <Geode/DefaultInclude.hpp>

#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GhostTrailEffect.hpp>
#include <cue/PlayerIcon.hpp>

using namespace geode::prelude;

#include "MainPopupMenu.hpp"
#include "globals.hpp"

class $modify(HGJGarageLayer, GJGarageLayer) {
    bool init() {
        if (!GJGarageLayer::init())
            return false;
		if (Mod::get()->getSettingValue<bool>("enabled")) {

			auto playerIcon = cue::PlayerIcon::create(IconType::Cube, GameManager::get()->getPlayerFrame(), GameManager::get()->m_playerColor, GameManager::get()->m_playerColor2, GameManager::get()->m_playerGlowColor);
			auto sprite = CircleButtonSprite::create(playerIcon);
			sprite->setID("all-cubes-menu-sprite"_spr);
			sprite->setScale(0.8f);
			playerIcon->setScale(0.85f);

			auto btn = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(HGJGarageLayer::onButton));
			btn->setPosition(ccp(20.25f, 101.5f));
			btn->setID("all-cubes-menu-button"_spr);
			
			auto subMenu = this->getChildByID("shards-menu");
			subMenu->addChild(btn);
		}

        return true;
    }

	void onBack(CCObject* sender) {
		Mod::get()->setSavedValue<bool>("isShipCube", Globals::get().isShipCube);
		Mod::get()->setSavedValue<bool>("isBallCube", Globals::get().isBallCube);
		Mod::get()->setSavedValue<bool>("isUFOCube", Globals::get().isUFOCube);
		Mod::get()->setSavedValue<bool>("isWaveCube", Globals::get().isUFOCube);
		Mod::get()->setSavedValue<bool>("isRobotCube", Globals::get().isRobotCube);
		Mod::get()->setSavedValue<bool>("isSpiderCube", Globals::get().isSpiderCube);
		Mod::get()->setSavedValue<bool>("isSwingCube", Globals::get().isSwingCube);
		GJGarageLayer::onBack(sender);
	}

	public:

		void onButton(CCObject* sender) {
			geode::log::debug("button pressed");
			auto mainMenuPopup = MainPopupMenu::create("Choose which gamemodes");
			// mainMenuPopup->m_scene = this;
			mainMenuPopup->show();
		}
};

class $modify(HPlayerObject, PlayerObject) {
	struct Fields {
		cue::PlayerIcon* m_customSprite = nullptr;
		bool enabled = false;
		bool menuEnabled = false;
		bool rotationEnabled = false;
		bool platformerEnabled = false;
		float waveSize = 0.35f;
	};

	enum Gamemodes : std::uint8_t {
		Cube,
		Ship,
		Ball,
		UFO,
		Wave,
		Robot,
		Spider,
		Swing,
	};

		bool init(int player, int ship, GJBaseGameLayer* gameLayer, CCLayer* layer, bool playLayer) {
			if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer)) 
				return false;
			auto fields = m_fields.self();
			fields->enabled = Mod::get()->getSettingValue<bool>("enabled");
			fields->menuEnabled = Mod::get()->getSettingValue<bool>("enabled-main-menu");
			fields->rotationEnabled = Mod::get()->getSettingValue<bool>("icon-rotation");
			fields->platformerEnabled = Mod::get()->getSettingValue<bool>("enabled-platformer");
			fields->waveSize = static_cast<float>(Mod::get()->getSettingValue<double>("wave-size"));
			if (m_isPlatformer && !(fields->platformerEnabled)) {
				return true;
			}
			if (!PlayLayer::get() && !(fields->menuEnabled)) {
				return true;
			}
			if (!(fields->enabled)) {
				return true;
			}
			if (Globals::get().isShipCube || Globals::get().isBallCube || Globals::get().isUFOCube || Globals::get().isWaveCube || Globals::get().isRobotCube || Globals::get().isSpiderCube || Globals::get().isSwingCube) {
				fields->m_customSprite = cue::PlayerIcon::create(IconType::Cube, player, GameManager::get()->m_playerColor, GameManager::get()->m_playerColor2, GameManager::get()->m_playerGlowColor);
				fields->m_customSprite->setScale(1.f);
				fields->m_customSprite->setAnchorPoint(ccp(0.f, 0.f));
				fields->m_customSprite->setPosition(ccp(-16.5f, -16.5f));
				fields->m_customSprite->setID("all-cubes-icon-replacement"_spr);
				this->addChild(fields->m_customSprite, 20);
				fields->m_customSprite->setVisible(true);
			} else {
				m_mainLayer->getChildByID("gamemode-frame")->setVisible(true);
			}
			return true;
		}

		private:

		HPlayerObject::Gamemodes gamemode() {
			if (m_isShip)
				return Ship;
			else if (m_isBall)
				return Ball;
			else if (m_isBird)
				return UFO;
			else if (m_isDart)
				return Wave;
			else if (m_isRobot)
				return Robot;
			else if (m_isSpider)
				return Spider;
			else if (m_isSwing)
				return Swing;
			else 
				return Cube;
		}

		public:

		void update(float dt) {
			PlayerObject::update(dt);

			auto fields = m_fields.self();

			// if mod isn't enabled
			if (!(fields->enabled)) {
				return;
			}

			if (!(Globals::get().isShipCube || Globals::get().isBallCube || Globals::get().isUFOCube || Globals::get().isWaveCube || Globals::get().isRobotCube || Globals::get().isSpiderCube || Globals::get().isSwingCube)) {
				return;
			}

			// if in platformer and platformer is disabled
			if (m_isPlatformer && !(fields->platformerEnabled)) {
				return;
			}

			// If on main menu bg and main menu bg icons are disabled
			if (!(PlayLayer::get()) && !(fields->menuEnabled)) {
				m_mainLayer->getChildByID("glow-frame")->setVisible(true);
				m_mainLayer->getChildByID("gamemode-frame")->setVisible(true);
				return;
			}

			// If on the level completed screen
			if (PlayLayer::get()) {
				if (PlayLayer::get()->getChildByType<EndLevelLayer>(0)) {
					fields->m_customSprite->setVisible(false);
					return;
				}
			}

			auto activeMode = this->gamemode();

			switch (activeMode) {
				case Ship: {
					if (m_mainLayer->getChildByID("ship-frame")) {
						if (!Globals::get().isShipCube) {
							m_mainLayer->getChildByID("ship-frame")->setVisible(true);
							goto activeDisabled;
						} else {
							m_mainLayer->getChildByID("ship-frame")->setVisible(false);
							if (!fields->rotationEnabled) {
								this->setRotation(0.f);
							}
							m_mainLayer->getChildByID("ship-frame")->setVisible(false);
							goto activeEnabled;
						}
					}
				}
				case UFO: {	
					if (m_mainLayer->getChildByID("ship-frame")) {
						if (!Globals::get().isUFOCube) {
							m_mainLayer->getChildByID("ship-frame")->setVisible(true);
							goto activeDisabled;
						} else {
							if (!fields->rotationEnabled) {
								this->setRotation(0.f);
							}
							m_mainLayer->getChildByID("ship-frame")->setVisible(false);
							goto activeEnabled;
						}
					}
				}
				case Ball: {
					if (!Globals::get().isBallCube) {
						goto activeDisabled;
					} else {
						if (!fields->rotationEnabled) {
							this->setRotation(0.f);
						}
						goto activeEnabled;
					}
				}
				case Cube: {
					if (!fields->rotationEnabled) {
						this->setRotation(0.f);
					}
					goto activeEnabled;
				}
				case Robot: {
					if (!Globals::get().isRobotCube) {
						m_robotBatchNode->setVisible(true);
						fields->m_customSprite->setVisible(false);
						return;
					} else {
						m_robotBatchNode->setVisible(false);
						goto activeEnabled;
					}
				}
				case Spider: {
					if (!Globals::get().isSpiderCube) {
						m_spiderBatchNode->setVisible(true);
						fields->m_customSprite->setVisible(false);
						return;
					} else {
						m_spiderBatchNode->setVisible(false);
						goto activeEnabled;
					}
				}
				case Wave: {
					if (!Globals::get().isWaveCube) {
						goto activeDisabled;
					} else {
						fields->m_customSprite->setScale(fields->waveSize);
						goto activeEnabled;
					}
				}
				case Swing: {
					if (!Globals::get().isSwingCube) {
						goto activeDisabled;
					} else {
						m_mainLayer->getChildByID("swing-bottom-boost")->setVisible(true);
						m_mainLayer->getChildByID("swing-middle-boost")->setVisible(true);
						m_mainLayer->getChildByID("swing-top-boost")->setVisible(true);
						if (!fields->rotationEnabled) {
							this->setRotation(0.f);
						}
						goto activeEnabled;
					}
				}
			}
			
			activeDisabled:
				m_mainLayer->getChildByID("glow-frame")->setVisible(true);
				m_mainLayer->getChildByID("gamemode-frame")->setVisible(true);
				fields->m_customSprite->setVisible(false);
				return;
			activeEnabled:
				m_mainLayer->getChildByID("glow-frame")->setVisible(false);
				m_mainLayer->getChildByID("gamemode-frame")->setVisible(false);
				fields->m_customSprite->setVisible(true);
				return;
		}

		void toggleDartMode(bool enable, bool noEffects) {
			auto fields = m_fields.self();
			PlayerObject::toggleDartMode(enable, noEffects);
			if (fields->enabled && (Globals::get().isShipCube || Globals::get().isBallCube || Globals::get().isUFOCube || Globals::get().isWaveCube || Globals::get().isRobotCube || Globals::get().isSpiderCube || Globals::get().isSwingCube)) {
				if (!PlayLayer::get() && !fields->menuEnabled) {
					m_mainLayer->getChildByID("glow-frame")->setVisible(true);
					m_mainLayer->getChildByID("gamemode-frame")->setVisible(true);
					return;
				}
			
				if (enable && Globals::get().isWaveCube) {
					fields->m_customSprite->setScale(fields->waveSize);
					fields->m_customSprite->setPosition(ccp(-5.5f, -5.5f));
				} else if (!enable && Globals::get().isWaveCube) {
					fields->m_customSprite->setScale(1.f);
					fields->m_customSprite->setPosition(ccp(-16.5f, -16.5f));
				}
			}
		}

		void playDeathEffect() {
			auto fields = m_fields.self();
			geode::log::info("Ship: {} Ball: {} UFO: {} Wave: {} Robot: {} Spider: {} Swing: {}", Globals::get().isShipCube, Globals::get().isBallCube, Globals::get().isUFOCube, Globals::get().isWaveCube, Globals::get().isRobotCube, Globals::get().isSpiderCube, Globals::get().isSwingCube);
			if (fields->enabled && (Globals::get().isShipCube || Globals::get().isBallCube || Globals::get().isUFOCube || Globals::get().isWaveCube || Globals::get().isRobotCube || Globals::get().isSpiderCube || Globals::get().isSwingCube)){
				geode::log::info("inside the if statement FUCK!");
				fields->m_customSprite->setVisible(false);
				m_mainLayer->getChildByID("gamemode-frame")->setVisible(true);
			}
			PlayerObject::playDeathEffect();
		}
		
		void playCompleteEffect(bool noEffects, bool instant) {		
			auto fields = m_fields.self();
			if (fields->enabled && (Globals::get().isShipCube || Globals::get().isBallCube || Globals::get().isUFOCube || Globals::get().isWaveCube || Globals::get().isRobotCube || Globals::get().isSpiderCube || Globals::get().isSwingCube)) {
				fields->m_customSprite->setVisible(false);
			}
			PlayerObject::playCompleteEffect(noEffects, instant);
		}
};

/* 

		TODO:
	Features:
 - Make player gamemode trail to be a cube as well when needed
 - FlipY when upsidedown

*/


/*
Dear HJFod,

You thought I was done? No no no, you can't escape this pal. I coded this mod so horribly that your eyes will bleed for days wishing you never made geode. You gave the general GD public so much control over the game that you will live to regret it. Sitting on your death bed you will regret 3 things: 1. Why did I make geode... 2. Why can't I lose weight... 3. For what is a man? What has he got? Yeah, that is from my way, because it's my way or the highway pal. You think your word is absolute huh? You aren't Allah, don't even try. Shut down shop unc, you don't got ts. Making children and trapping them in Nodes for their entire lives until they one day die from starvation isn't a good business move and the police is catching onto you. You can't keep getting away with this.

With all disrespect,
Revalutionary

(I would've coded this entire thing in TürkçeKod to make your life harder but I couldn't be bothered)
*/