# University of Leeds 2020-20201

To compile on the University Linux machines, you will need to do the following:

[userid@machine Assignment2MeshDSHandout]$ module add qt/5.3.1
[userid@machine Assignment2MeshDSHandout]$ qmake -project QT+=opengl LIBS+=-lGLU
[userid@machine Assignment2MeshDSHandout]$ qmake
[userid@machine Assignment2MeshDSHandout]$ make
[userid@machine Assignment2MeshDSHandout]$ ./Assignment2MeshDSHandout ../models/tetrahedron.tri

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

All questions have been answered, time complexity analysis is available
as a .pdf in the repository.


Thanks,

Jonathan.
