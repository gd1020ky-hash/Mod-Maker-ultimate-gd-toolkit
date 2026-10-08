#include "SmMacro.hpp"

#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

namespace sm {

static Pathfinder g_pathfinder;

// ============================================================
// Existing Pathfinder functions
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
    log::info("Ultimate GD Toolkit: Pathfinder retry #{}", m_attempts);
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

    log::info("Ultimate GD Toolkit: export requested");
    return false;
}

// ============================================================
// Main Toolkit menu
// ============================================================

class ToolkitMenu : public geode::Popup<> {
protected:

    CCLabelBMFont* m_title = nullptr;
    CCLabelBMFont* m_pageTitle = nullptr;
    CCLabelBMFont* m_content = nullptr;

    CCMenu* m_tabs = nullptr;

    int m_currentTab = 0;

    enum Tab {
        Macro = 0,
        PathfinderTab = 1,
        FrameTools = 2,
        Playback = 3,
        Routes = 4,
        Settings = 5,
        Themes = 6
    };

    bool setup() override {

        this->setTitle("Ultimate GD Toolkit");

        auto size = this->m_mainLayer->getContentSize();

        // ----------------------------------------------------
        // Left tab panel
        // ----------------------------------------------------

        auto leftBG = CCScale9Sprite::create(
            "square02b_001.png"
        );

        leftBG->setContentSize(
            CCSize(125.f, size.height - 45.f)
        );

        leftBG->setPosition(
            ccp(
                70.f,
                size.height / 2.f
            )
        );

        this->m_mainLayer->addChild(leftBG);

        // ----------------------------------------------------
        // Right content panel
        // ----------------------------------------------------

        auto rightBG = CCScale9Sprite::create(
            "square02b_001.png"
        );

        rightBG->setContentSize(
            CCSize(
                size.width - 155.f,
                size.height - 45.f
            )
        );

        rightBG->setPosition(
            ccp(
                225.f,
                size.height / 2
