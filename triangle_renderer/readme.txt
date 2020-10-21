# University of Leeds 2020-20201

To compile on the University Linux machines, you will need to do the following:

[userid@machine Assignment2MeshDSHandout]$ module add qt/5.13.0
[userid@machine Assignment2MeshDSHandout]$ qmake -project QT+=opengl LIBS+=-lGLU
[userid@machine Assignment2MeshDSHandout]$ qmake
[userid@machine Assignment2MeshDSHandout]$ make
[userid@machine Assignment2MeshDSHandout]$ ./triangle_renderer ../models/tetrahedron.tri

Extended from original to include .face and .diredge files

To run, provide a .tri file. This is converted into a .face
file and saved in the /faces directory. Then this file is read
and converted into a .diredge file and saved in the /diredge file.

The main.cpp has been modified slightly to accept  FaceIndex2DirectedEdge
and Face2faceindex objects instead of GeometricSurfaceFaceDS objects.

The two added files are face2faceindex.cpp and faceindex2directededge.cpp.
These use inheritance from the provided code.
All numbers have been converted to longs to handle large files,
some checks have been done at file read to check that it is valid.
No external libraries have been used and compilation has been checked
on DEC-10 machines.

When compiling on the DEC-10 Feng machines,
instead of qt/5.3.1, only qt/5.13.0 was available when searching
through module avail, so that was used instead. Also in the make
file -std=c++11 flag had to be added to allow for std::to_string.

All questions have been answered, time complexity analysis is available
as a .pdf in the repository.


Thanks,

Jonathan.
