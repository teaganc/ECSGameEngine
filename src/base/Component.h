#pragma once

namespace base {

class Entity;
class System;

class Component {
public:
    Component(System* parent) : parent_ (parent) {};
    virtual ~Component() {};

    System* parent_;
    Entity* entity_ = nullptr;
};

};