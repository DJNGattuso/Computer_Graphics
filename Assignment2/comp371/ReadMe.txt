Assignment by Nicholas Gattuso 40007087

Assignment 1 represents a mini version of the pacman game.

I used the skeleton code supplied in class. In addition, I had my friend Chris McArthur and my TA help me with the 
cursor callback function because I was having difficulty getting the change of position to properly apply. 

Please note, that even with the help, my cursor function isn't acting too well. It doesn't seem to be applying the
changes properly. When testing it, you can use the HOME button to reset the orientation in order to prevent rerunning
the code. 

Aside from the cursor callback, my program should run according to the requirements of the assignment.
- Keys A,W,S, and D move pacman. As pacman changes direction, its orientation changes too so that pacman faces in the 
direction its moving in. This can be seen with the mouth - I adjusted the rotation axis so that the mouth can be seen 
better based on which side its facing in.
- Keys T,L and,P change the drawing type
- Keys U and J scale pacman, the ghost and, the food up and down 
- Space repositions pacman randomnly in the grid
- Keys LEFT, RIGHT, UP, DOWM changes the orientation along the x and along the y
- HOME key resets the orientations to the initial setting
- ENTER key restarts the enter game by repositioning the objects and setting a new number of food objects
- Right Click Mouse allows for pan
- Middle Click Mouse allows for tilt
- Left Click Mouse allows for zoom
(Though none of the cursor stuff are working too well)

In addition, I attempted the bonus for having ghost objects chase pacman. I set the teapot objects as the ghost and gave 
them a uniform shader to change colours from dark blue to green. The ghost chase pacman by calculating the difference
between themselves and pacman and chose randomnly to go either to the next y or next x position.

Lastly, the game restarts when either pacman eats all objects or when a ghost gets pacman.