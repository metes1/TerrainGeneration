Team Members:
Seda Mete
Christina Mudarth


How to Run Program
--------------------
The program is run using the makefile located in the directory. Type "make" to run the program.

Controls
------------
- Left-Arrow   => Rotate scene left on y-axis
- Right-Arrow  => Rotate scene right on y-axis
- Up-Arrow Key => Rotate scene on x-axis
- Down-Arrow   => Rotate scene on x-axis
- W            => Toggle wireframe mode
- L            => Toggle lighting
- O            => Toggle between flat and Gouraud shading
- S            => Toggle polygon mesh (quads/triangles)
- T            => Change terrain texture
- P            => Display 2D terrain overview
- X            => Switch terrain generation algorithm
- 1            => Switch to Light 1 Control
- 2            => Switch to Light 2 Control
- C/B          => Move light left/right
- F/V          => Move light up/down
- R            => Reset: generate new terrain
- q/ESC        => Quit program

Additional Features
----------------------
1) 2D terrain overview: When the 'P' key is pressed a new window is opened, which displays a 2D overhead view of the terrain.

2) Additional terrain algorithm: When the 'X' key is pressed a new terrain is generated using a different algorithm. The default algorithm is the circles algorithm, the additional one is the fault algorithm.

References
------------
1) http://www.lighthouse3d.com/opengl/terrain/index.php

Both circles and fault algorithms were referenced from this site.

2) http://www.it.hiof.no/~borres/j3d/explain/light/p-materials.html

The material float values were taken from this site.
