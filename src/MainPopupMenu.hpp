#pragma once

#include <Geode/modify/MenuLayer.hpp>

#include "globals.hpp"

using namespace geode::prelude;

class MainPopupMenu : public Popup {
	protected:

		bool init(std::string const& value) {

            /*
			this->setTitle("All Cubes");

            auto popupMenu = m_mainLayer->getChildByType<CCMenu>(0);

			auto label = CCLabelBMFont::create(value.c_str(), "bigFont.fnt");
            label->setScale(0.5f);
			label->setID("all-cubes-popup-label"_spr);
			popupMenu->addChildAtPosition(label, Anchor::Center);

            */

            if (!Popup::init(300.f, 80.f))
                return false;
            CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

            auto node = CCLabelBMFont::create(value.c_str(), "bigFont.fnt");
            node->setScale(0.6f);
            this->addChild(node);
            auto menu = this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0);
            node->setPosition(screenSize/2);
            node->setPositionY(182.f);
            
			// Ship Button
			auto shipSpr = CircleButtonSprite::create(CCSprite::createWithSpriteFrameName("gj_shipBtn_on_001.png"), CircleBaseColor::Green, CircleBaseSize::SmallAlt);
			auto shipBtn = CCMenuItemSpriteExtra::create(shipSpr, menu, menu_selector(MainPopupMenu::onShipButton));
            shipBtn->setSprite(CCSprite::createWithSpriteFrameName(fmt::format("gj_shipBtn_{}_001.png", Globals::get().isShipCube ? "on" : "off").c_str()));
			shipBtn->setID("all-cubes-ship-button"_spr);
			menu->addChildAtPosition(shipBtn, Anchor::BottomLeft, ccp(41.5f, 24.f));

            // Ball Button
            auto ballSpr = CircleButtonSprite::create(CCSprite::createWithSpriteFrameName("gj_ballBtn_on_001.png"), CircleBaseColor::Green, CircleBaseSize::SmallAlt);
			auto ballBtn = CCMenuItemSpriteExtra::create(ballSpr, menu, menu_selector(MainPopupMenu::onBallButton));
            ballBtn->setSprite(CCSprite::createWithSpriteFrameName(fmt::format("gj_ballBtn_{}_001.png", Globals::get().isBallCube ? "on" : "off").c_str()));
			ballBtn->setID("all-cubes-ball-button"_spr);
			menu->addChildAtPosition(ballBtn, Anchor::BottomLeft, ccp(77.5f, 24.f));

            // UFO Button
            auto UFOSpr = CircleButtonSprite::create(CCSprite::createWithSpriteFrameName("gj_birdBtn_on_001.png"), CircleBaseColor::Green, CircleBaseSize::SmallAlt);
			auto UFOBtn = CCMenuItemSpriteExtra::create(UFOSpr, menu, menu_selector(MainPopupMenu::onUFOButton));
            UFOBtn->setSprite(CCSprite::createWithSpriteFrameName(fmt::format("gj_birdBtn_{}_001.png", Globals::get().isUFOCube ? "on" : "off").c_str()));
			UFOBtn->setID("all-cubes-ufo-button"_spr);
			menu->addChildAtPosition(UFOBtn, Anchor::BottomLeft, ccp(113.5f, 24.f));

            // Wave Button
            auto waveSpr = CircleButtonSprite::create(CCSprite::createWithSpriteFrameName("gj_dartBtn_on_001.png"), CircleBaseColor::Green, CircleBaseSize::SmallAlt);
			auto waveBtn = CCMenuItemSpriteExtra::create(waveSpr, menu, menu_selector(MainPopupMenu::onWaveButton));
            waveBtn->setSprite(CCSprite::createWithSpriteFrameName(fmt::format("gj_dartBtn_{}_001.png", Globals::get().isWaveCube ? "on" : "off").c_str()));
			waveBtn->setID("all-cubes-wave-button"_spr);
			menu->addChildAtPosition(waveBtn, Anchor::BottomLeft, ccp(149.5f, 24.f));

            // Robot Button
            auto robotSpr = CircleButtonSprite::create(CCSprite::createWithSpriteFrameName("gj_robotBtn_on_001.png"), CircleBaseColor::Green, CircleBaseSize::SmallAlt);
			auto robotBtn = CCMenuItemSpriteExtra::create(robotSpr, menu, menu_selector(MainPopupMenu::onRobotButton));
            robotBtn->setSprite(CCSprite::createWithSpriteFrameName(fmt::format("gj_robotBtn_{}_001.png", Globals::get().isRobotCube ? "on" : "off").c_str()));
			robotBtn->setID("all-cubes-robot-button"_spr);
			menu->addChildAtPosition(robotBtn, Anchor::BottomLeft, ccp(185.5f, 24.f));

            // Spider Button
            auto spiderSpr = CircleButtonSprite::create(CCSprite::createWithSpriteFrameName("gj_spiderBtn_on_001.png"), CircleBaseColor::Green, CircleBaseSize::SmallAlt);
			auto spiderBtn = CCMenuItemSpriteExtra::create(spiderSpr, menu, menu_selector(MainPopupMenu::onSpiderButton));
            spiderBtn->setSprite(CCSprite::createWithSpriteFrameName(fmt::format("gj_spiderBtn_{}_001.png", Globals::get().isSpiderCube ? "on" : "off").c_str()));
			spiderBtn->setID("all-cubes-spider-button"_spr);
			menu->addChildAtPosition(spiderBtn, Anchor::BottomLeft, ccp(221.5f, 24.f));

