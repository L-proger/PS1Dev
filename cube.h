#define _WCHAR_T

#include <stdlib.h>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBETC.H>
#include <stdio.h>



class Cube {
public:
    static constexpr short CubeSize = 128;
    void Construct();

    void render(u_long* orderTable);

    SVECTOR _rotation;
    VECTOR  _translation;
private:
    POLY_F4 _primitives[6];
    MATRIX  _matrix;
};