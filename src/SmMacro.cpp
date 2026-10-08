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
        "Ultimate GD Toolkit: Pathfinder depth = {}",
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
    // Export formats are not implemented yet.
    // The native .sm format remains the master format.
    (void)route;
    (void)format;
    (void)path;

    log::info(
        "Ultimate GD Toolkit: route export requested"
    );

    return false;
}

// ============================================================
// Ultimate GD Toolkit Mod Menu
// ============================================================

class ModMenu : public geode::Popup<> {

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

    bool setup() override {

        this->setTitle("Ultimate GD Toolkit");

        auto size = this->m_mainLayer->getContentSize();

        // ----------------------------------------------------
        // Page title
        // ----------------------------------------------------

        m_pageTitle = CCLabelBMFont::create(
            "SM MACRO",
            "bigFont.fnt"
        );

        m_pageTitle->setScale(0.65f);

        m_pageTitle->setPosition(
            ccp(
                size.width / 2.f + 55.f,
                size.height - 48.f
            )
        );

        this->m_mainLayer->addChild(
            m_pageTitle
        );

        // ----------------------------------------------------
        // Content
        // ----------------------------------------------------

        m_content = CCLabelBMFont::create(
            "",
            "goldFont.fnt"
        );

        m_content->setAnchorPoint(
            ccp(0.f, 1.f)
        );

        m_content->setScale(0.42f);

        m_content->setPosition(
            ccp(
                125.f,
                size.height - 82.f
            )
        );

        this->m_mainLayer
            
