# Animations

Animations are concieved as some sort of function which can update a transform based on a previous transform the total time for which the program has been running and the time difference between the previous update and this one.

The `src/Animation.cppm` module defines two basic animations 
 1. Translate at a constant rate.
 2. Rotate at a constant rate.

As well as various higher order animation constrcuts such as
 1. A combination of several animations.
 2. A limit to the duration of an animation.
 3. A sequence of animations each performed one after the other.
 4. A loop of animations which behaves similarly to a sequence except that the animation is reset to a known state before repeating the animation.

There are overloads to binary operators to more easily compose animations.
 1. A logical and `&` of two or more animations creates a combination.
 2. A logical and `&` of an animation and a duration creates a limited time animation.
 3. A logical or `|` of two or more time limited animations creates a sequence of animations.