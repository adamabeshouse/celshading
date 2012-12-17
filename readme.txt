Celshading

by Adam Abeshouse and Zach Salmon

--------------
Design Decisions
---------------

Our project was cartoon rendering (including a toon shader and inky outlines), and cartoon fire and rain. The general theme was 'lost in the woods.'

To do cartoon shading, we render into a framebuffer object, bucketizing "the normals" (really, the dot product of the normal and the light vectors). This gives a sharp shading effect.
To do the outlines, we render into another framebuffer object, perform a sobel edge detect, and then subtract that from the other fbo, thus getting a toon-shaded scene with inky black outlines.
To do the fire, we create a particle emitter with only upwards velocity. Since we pretty much know what we want the shape of the fire to be, we describe its trajectory using a radius function, rather than using meandering random walks in the x and z directions. It decays proportionally to 1/y as the y coordinate of each fire particle rises. This also affects its scale.
To do the rain, we create a particle emitter with only downwards velocity. Each drop is a blue sphere that has been cartoon loaded. However we found that it looked better without the outlines, so we removed them.

To build the scene, we use a skybox for the nighttime sky, and some brush and grass. We also built a mesh loader and built a model of a tree in Wings3D that we use. We modified the obj format a little bit for ease of use (we don't handle mtl's or o's, and just have v's, vn's, f's, and specifications of color for a section which are in the form kd r g b and ka r g b).

We chose a yellow lego man because it is a cool shape that highlights the edge detection and cartoony-looking effects.

---------------
Bugs
---------------
None that we know of.


---------------
Things to remark on
---------------
We modified the camera scroll functionality so that it moves a little more smoothly.

---------------
Memory leaks
---------------

It's hard to say whether or not we have memory leaks becuse, as with other projects, the qt interface leaks some memory of its own. It seems to us that we are freeing everything we new, and our memory leaks hover around 4000, which is what the support code from the labs we used code from leak. 

Additional problems were that, at the precise time we were looking at memory leaks, the cit filesystem crashed. This was, admittedly, monday very early morning. But the last time we checked in valgrind, memory leaks were around the same as the support code.
