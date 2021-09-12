# M4OEP Graphics: Snake
![snake2](https://user-images.githubusercontent.com/8117699/132993784-d38df4a7-9a10-4396-b5c1-ad4fca8fb277.PNG)

## Table of Contents
  - [Description](#description)
    - [Programs](#programs)
    - [Commands](#programs)
  - [Setup](#setup)
  
## Description
In this project I decided to recreate the classic game "Snake" in which the player is a snake and their goal is to
grab all the pieces of food around the plane. As the snake eats food, it grows by one square in size. In this implementation,
If it makes collision with the walls or itself then you lose one of your three lives. 
### Programs:

* `rect.cpp & shape.cpp` are Lisa's rectangle and shape classes.
* `graphics.cpp` are based on Lisa's graphics setup and modified to create the rectangles representing the snake,
the food, and among other things. 

### Keyboard commands:

*  `Arrow Keys` are used to move the snake around the plane.
* `ESC` is used to quit the game (once you lose the game will quit as well).

## Setup
To run you need Lisa's "Graphics Starter" Clion files (CmakeLists.txt, etc) and you can run it in Clion by cloning this repo with:

``` 
git clone [repo ssh address]
```

By Mateo Riofrio
