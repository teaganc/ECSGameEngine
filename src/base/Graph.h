#pragma once

#include <memory>
#include "Node.h"

namespace base {

class Graph {
public:
    Entity* AddNode(Node* node, std::unique_ptr<Entity> entity);
    void RemoveNode(Node* node);

    int LoadJson(const char* filename);
    
    Node* GetHead() { return head_.get(); }

private:
    std::unique_ptr<Node> head_ = std::make_unique<Node>(nullptr, std::make_unique<Entity>());

};

};