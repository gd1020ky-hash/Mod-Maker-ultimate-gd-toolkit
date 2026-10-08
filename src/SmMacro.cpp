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
    m_searchDepth = std::max<uint32_t>(1, depth);

    log::info(
        "Ultimate GD Toolkit: Pathfinder search depth = {}",
        m_searchDepth
    );
}

// ============================================================
// Route exporter
// ============================================================

bool RouteExporter::exportRoute(
    Route const& route,
    ExportFormat format,
    std::string const& path
) {
    // Export formats will be implemented later.
    // Do NOT pretend these are valid exports yet.
    (void)route;
    (void)format;
    (void)path;

    log::info(
        "Ultimate GD Toolkit: route export requested"
    );

    return false;
}

// ============================================================
// Toolkit popup
// ============================================================

void openSMMenu() {
    FLAlertLayer::create(
        "Ultimate GD Toolkit",

        "<cy>SM Macro</c>\n"
        "Record / Playback\n\n"

        "<cy>Frame Tools</c>\n"
        "Frame counter / frame window\n\n"

        "<cy>Pathfinder</c>\n"
        "Search / Retry / Verify\n\n"

        "<cy>Route Tools</c>\n"
        "SM / GDR / GDR2 / ECHO\n\n"

        "<d>More Ultimate GD Toolkit features "
        "are coming soon.</d>",

        "OK"
    )->show();
}

// ============================================================
// Mobile main-menu button
// ============================================================

class $modify(UltimateGDToolkitMenu, MenuLayer) {

    bool init() {
        if (!MenuLayer::init())
            return false;

        // Find Geometry Dash's existing bottom menu.
        auto menu = this->getChildByID("bottom-menu");

        if (!menu) {
            log::error(
                "Ultimate GD Toolkit: bottom-menu not found"
            );

            return true;
        }

        // Prevent duplicate buttons if the menu is initialized
        // more than once.
        if (menu->getChildByID("ultimate-gd-toolkit-button"_spr)) {
            return true;
        }

        // Create the button.
        auto sprite = ButtonSprite::create(
            "Toolkit",
            "bigFont.fnt",
            "GJ_button_01.png",
            0.8f
        );

        if (!sprite) {
            log::error(
                "Ultimate GD Toolkit: failed to create button sprite"
            );

            return true;
        }

        // Make it compact enough for the mobile menu.
        sprite->setScale(0.55f);

        auto button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(UltimateGDToolkitMenu::onToolkitButton)
        );

        if (!button) {
            log::error(
                "Ultimate GD Toolkit: failed to create menu button"
            );

            return true;
        }

        // Give the button a Geode namespaced ID.
        button->setID("ultimate-gd-toolkit-button"_spr);

        // Add it to Geometry Dash's existing mobile menu.
        menu->addChild(button);

        // Let the existing layout automatically position it.
        menu->updateLayout();

        log::info(
            "Ultimate GD Toolkit: mobile Toolkit button added"
        );

        return true;
    }

    void onToolkitButton(CCObject*) {
        log::info(
            "Ultimate GD Toolkit: button pressed"
        );

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
        "========================================"
    );
}

}
