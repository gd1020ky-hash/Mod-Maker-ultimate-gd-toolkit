#include "SmMacro.hpp"

#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include <algorithm>
#include <string>

using namespace geode::prelude;

namespace sm {

static Pathfinder g_pathfinder;

// ============================================================
// Pathfinder
// ============================================================

void Pathfinder::start() {
    m_running = true;

    log::info(
        "Ultimate GD Toolkit: Pathfinder started"
    );
}

void Pathfinder::stop() {
    m_running = false;

    log::info(
        "Ultimate GD Toolkit: Pathfinder stopped"
    );
}

void Pathfinder::retry() {
    ++m_attempts;

    log::info(
        "Ultimate GD Toolkit: Pathfinder retry #{}",
        m_attempts
    );
}

void Pathfinder::verify() {
    log::info(
        "Ultimate GD Toolkit: Pathfinder verification requested"
    );
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
    m_searchDepth =
        std::max<uint32_t>(1, depth);
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

    log::info(
        "Ultimate GD Toolkit: route export requested"
    );

    // Export implementations will be added later.
    return false;
}

// ============================================================
// Ultimate GD Toolkit Mod Menu
//
// Geode 5.x Popup API:
//   - Popup is NOT templated
//   - call Popup::init(width, height)
//   - then use setTitle()
// ============================================================

class ModMenu : public geode::Popup {

protected:

    CCLabelBMFont* m_pageTitle = nullptr;
    CCLabelBMFont* m_content = nullptr;

    enum Tab {
        Macro = 0,
        PathfinderTab = 1,
        FrameTools = 2,
        Playback = 3,
        Routes = 4,
        Settings = 5
    };

    bool init() {

        // Geode 5.x Popup initialization.
        if (!Popup::init(460.f, 300.f))
            return false;

        this->setTitle(
            "Ultimate GD Toolkit"
        );

        auto size =
            this->m_mainLayer->getContentSize();

        // ----------------------------------------------------
        // Page title
        // ----------------------------------------------------

        m_pageTitle =
            CCLabelBMFont::create(
                "SM MACRO",
                "bigFont.fnt"
            );

        m_pageTitle->setScale(0.65f);

        m_pageTitle->setPosition(
            ccp(
                285.f,
                size.height - 42.f
            )
        );

        this->m_mainLayer->addChild(
            m_pageTitle
        );

        // ----------------------------------------------------
        // Content
        // ----------------------------------------------------

        m_content =
            CCLabelBMFont::create(
                "",
                "goldFont.fnt"
            );

        m_content->setAnchorPoint(
            ccp(0.f, 1.f)
        );

        m_content->setScale(0.42f);

        m_content->setPosition(
            ccp(
                145.f,
                size.height - 75.f
            )
        );

        this->m_mainLayer->addChild(
            m_content
        );

        // ----------------------------------------------------
        // Tabs
        // ----------------------------------------------------

        addTab(
            "MACRO",
            Macro,
            0
        );

        addTab(
            "PATHFINDER",
            PathfinderTab,
            1
        );

        addTab(
            "FRAME",
            FrameTools,
            2
        );

        addTab(
            "PLAYBACK",
            Playback,
            3
        );

        addTab(
            "ROUTES",
            Routes,
            4
        );

        addTab(
            "SETTINGS",
            Settings,
            5
        );

        // Start on Macro.
        showTab(Macro);

        return true;
    }

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

    // ========================================================
    // Add tab
    // ========================================================

    void addTab(
        char const* name,
        int tab,
        int index
    ) {

        auto button =
            CCMenuItemSpriteExtra::create(
                ButtonSprite::create(
                    name,
                    "bigFont.fnt",
                    "GJ_button_01.png",
                    0.60f
                ),
                this,
                menu_selector(
                    ModMenu::onTab
                )
            );

        if (!button)
            return;

        button->setTag(tab);

        button->setPosition(
            ccp(
                65.f,
                185.f - index * 40.f
            )
        );

        this->m_buttonMenu->addChild(
            button
        );
    }

    // ========================================================
    // Tab click
    // ========================================================

    void onTab(CCObject* sender) {

        auto item =
            static_cast<CCMenuItem*>(sender);

        showTab(
            item->getTag()
        );
    }

    // ========================================================
    // Show tab
    // ========================================================

    void showTab(int tab) {

        switch (tab) {

            case Macro:
                showMacro();
                break;

            case PathfinderTab:
                showPathfinder();
                break;

            case FrameTools:
                showFrameTools();
                break;

            case Playback:
                showPlayback();
                break;

            case Routes:
                showRoutes();
                break;

            case Settings:
                showSettings();
                break;

            default:
                showMacro();
                break;
        }
    }

    // ========================================================
    // Macro
    // ========================================================

    void showMacro() {

        m_pageTitle->setString(
            "SM MACRO"
        );

        m_content->setString(
            "SM MACRO\n\n"
            "Native format: .sm\n\n"
            "RECORDING\n"
            "Frame-by-frame input recording\n\n"
            "PLAYBACK\n"
            "Frame-accurate input playback\n\n"
            "Events: 0\n"
            "Frames: 0\n\n"
            "Press / Release events"
        );
    }

    // ========================================================
    // Pathfinder
    // ========================================================

