// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      triangle.c
//      Purpose :   Triangles
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "alt_graphics_module.h"
#include "alt_graphics_module_local.h"

#define HORIZLINE(x1,x2,y)      { GFXRawMove(x1,y);GFXDrawLine(x1,y,x2,y,true); }
#define ANYLINE(x1,y1,x2,y2)    { GFXRawMove(x1,y1);GFXDrawLine(x1,y1,x2,y2,true); }

typedef struct _Vertice {
    int32_t x,y;
} Vertice;

static Vertice vt1,vt2,vt3;

#define TOFIXED(n)          ((n) << 16)
#define FROMFIXED(n)        (((n)+0x8000) >> 16)

/**
 * @brief      Sort the triangle vertices in ascending Y order
 */
static void sortVerticesAscendingByY(void)
{
    Vertice     vTmp;

    if (vt1.y > vt2.y) {
        vTmp = vt1;
        vt1 = vt2;
        vt2 = vTmp;
    }
    /* here v1.y <= v2.y */
    if (vt1.y > vt3.y) {
        vTmp = vt1;
        vt1 = vt3;
        vt3 = vTmp;
    }
    /* here v1.y <= v2.y and v1.y <= v3.y so test v2 vs. v3 */
    if (vt2.y > vt3.y) {
        vTmp = vt2;
        vt2 = vt3;
        vt3 = vTmp;
    }
}

/**
 * @brief      Flat bottom part of the triangle
 *
 * @param[in]  v1    The v1 vertex
 * @param[in]  v2    The v2 vertex
 * @param[in]  v3    The v3 vertex
 */
static void fillBottomFlatTriangle(Vertice v1,Vertice v2,Vertice v3) {

    int invslope1 = (v2.y == v1.y) ? 0 : TOFIXED(v2.x - v1.x)  / (v2.y - v1.y);
    int invslope2 = (v3.y == v1.y) ? 0 : TOFIXED(v3.x - v1.x)  / (v3.y - v1.y);

    int curx1 = TOFIXED(v1.x);
    int curx2 = TOFIXED(v1.x);

    for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++) {
        HORIZLINE(FROMFIXED(curx1),FROMFIXED(curx2),scanlineY);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}


/**
 * @brief      Flat top part of the triangle
 *
 * @param[in]  v1    The v1 vertex
 * @param[in]  v2    The v2 vertex
 * @param[in]  v3    The v3 vertex
 */
static void fillTopFlatTriangle(Vertice v1,Vertice v2,Vertice v3)
{
    int invslope1 = (v3.y == v1.y) ? 0 : TOFIXED(v3.x - v1.x) / (v3.y - v1.y);
    int invslope2 = (v3.y == v2.y) ? 0 : TOFIXED(v3.x - v2.x) / (v3.y - v2.y);

    int curx1 = TOFIXED(v3.x);
    int curx2 = TOFIXED(v3.x);

    for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--) {
        HORIZLINE(FROMFIXED(curx1),FROMFIXED(curx2),scanlineY);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

/**
 * @brief      Draw a solid filled triangle
 */
static void drawFilledTriangle() {
    /* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
    sortVerticesAscendingByY();

    /* here we know that v1.y <= v2.y <= v3.y */
    /* check for trivial case of bottom-flat triangle */
    if (vt2.y == vt3.y) {
        fillBottomFlatTriangle(vt1, vt2, vt3);
    }
    /* check for trivial case of top-flat triangle */
    else if (vt1.y == vt2.y) {
        fillTopFlatTriangle(vt1, vt2, vt3);
    } else {
        /* general case - split the triangle in a topflat and bottom-flat one */
        Vertice vt4;
        vt4.x =  (int)(vt1.x + ((float)(vt2.y - vt1.y) / (float)(vt3.y - vt1.y)) * (vt3.x - vt1.x));
        vt4.y = vt2.y;
        fillBottomFlatTriangle(vt1, vt2, vt4);
        fillTopFlatTriangle(vt2, vt4, vt3);
    }
}

/**
 * @brief      Draw a filled triangle
 *
 * @param[in]  x0    The x0 coordinate
 * @param[in]  y0    The y0 coordinate
 * @param[in]  x1    The x1 coordinate
 * @param[in]  y1    The y1 coordinate
 * @param[in]  x2    The x2 coordinate
 * @param[in]  y2    The y2 coordinate
 */

void GFXDrawFilledTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1,int32_t x2,int32_t y2) {
    vt1.x = x0;vt1.y = y0;
    vt2.x = x1;vt2.y = y1;
    vt3.x = x2;vt3.y = y2;
    drawFilledTriangle();
}

/**
 * @brief      Draw an outline triangle
 *
 * @param[in]  x0    The x0 coordinate
 * @param[in]  y0    The y0 coordinate
 * @param[in]  x1    The x1 coordinate
 * @param[in]  y1    The y1 coordinate
 * @param[in]  x2    The x2 coordinate
 * @param[in]  y2    The y2 coordinate
 */
void GFXDrawOutlineTriangle(int32_t x0,int32_t y0,int32_t x1,int32_t y1,int32_t x2,int32_t y2) {
    ANYLINE(x0,y0,x1,y1);
    ANYLINE(x0,y0,x2,y2);
    ANYLINE(x2,y2,x1,y1);
}

// 