# project_labyrinth
project_labyrinth



# Build 

``` cmake . -Bbuild ``` - build project. 

``` make run ``` - will execute `lab-prim-algo` - that is currently placed in the `test` folder. The output will be `data-prim.json`.   

# Current Tasks

- Session Header - take a point from session
- Lab Point/Map Refactoring [@D-Hatz]
- Player tests 
- Cmake configuration [@D-Hatz]
- In/Out System
- Command / Comand Service
- Player refactoring - remove position. take a point from session



# Naming Conventions

- If struct contains Struct in name it should be handled as struct without any getter or setters
- If struct is syntax-sugar that create via CORE_OBJECT_INTERFACE it should be handled as `Class` 