    void showPathfinder() {

        m_pageTitle->setString(
            "PATHFINDER"
        );

        std::string text =
            "PATHFINDER\n\n"
            "Status: ";

        text +=
            g_pathfinder.running()
                ? "RUNNING"
                : "STOPPED";

        text +=
            "\n\nSearch Depth: ";

        text += std::to_string(
            g_pathfinder.searchDepth()
        );

        text +=
            "\n\nAttempts: ";

        text += std::to_string(
            g_pathfinder.attempts()
        );

        text +=
            "\n\n"
            "Checkpoints: ON\n"
            "Route Verification: ON\n"
            "State Search: ENABLED";

        m_content->setString(
            text.c_str()
        );
    }

    // ========================================================
    // Frame Tools
    // ========================================================

    void showFrameTools() {

        m_pageTitle->setString(
            "FRAME TOOLS"
        );

        m_content->setString(
            "FRAME TOOLS\n\n"
            "Frame Counter\n\n"
            "Frame Window\n\n"
            "Frame Stepper\n\n"
            "Event Viewer\n\n"
            "Custom Frame Sounds"
        );
    }

    // ========================================================
    // Playback
    // ========================================================

    void showPlayback() {

        m_pageTitle->setString(
            "PLAYBACK"
        );

        m_content->setString(
            "PLAYBACK\n\n"
            "SM Playback\n\n"
            "Frame-accurate input\n\n"
            "Music synchronization\n\n"
            "Press / Release events\n\n"
            "Speed: 1.00x"
        );
    }

    // ========================================================
    // Routes
    // ========================================================

    void showRoutes() {

        m_pageTitle->setString(
            "ROUTE TOOLS"
        );

        m_content->setString(
            "ROUTE TOOLS\n\n"
            "MASTER FORMAT\n"
            ".sm\n\n"
            "PLANNED EXPORTS\n"
            ".gdr\n"
            ".gdr2\n"
            ".echo\n\n"
            "Route verification"
        );
    }

    // ========================================================
    // Settings
    // ========================================================

    void showSettings() {

        m_pageTitle->setString(
            "SETTINGS"
        );

        m_content->setString(
            "SETTINGS\n\n"
            "Mobile UI: ON\n\n"
            "Compact Layout: ON\n\n"
            "Input Visualization: ON\n\n"
            "Sound Effects: ON\n\n"
            "Debug Logging: OFF"
        );
    }
};

// ============================================================
// Open Mod Menu
// ============================================================

void openSMMenu() {

    auto menu =
        ModMenu::create();

    if (menu) {

        menu->show();

        log::info(
            "Ultimate GD Toolkit: Mod Menu opened"
        );
    }
    else {

        log::error(
            "Ultimate GD Toolkit: "
            "failed to create Mod Menu"
        );
    }
}

// ============================================================
// Geometry Dash Main Menu
// ============================================================

class $modify(
    UltimateGDToolkitMenu,
    MenuLayer
) {

    bool init() {

        if (!MenuLayer::init())
            return false;

        auto menu =
            this->getChildByID(
                "bottom-menu"
            );

        if (!menu) {

            log::error(
                "Ultimate GD Toolkit: "
                "bottom-menu not found"
            );

            return true;
        }

        // ----------------------------------------------------
        // Toolkit button
        // ----------------------------------------------------

        if (!menu->getChildByID(
            "ultimate-gd-toolkit-button"_spr
        )) {

            auto sprite =
                ButtonSprite::create(
                    "Toolkit",
                    "bigFont.fnt",
                    "GJ_button_01.png",
                    0.8f
                );

            if (sprite) {

                sprite->setScale(
                    0.55f
                );

                auto button =
                    CCMenuItemSpriteExtra::create(
                        sprite,
                        this,
                        menu_selector(
                            UltimateGDToolkitMenu::
                            onToolkit
                        )
                    );

                if (button) {

                    button->setID(
                        "ultimate-gd-toolkit-button"_spr
                    );

                    menu->addChild(
                        button
                    );
                }
            }
        }

        // ----------------------------------------------------
        // Mod Menu button
        // ----------------------------------------------------

        if (!menu->getChildByID(
            "ultimate-gd-mod-menu-button"_spr
        )) {

            auto sprite =
                ButtonSprite::create(
                    "Mod Menu",
                    "bigFont.fnt",
                    "GJ_button_01.png",
                    0.8f
                );

            if (sprite) {

                sprite->setScale(
                    0.55f
                );

                auto button =
                    CCMenuItemSpriteExtra::create(
                        sprite,
                        this,
                        menu_selector(
                            UltimateGDToolkitMenu::
                            onModMenu
                        )
                    );

                if (button) {

                    button->setID(
                        "ultimate-gd-mod-menu-button"_spr
                    );

                    menu->addChild(
                        button
                    );
                }
            }
        }

        // Let GD arrange the mobile buttons.
        menu->updateLayout();

        log::info(
            "Ultimate GD Toolkit: "
            "Toolkit + Mod Menu buttons loaded"
        );

        return true;
    }

    // --------------------------------------------------------
    // Toolkit button
    // --------------------------------------------------------

    void onToolkit(CCObject*) {

        FLAlertLayer::create(
            "Ultimate GD Toolkit",
            "Toolkit access is ready.",
            "OK"
        )->show();
    }

    // --------------------------------------------------------
    // Mod Menu button
    // --------------------------------------------------------

    void onModMenu(CCObject*) {

        openSMMenu();
    }
};

// ============================================================
// Mod loaded
// ============================================================

$on_mod(Loaded) {

    log::info(
        "========================================"
    );

    log::info(
        "Ultimate GD Toolkit loaded!"
    );

    log::info(
        "Mobile Toolkit button enabled."
    );

    log::info(
        "Mobile Mod Menu button enabled."
    );

    log::info(
        "Tabbed Mod Menu enabled."
    );

    log::info(
        "========================================"
    );
}

}
