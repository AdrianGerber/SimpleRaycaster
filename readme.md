# Simple Raycaster
This is my first attempt at creating a 3D game using the 2D framework SFML.
Most of the code is just there to allow you to load environments and walk around in them. The Raycaster class is used to cast rays using a method similar to Wolfenstein 3D, creating a sort of depth buffer in the process. This buffer is then sent to one of the rendering classes, which interpret the data in different ways and draw the picture to the screen using SFML primitives. There are 3 classes that can be used for rendering:
-   LineRenderer: Draws a line to the screen for each ray that was sent out. This line is scaled according to the distance traveled by the ray, to create an illusion of depth. This mode supports textures, by calculating the appropriate texture coordinates from the ray's 'subblock'-position at the moment it hit a wall. The resulting lines are also faded to black with increasing distance.
-   MapRenderer: This variant uses the information to render a topdown view of the world. The camera's field of view is shown using red lines, with the endpoint of each ray marked by a red square. Very useful for debugging raycasting issues.
-   PolygonRenderer: Attempts to draw continuous segments of wall using polygons. Performance is much better when compared with LineRenderer, as there are much less 'things' to draw (Many rays can be drawn as a single polygon at once). This mode does not support textures, as they can not easily be distorted using SFML-methods in a way that simulates perspective. The used polygon cannot be configured to display a color gradient, which means there can't be any 'distance-fade'. The PolygonRenderer class also struggles with correctly (not) rendering objects that are outside the raycaster's viewdistance.

## Building
To compile this code, you will need to link against the SFML library and provide the corresponding .dll inside the .exe's directory. You also need to set the copmiler's mode to c++2017, as the code uses std::optional.

## Keybindings
The following keys can be used to control the demo:
-   W: Move forward
-   S: Move backward
-   A: Move left
-   D: Move right
-   E: Cycle between the different Rendermodes
-   Q: Hold Q to freely move the mouse

You can control the camera's orientation by moving the mouse.

## Configuration files
Make sure all of these files are accessible to the program. When executing the software form inside Visual Studio, the files will need to be inside the Renderer3D folder. If the .exe is ran directly, then they need to be present in the .exe's directory!
### game.conf
You can configure some attributes of the engine, like the window's resolution, inside this file.
### testmap.mapdef
This file declares tiles that can be used inside the loaded .map file. Each tile is defined by it's name (1 character long), the color of each of it's sides, and it's texture. A texture should be a square .png or .jpg image. If the texture is empty or cannot be loaded, only the color will be used to draw this tile. The names of every tile that will be defined needs to be listed as the value of MAPTILES. You can also limit the maximum distance a ray will travel by adjusting PLAYER_VIEWDISTANCE. Smaller values will improve performance, expecially on large maps.
### testmap.map
Inside the .map file, the map can be drawn using all the tiles defined in the .mapdef file. A 'P' character can be used to set the player's entry point, ' ' means walkable floor. Unknown letters will be rendererd as a white wall. The map should be a rectangle of any size, fully surrounded by walls.