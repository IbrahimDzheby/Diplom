/* Copyright 2024 The MathWorks, Inc. */
#ifndef CODER_SCATTEREDINTERP_STRUCTS_H
#define CODER_SCATTEREDINTERP_STRUCTS_H

#include "libqhull_r.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct coderDelaunayTri coderDelaunayTri;
typedef struct coderConvHull coderConvHull;
typedef struct circumCache circumCache;

struct coderDelaunayTri {
    const double* samplePts; // Pointer to sample points (Points to qhdl->first_point, i.e. array of
                             // input points).
    int numPts;              // Number of sample points.
    qhT* qhdl;               // qhT of delaunay triangultaion.
    int ntri;                // Nubmer of simplices of the delaunay triangulation.
    int* connList;           // connectivity list of delaunay triangulation.
    vertexT** vertexHandle;  /* Array of vertexT pointers. Points to each vertex in the delaunay
                                triangulation.*/
    int nVtxPerSimplex;  // Number of vertices per simplex, = dimensionality of input points + 1.
    circumCache* ccache; // Struct storing cicumcenter of each facet.
    facetT* startFacet;  // Starting facet to perform visibility walk.
};

struct coderConvHull {
    int numHullPts;         // Number of points on the convex hull.
    vertexT** vertexHandle; // Array of vertexT pointers. Points to each vertex on the convex hull.
    double centroid[2];     // Centroid of convex hull.
    int* globalHullIndex;   // Index of the vertex point in the input point array.
};

struct circumCache {
    double* ccentrePt;
    unsigned char* ccentreValid;
};

#ifdef __cplusplus
}
#endif

#endif // CODER_SCATTEREDINTERP_STRUCTS_H
