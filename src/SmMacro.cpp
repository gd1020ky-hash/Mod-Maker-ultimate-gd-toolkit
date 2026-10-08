#include "SmMacro.hpp"

#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

namespace sm {

static Pathfinder g_pathfinder;

// ============================================================
// Pathfinder
// ============================================================

void Pathfinder::start() {
    m_running = true;
    log::info("Ultimate GD Toolkit: Pathfinder started");
}

void Pathfinder::stop() {
    m_running = false;
    log::info("Ultimate GD Toolkit: Pathfinder stopped");
}

void Pathfinder::retry() {
    ++m_attempts;
    log::info("Ultimate GD Toolkit: Pathfinder retry {}", m_attempts);
}

void Pathfinder::verify() {
    log::info("Ultimate GD Toolkit: Pathfinder verification requested");
}

bool Pathfinder::running() const {
    return m_running;
}

uint64_t Pathfinder::attempts() const {
    return m_attempts;
}

uint32_t Pathfinder::searchDepth() const {
    return m_searchDepth;
}

void Pathfinder::setSearchDepth(uint32_t depth) {
    m_searchDepth = std::max<uint32_t>(1, depth);
}

// ============================================================
// Route exporter
// ============================================================

bool RouteExporter::exportRoute(
    Route const& route,
    ExportFormat format,
    std::string const& path
) {
    (void)route;
    (void)format;
    (void)path;

    log::info("Ultimate GD Toolkit: route export requested");

    return false;
}

// ============================================================
// Simple Mod Menu
// ============================================================

class ModMenu : public FLAlertLayer {
public:

    static ModMenu* create() {
        auto ret = new ModMenu();

        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }

        CC_SAFE_DELETE(ret);
        return nullptr;
    }

protected:

    bool init() {

        if (!FLAlertLayer::init(
            nullptr,
            "Ultimate GD Toolkit",
            "CLOSE",
            nullptr,
            420.f,
            300.f
        )) {
            return false;
        }

        auto layer = this->m_mainLayer;

        auto title = CCLabelBMFont::create(
            "ULTIMATE GD TOOLKIT",
            "bigFont.fnt"
        );

        title->setScale(0.65f);

        title->setPosition(
            ccp(210.f, 245.f)
        );

        layer->addChild(title);

        // ----------------------------------------------------
        // Macro button
        // ----------------------------------------------------

        auto macroButton =
            CCMenuItemSpriteExtra::create(
                ButtonSprite::create(
                    "MACRO",
                    "bigFont.fnt",
                    "GJ_button_01.png",
                    0.65f
                ),
                this,
                menu_selector(ModMenu::onMacro)
            );

        macroButton->setPosition(
            ccp(110.f, 185.f)
        );

        this->m_buttonMenu->addChild(macroButton);

        // ----------------------------------------------------
        // Pathfinder button
        // ----------------------------------------------------

        auto pathButton =
            CCMenuItemSpriteExtra::create(
                ButtonSprite::create(
                    "PATHFINDER",
                    "bigFont.fnt",
                    "GJ_button_01.png",
                    0.65f
                ),
                this,
                menu_selector(ModMenu::onPathfinder)
            );

        pathButton->setPosition(
            ccp(310.f, 185.f)
        );

        this->m_buttonMenu->addChild(pathButton);

        // ----------------------------------------------------
        // Frame Tools
        // ----------------------------------------------------

        auto frameButton =
            CCMenuItemSpriteExtra::create(
                ButtonSprite::create(
                    "FRAME",
                    "bigFont.fnt",
                    "GJ_button_01.png",
                    0.65f
                ),
                this,
                menu_selector(ModMenu::onFrame)
            );

        frameButton->setPosition(
            ccp(110.f, 125.f)
        );

        this->m_buttonMenu->addChild(frameButton);

        // ----------------------------------------------------
        // Playback
        // ----------------------------------------------------

        auto playbackButton =
            CCMenuItemSpriteExtra::create(
                ButtonSprite::create(
                    "PLAYBACK",
                    "bigFont.fnt",
                    "GJ_button_01.png",
                    0.65f
                ),
                this,
                menu_selector(ModMenu::onPlayback)
            );

        playbackButton->setPosition(
            ccp(310.f, 125.f)
        );

        this->m_buttonMenu->addChild(playbackButton);

        return true;
    }

