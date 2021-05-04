#pragma once 

#include "base/Component.h"
#include "base/System.h"
#include "Controller.h"

namespace input {

class Component : public base::Component {
public:
    Component(base::System* parent, std::unique_ptr<Controller> controller) : base::Component(parent), controller_(std::move(controller)) {}
    
    std::unique_ptr<Controller> controller_;
};

};