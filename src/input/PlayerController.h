#pragma once 

#include "Controller.h"

#include <functional>
#include <unordered_map>

namespace input {

class PlayerController : public Controller {
public:
    std::unordered_map<ButtonEvent, std::function<void(void)>> button_binds_;
    std::unordered_map<AxisEvent, std::function<void(int)>> axis_binds_;
};

};