    void onMacro(CCObject*) {

        FLAlertLayer::create(
            "SM Macro",
            "SM Macro\n\n"
            "Record\n"
            "Playback\n"
            "Native format: .sm",
            "BACK"
        )->show();
    }

    void onPathfinder(CCObject*) {

        std::string text =
            "Pathfinder\n\n"
            "Status: ";

        text += g_pathfinder.running()
            ? "RUNNING"
            : "STOPPED";

        text +=
            "\n\nSearch Depth: " +
            std::to_string(
                g_pathfinder.searchDepth()
            );

        text +=
            "\nAttempts: " +
            std::to_string(
                g_pathfinder.attempts()
            );

        FLAlertLayer::create(
            "Pathfinder",
            text,
            "BACK"
        )->show();
    }

    void onFrame(CCObject*) {

        FLAlertLayer::create(
            "Frame Tools",
            "Frame Counter\n"
            "Frame Window\n"
            "Frame Stepper\n"
            "Event Viewer",
            "BACK"
        )->show();
    }

    void onPlayback(CCObject*) {

        FLAlertLayer::create(
            "Playback",
            "SM Playback\n"
            "Frame-accurate input\n"
            "Music synchronization\n"
            "Press / Release events",
            "BACK"
        )->show();
    }
};

// ============================================================
// Open Mod Menu
// ============================================================

void openSMMenu() {

    auto menu = ModMenu::create();

    if (menu) {
        menu->show();
    }
}

// ============================================================
// Geometry Dash main menu
// ============================================================

class $modify(UltimateGDToolkitMenu, MenuLayer) {

    bool init() {

        if (!MenuLayer::init())
            return false;

        auto menu = this->getChildByID(
            "bottom-menu"
        );

        if (!menu) {

            log::error(
                "Ultimate GD Toolkit: bottom-menu not found"
            );

            return true;
        }

        // ----------------------------------------------------
        // Toolkit button
        // ----------------------------------------------------

        if (!menu->getChildByID(
            "ultimate-gd-toolkit-button"_spr
        )) {

            auto sprite = ButtonSprite::create(
                "Toolkit",
                "bigFont.fnt",
                "GJ_button_01.png",
                0.8f
            );

            sprite->setScale(0.55f);

            auto button =
                CCMenuItemSpriteExtra::create(
                    sprite,
                    this,
                    menu_selector(
                        UltimateGDToolkitMenu::onToolkit
                    )
                );

            button->setID(
                "ultimate-gd-toolkit-button"_spr
            );

            menu->addChild(button);
        }

        // ----------------------------------------------------
        // Mod Menu button
        // ----------------------------------------------------

        if (!menu->getChildByID(
            "ultimate-gd-mod-menu-button"_spr
        )) {

            auto sprite = ButtonSprite::create(
                "Mod Menu",
                "bigFont.fnt",
                "GJ_button_01.png",
                0.8f
            );

            sprite->setScale(0.55f);

            auto button =
                CCMenuItemSpriteExtra::create(
                    sprite,
                    this,
                    menu_selector(
                        UltimateGDToolkitMenu::onModMenu
                    )
                );

            button->setID(
                "ultimate-gd-mod-menu-button"_spr
            );

            menu->addChild(button);
        }

        menu->updateLayout();

        log::info(
            "Ultimate GD Toolkit: buttons loaded"
        );

        return true;
    }

    void onToolkit(CCObject*) {

        FLAlertLayer::create(
            "Ultimate GD Toolkit",
            "Toolkit button\n\n"
            "The main Toolkit systems will be connected here.",
            "OK"
        )->show();
    }

    void onModMenu(CCObject*) {

        openSMMenu();
    }
};

// ============================================================
// Mod loaded
// ============================================================

$on_mod(Loaded) {

    log::info(
        "Ultimate GD Toolkit loaded successfully!"
    );

}

}
