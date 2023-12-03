
# Implementation of the frustum culling algorithm

The frustum culling algorithm provides for checking whether an object falls into a truncated viewing frustum
and discarding those objects that do not fall into this frustum
, that is, such objects are not even
transferred to OpenGL and their vertices are not transformed by the vertex shader.


## Demonstration of frustum culling

![App Screenshot](https://github.com/nvmediagithub/Frustum_Culling/raw/main/doc/demonstration.gif)


## Tech Stack

**Graphics Library:** OpenGL 3.3

**Other Library:** GLM, FreeGlut, GLEW, rapidjson-master