            // Swing Button
            auto swingSpr = CircleButtonSprite::create(CCSprite::createWithSpriteFrameName("gj_swingBtn_on_001.png"), CircleBaseColor::Green, CircleBaseSize::SmallAlt);
			auto swingBtn = CCMenuItemSpriteExtra::create(swingSpr, menu, menu_selector(MainPopupMenu::onSwingButton));
            swingBtn->setSprite(CCSprite::createWithSpriteFrameName(fmt::format("gj_swingBtn_{}_001.png", Globals::get().isSwingCube ? "on" : "off").c_str()));
			swingBtn->setID("all-cubes-swing-button"_spr);
			menu->addChildAtPosition(swingBtn, Anchor::BottomLeft, ccp(257.5f, 24.f));

            return true;
		}

	public:

		static MainPopupMenu* create(std::string const& text) {
			auto ret = new MainPopupMenu();
			if (ret->init(text)) {
				ret->autorelease();
				return ret;
			}

			delete ret;
			return nullptr;
		}

    private:

		void onShipButton(CCObject* sender) {
            auto shipBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
            if (shipBtn) {
                if (Globals::get().isShipCube) {
				    Globals::get().isShipCube = false;
                    shipBtn->setSprite(CCSprite::createWithSpriteFrameName("gj_shipBtn_off_001.png"));
                } else {
                    Globals::get().isShipCube = true;
                    shipBtn->setSprite(CCSprite::createWithSpriteFrameName("gj_shipBtn_on_001.png"));
                }
            }
            else {
                geode::log::error("Couldn't find popup's ship button");
            }
		}

        void onBallButton(CCObject* sender) {
            auto ballBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
            if (ballBtn) {
                if (Globals::get().isBallCube) {
				    Globals::get().isBallCube = false;
                    ballBtn->setSprite(CCSprite::createWithSpriteFrameName("gj_ballBtn_off_001.png"));
                } else {
                    Globals::get().isBallCube = true;
                    ballBtn->setSprite(CCSprite::createWithSpriteFrameName("gj_ballBtn_on_001.png"));
                }
            }
            else {
                geode::log::error("Couldn't find popup's ball button");
            }
		}

        void onUFOButton(CCObject* sender) {
            auto UFOBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
            if (UFOBtn) {
                if (Globals::get().isUFOCube) {
				    Globals::get().isUFOCube = false;
                    UFOBtn->setSprite(CCSprite::createWithSpriteFrameName("gj_birdBtn_off_001.png"));
                } else {
                    Globals::get().isUFOCube = true;
                    UFOBtn->setSprite(CCSprite::createWithSpriteFrameName("gj_birdBtn_on_001.png"));
                }
            }
            else {
                geode::log::error("Couldn't find popup's UFO button");
            }
		}

        void onWaveButton(CCObject* sender) {
            auto waveBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
            if (waveBtn) {
                if (Globals::get().isWaveCube) {
				    Globals::get().isWaveCube = false;
                    waveBtn->setSprite(CCSprite::createWithSpriteFrameName("gj_dartBtn_off_001.png"));
                } else {
                    Globals::get().isWaveCube = true;
                    waveBtn->setSprite(CCSprite::createWithSpriteFrameName("gj_dartBtn_on_001.png"));
                }
            }
            else {
                geode::log::error("Couldn't find popup's wave button");
            }
		}

        void onRobotButton(CCObject* sender) {
            auto robotBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
            if (robotBtn) {
                if (Globals::get().isRobotCube) {
				    Globals::get().isRobotCube = false;
                    robotBtn->setSprite(CCSprite::createWithSpriteFrameName("gj_robotBtn_off_001.png"));
                } else {
                    Globals::get().isRobotCube = true;
                    robotBtn->setSprite(CCSprite::createWithSpriteFrameName("gj_robotBtn_on_001.png"));
                }
            }
            else {
                geode::log::error("Couldn't find popup's robot button");
            }
		}

        void onSpiderButton(CCObject* sender) {
            auto spiderBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
            if (spiderBtn) {
                if (Globals::get().isSpiderCube) {
				    Globals::get().isSpiderCube = false;
                    spiderBtn->setSprite(CCSprite::createWithSpriteFrameName("gj_spiderBtn_off_001.png"));
                } else {
                    Globals::get().isSpiderCube = true;
                    spiderBtn->setSprite(CCSprite::createWithSpriteFrameName("gj_spiderBtn_on_001.png"));
                }
            }
            else {
                geode::log::error("Couldn't find popup's spider button");
            }
		}

        void onSwingButton(CCObject* sender) {
            auto swingBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
            if (swingBtn) {
                if (Globals::get().isSwingCube) {
				    Globals::get().isSwingCube = false;
                    swingBtn->setSprite(CCSprite::createWithSpriteFrameName("gj_swingBtn_off_001.png"));
                } else {
                    Globals::get().isSwingCube = true;
                    swingBtn->setSprite(CCSprite::createWithSpriteFrameName("gj_swingBtn_on_001.png"));
                }
            }
            else {
                geode::log::error("Couldn't find popup's swing button");
            }
		}
};