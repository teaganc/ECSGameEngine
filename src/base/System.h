#pragma once

#include "Component.h"

namespace base {

class System {
public:
    virtual void RegisterComponent(Component* Component) = 0;
    virtual void UnRegisterComponent(Component* Component) = 0;

    virtual int Start() = 0;
    virtual int End() = 0;
    virtual int Update(double delta_time) = 0;
private:

};

};