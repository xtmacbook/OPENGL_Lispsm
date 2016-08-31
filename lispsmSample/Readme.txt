LIGHT SPACE PERSPECTIVE SHADOW MAPS
===================================

This programn is a demonstration of the Light Space Perspective Shadow Mapping
algorithm described in the respective paper and the Diploma Thesis by Daniel Scherzer. 
Additional information and sample images are available at 

http://www.cg.tuwien.ac.at/research/vr/lispsm/

Copyright and Disclaimer:

This code is copyright Vienna University of Technology, 2004.


Please feel FREE to COPY and USE the code to include it in your own work, 
provided you include this copyright notice.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 


Authors Code:
Daniel Scherzer (scherzer@cg.tuwien.ac.at)

Authors Paper: 
Michael Wimmer (wimmer@cg.tuwien.ac.at)
Daniel Scherzer (scherzer@cg.tuwien.ac.at)
Werner Purgathofer

Date: March 1, 2006

----------
Input
----------

Help for keybord and mouse is available with F1.
In the light view, the view frustum is shown as a semitransparent body and the
view vector as a red line emanating from the near plane.


----------
The scene
----------

The scene used in this program is designed to show all known shadow mapping problems:
projection aliasing, perspective aliasing and offset problems. 
For instance, self shadowing artefacts at the spiky cones are visible, and offset 
problems can be generated at certain viewing angles towards the hills. 

The LiSPSM algorithm is designed to reduce perspective aliasing only. Projection aliasing
and offset problems appear just like in uniform or perspective shadow maps.

To reduce offset problems we implemented different strategies: shadow map rendering with polygon
offset enabled (slope-scale biasing), back side rendering, const biasing. for switching see the help <F1>.
Hint: Some driver implementations give better results with back side rendering, others with 
polygon offset.

----------
Installation
----------

A binary for Win32 is included.

The program should compile under Windows (Linux doesn't use pbuffers the same way), a CMAKE makefile for multi-platform
compilation is included (see www.cmake.org for details).
For Linux, you need to have a working GLUT and GLEW installation.

----------
Structure
----------

This demo is written in C/C++ and uses OpenGL in order to keep it as universal as possible. 
It has few external needs like limits.h, stdlib.h, math.h, string.h, stdio.h and a 
C++-compiler ;)

It expects the ARB_Shadow extension and uses vertex/pixel shader 2.0.

The program and especially the mathematical routines are not designed for speed or
efficiency, but for simplicity and clarity.

The shadow map size is set to 2048x2048 pixel. As a rule of thumb the shadow map resolution 
should be at least twice the size of the viewport.

If you find any problem with the code or have any comments, we would like to hear from you!

