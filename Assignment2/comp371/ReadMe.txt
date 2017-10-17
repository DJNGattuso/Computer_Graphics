Assignment by Nicholas Gattuso 40007087

Assignment 2 represents an image heightmap using point interpolation with Catmull Rom splines. In this program, the 
depth.bmp image is read using CImg. From there, each pixel of the image is accessed to fetch its colour value. A height
map is then created to represent the image.

Please note that there are 2 things that aren't functioning properly. First is drawing the points with triangles. For some
reason, my points change their order (they aren't just respective column and row). I believe it has to do either with how
the points were stored in my vector or with the way I'm keeping track of the width and height (this can cause the algorithm
to set a point that's not in the correct spot). Second, my catmull rom isn't giving me exactly what I'm looking for. My
step 4 (make catmull rom along the x) seems to be working well but it's giving me 3 lines that are out of place. My step
5 (catmull rom along the z) just isn't working well. There's something about my points or the way I'm accessing them that
is causing a weird behaviour.

Aside from that, the program is running as it should. I get the original image, the skip size image and, the two catmull 
rom (even though they aren't 100% perfect). Additionally, I completed the bonus that tracks the original map and can output
the original with the catmull rom interpolation. I have keyboard control on each step and can constantly change the step 
and skip size.

The following is the controls for the keyboard:
--For moving the camera--
A : movement in the -x
D : movement in the +x
W : movement in the -z
S : movement in the +z
U : movement in the +y
J : movement in the -y
--For changing the steps--
Z : step 2 (original)
X : step 3 (skip size)
C : step 4 (catmull rom along x)
V : step 5 (catmull rom along z) 
--For changing the environment--
T		: to draw in triangles (only works in 'Z' mode since I couldn't generate it well)
I		: to retake the user inputs
BACKSPACE	: reset the camera position, goes back to 'Z' and ask for user inputs
ESC		: close the window