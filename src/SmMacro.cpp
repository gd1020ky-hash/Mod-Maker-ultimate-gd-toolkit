#include "SmMacro.hpp"

using namespace geode::prelude;

namespace sm {

static Pathfinder g_pathfinder;

void Pathfinder::start() {
    m_running = true;
}

void Pathfinder::stop() {
    m_running = false;
}

void Pathfinder::retry() {
    ++m_attempts;
}

void Pathfinder::verify() {
    log::info("Pathfinder verification requested");
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

bool RouteExporter::exportRoute(
    Route const& route,
    ExportFormat format,
    std::string const& path
) {
    (void)route;
    (void)format;
    (void)path;

    log::info("Route exporter requested");

    return false;
}

void openSMMenu() {
    log::info("Ultimate GD Toolkit menu requested");
}

$on_mod(Loaded) {
    log::info("Ultimate GD Toolkit loaded");
}

}
