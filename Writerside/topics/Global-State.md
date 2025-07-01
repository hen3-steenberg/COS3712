# Global State

There are some actions which can be initiated either from a menu or as a key event.
And there is some actions which has repercussions for multiple other systems.

For these cases the appropriate functions to manipulate and access limited shared state is exposed from the `src/GlobalState.cppm` module;

This includes:
 1. `windowRef` which required both to display to the screen as well as recieve input from the keyboard and mouse.
 2. `CurrentCameraMode` which can be updated from both a menu action and a key event.
 3. `cameraPosition` which required to update the camera as well as draw the scene.
 4. `AnimateFirst2Drones` which has to be updated from user input and used when animating the drones.
 5. `AnimateCars` which has to be updated from user input and used to decide wether to animate any cars.

There is also a utility function `toggleKey` which only triggers an action again if the key was released between presses.