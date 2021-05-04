# ECS Game Engine

A small 3D entity component system engine built using C++20. It supports model loading and rendering with OpenGL. It support physics through physx. 

## Quickstart

To get started create an engine and call Start(). Call AddSystem() to add any systems you want to your engine. It is recommended to start with a render, physics and input system. An example can be seen in main.cpp. Then get started by creating some entities by calling CreateNode() in the engines Graph. This will create and add an entity to the scene graph of the object. Now you can AddComponent() to the entity. The engine comes with factory functions in the Systems to generate Components for you. Next call Engine::Execute() to begin looping. Once this exits call Engine::End() to clean it all up.

To begin extending the engine, start by making a class that inherits from base::Component. This will hold your data for each entity. Next make a class that inherits from base::System which will do the processing of your components. Add your system to your engine and attach it's component to any entity you like. Note that when adding an component to an entity it will register the component with its respective system.  


## Building 

This project uses physx 4.0 so it is likely you will have to build it yourself from source. It also depends on SDL2 and glew so install those from your favorite repository. Ensure that your physx libraries exist in the linking path or specify their directory in the makefile. 

Once you have done this simply make and use make run to run the project. 
