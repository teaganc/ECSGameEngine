#pragma once

namespace base {

class Engine; 

class Input {
public:
    Input(Engine* engine) : engine_(engine) {};
    int Poll();

private:
    Engine* engine_;
};
};