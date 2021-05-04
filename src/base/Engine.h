#pragma once
#include <algorithm>
#include <memory>

#include "Graph.h"
#include "Input.h"
#include "System.h"
#include "Window.h"

namespace base {

class Engine {
public:
    bool is_running;

    Engine();

    int Start(int width, int height);
    int End();

    int Execute();

    Window* GetWindow() { return window_.get(); }

    Graph* GetGraph() {return graph_.get(); }

    void AddSystem(std::unique_ptr<System> system);

    template <std::derived_from<System> T> 
    T* GetSystem() {
        auto item = std::ranges::find_if(systems_, [] (auto& i) { 
            return dynamic_cast<T*>(i.get()) != nullptr; });
        if (item != systems_.end()) {
            return static_cast<T*>(item->get());
        }
        return nullptr;
    }


private:
    std::vector<std::unique_ptr<System>> systems_;
    std::unique_ptr<Window> window_;
    std::unique_ptr<Graph> graph_;

    uint32_t last_tick;
};

};