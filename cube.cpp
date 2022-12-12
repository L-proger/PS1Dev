#define _WCHAR_T

#include "cube.h"
#include "config.h"


static SVECTOR cubeCorners[8]{
    SVECTOR {-Cube::CubeSize/2,-Cube::CubeSize/2,-Cube::CubeSize/2,0},
    SVECTOR { Cube::CubeSize/2,-Cube::CubeSize/2,-Cube::CubeSize/2,0},
    SVECTOR { Cube::CubeSize/2, Cube::CubeSize/2,-Cube::CubeSize/2,0},
    SVECTOR {-Cube::CubeSize/2, Cube::CubeSize/2,-Cube::CubeSize/2,0},
    SVECTOR {-Cube::CubeSize/2,-Cube::CubeSize/2, Cube::CubeSize/2,0},
    SVECTOR { Cube::CubeSize/2,-Cube::CubeSize/2, Cube::CubeSize/2,0},
    SVECTOR { Cube::CubeSize/2, Cube::CubeSize/2, Cube::CubeSize/2,0},
    SVECTOR {-Cube::CubeSize/2, Cube::CubeSize/2, Cube::CubeSize/2,0}
};


static const SVECTOR cubeNormals[6]{
    SVECTOR { ONE,   0,    0, 0,},
    SVECTOR {-ONE,   0,    0, 0,},
    SVECTOR {0,    ONE,    0, 0,},
    SVECTOR {0,   -ONE,    0, 0,},
    SVECTOR {0,      0,  ONE, 0,},
    SVECTOR {0,      0, -ONE, 0,}
};



Cube::Cube() {
    printf("Cube constructor\n");
    CVECTOR colors[6] = {
        {255,   0,      0,      0},
        {0,     255,    0,      0},
        {0,     0,      255,    0},
        {255,   0,      255,    0},
        {255,   255,    0,      0},
        {0,     255,    255,    0}
    };

    // initialize prim properties
    for (int i=0; i < 6; ++i)
    {
        SetPolyF4(&_primitives[i]);
        setRGB0(&_primitives[i], colors[i].r, colors[i].g, colors[i].b);
    }

    setVector(&_rotation, 0, 0, 0);
    setVector(&_translation, 5, 0, 512);
}

void Cube::render(u_long* orderTable){
    PushMatrix();

    RotMatrix(&_rotation, &_matrix);
    TransMatrix(&_matrix, &_translation);

    SetRotMatrix(&_matrix);
    SetTransMatrix(&_matrix);

    u_char vertexIndices[24] = {
        0,1,2,3,
        5,4,7,6,
        1,5,6,2,
        4,0,3,7,
        4,5,1,0,
        6,7,3,2
    };

    long p;
    long orderTableZ;
    long flag;
    //draw each face
    for(int i = 0; i < 6; ++i){
        int vFaceOffset = i << 2;

        long faceNormal = RotAverageNclip4(
            &cubeCorners[vertexIndices[vFaceOffset + 0]], &cubeCorners[vertexIndices[vFaceOffset + 1]], &cubeCorners[vertexIndices[vFaceOffset + 2]], &cubeCorners[vertexIndices[vFaceOffset + 3]],
            (long*)&(_primitives[i].x0), (long*)&(_primitives[i].x1), (long*)&(_primitives[i].x2), (long*)&(_primitives[i].x3), &p, &orderTableZ, &flag);

        if (faceNormal <= 0){
            continue;
        }

        if (orderTableZ > 0 && orderTableZ < ORDER_TABLE_SIZE) {
            AddPrim(orderTable + orderTableZ, &_primitives[i]);
        }
    }

    PopMatrix();
}
