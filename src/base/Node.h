#pragma once
#include <memory>
#include <vector>

#include "Entity.h"

namespace base {

class Node {
public:
    Node(Node* parent, std::unique_ptr<Entity> entity) : parent_(parent), entity_(std::move(entity)) {};

    Node* parent_;
    std::unique_ptr<Entity> entity_;
    std::vector<std::unique_ptr<Node>> children_;
};

};