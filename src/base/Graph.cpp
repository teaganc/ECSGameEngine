#include "Graph.h"

#include <algorithm>

namespace base {

Entity* Graph::AddNode(Node* parent, std::unique_ptr<Entity> entity) {
    parent->children_.push_back(std::make_unique<Node>(parent, std::move(entity)));
    return parent->children_.back().get()->entity_.get();
}

void Graph::RemoveNode(Node* node) {
    auto& vec = node->parent_->children_;
    auto same_as_unique = [node] (auto& i) { return i.get() == node; };
    vec.erase(std::remove_if(vec.begin(), vec.end(), same_as_unique), vec.end());
}

int Graph::LoadJson(const char* filename) {

    return 0;
}

};