/* Copyright 2024-2025 The MathWorks, Inc. */

#include <stdio.h>
#include <stdlib.h>

#include "qhull_ra.h"
#include "mem_r.h"
#include "qset_r.h"

#include "poly_r.h"

#include "scatteredInterp_qhull_util.h"
#include "qhTWrapper.h"

/*-------------------Defined in geom_utils-------------------------------*/
void vectorSubtraction(const double* a, const double* b, double* vec);
void vectorRotate90Clockwise(double* v);
double vectorLengthSquare(const double* v);
void normalizeVector(double* v, const double LOCAL_TOL);
double vectorDistanceSquare(const double* v1, const double* v2);
void computeOuterProd(const double* nnVec, double* outerProd);
double crossProd(const double* v1, const double* v2, const double* p1);
unsigned int leftTurn(const double* p, const double* q, const double* r);
double dotProd(const double* v1, const double* v2);
void triIncenter(const double* a,
                 const double* b,
                 const double* c,
                 double* incenter,
                 double* inRadius);
void circumcentre(const double* x1, const double* x2, const double* x3, double* cc);
unsigned int isInsideTriangle(const double triVtx[3][2], const double* qp);
unsigned int incircle(const double* v1, const double* v2, const double* v3, const double* qp);
double signedArea(const double* x1, const double* x2, const double* x3);
/*-----------------------------------------------------------------------*/


enum locationType {
    VERTEX = 0,             // query is exactly, ==, on a vertex
    EDGE = 1,               // query lies on on of the edges of the simplex, with some tolerance
    FACET = 2,              // query lies inside the smiplex, but isn't on the vertex or edge
    OUTSIDE_CONVEX_HULL = 3 // query lies outside the convex hull of the triangulation
    };
typedef enum locationType locationTypeT;

// Outcome of a natural neighbor coordinate computation
enum naturalNeighborOutcome {
    NATURAL_NBR_FAILED_OUTSIDE = 0,
    NATURAL_NBR_FAILED_BOUNDARY = 1,
    NATURAL_NBR_FAILED_INSIDE = 2,
    NATURAL_NBR_OK = 3
    };
typedef enum naturalNeighborOutcome naturalNeighborOutcomeT;


void getVtxIDsOfSimplex(void** dtStructPtr, const int sid, int* vxID) {
    coderDelaunayTri* dtStruct = (coderDelaunayTri*)(*dtStructPtr);
    const int nVtxPerSimplex = 3;
    for (int k = 0; k < 3; k++) {
        vxID[k] = dtStruct->connList[sid * nVtxPerSimplex + k] +
            1; // Adding 1 to convert to MATLAB indexing.
    }
}

void cacheCircumcentres(coderDelaunayTri* dtStruct);

static int runQhull(qhT* qh, double* pts, int numDims, int numPts, boolT ismalloc) {

    int exitcode = setjmp(qh->errexit);
    if (exitcode) {
        return exitcode;
    }

    qh->NOerrexit = False;
    qh_initflags(qh, qh->qhull_command);
    if (qh->DELAUNAY) {
        qh->PROJECTdelaunay = True;
    }

    qh_init_B(qh, pts, numPts, numDims, ismalloc);
    qh_qhull(qh);
    qh_check_output(qh);

    if (qh->VORONOI) {
        qh_clearcenters(qh, qh_ASvoronoi);
        qh_vertexneighbors(qh);
    }
    if (qh->TRIangulate) {
        qh_triangulate(qh);
        if (qh->VERIFYoutput && !qh->CHECKfrequently) {
            qh_checkpolygon(qh, qh->facet_list);
        }
    }

    qh->NOerrexit = True;
    return 0;
}

int numDelaunayTris(qhT* qh) {
    int numTri = 0;
    facetT* facet;
    FORALLfacets {
        if (!facet->upperdelaunay) {
            numTri++;
        }
    }
    return numTri;
}

int connectivityList(qhT* qh, const int numTri, int* connList) {

    int i = 0, j;
    vertexT* vertex;
    vertexT** vertexp;
    facetT* facet;
    const int delDim = 3; // Number of vertices in simplex.
    FORALLfacets {
        if (!facet->upperdelaunay) {
            j = 0;
            if (i >= numTri) {
                return -1;
            }
            FOREACHvertex_(facet->vertices) {
                if (j >= delDim) {
                    return -2;
                }
                connList[delDim * i + j] = qh_pointid(qh, vertex->point);
                j++;
            }
            facet->id = i;
            i++;
        }
    }
    return 0;
}

void centroidOfHull2D(vertexT** vertexHandle, const int numVtx, double* centroid) {
    double xc = 0;
    double yc = 0;
    // The centroid is calculated only for the vertices of the convex hull,
    // not for the polygon that it forms.
    for (int i = 0; i < numVtx; i++) {
        const vertexT* currVx = vertexHandle[i];
        xc += currVx->point[0];
        yc += currVx->point[1];
    }
    xc /= numVtx;
    yc /= numVtx;

    centroid[0] = xc;
    centroid[1] = yc;
}

void cacheDelaunayTriVertexHandles(coderDelaunayTri* dtStruct, const int nVtxs) {

    // Store the vertices of the delaunay triangulation in input order.
    vertexT** vertexHandle = malloc(nVtxs * sizeof(vertexT*));
    dtStruct->vertexHandle = vertexHandle;

    qhT* qh = dtStruct->qhdl;
    vertexT* vertex;
    boolT atInfinity = qh->ATinfinity;
    int qhTnumPoints = qh->num_points;
    FORALLvertices {
        int vid = qh_pointid(qh, vertex->point);
        if (atInfinity && vid == (qhTnumPoints - 1)) {
            // Ignore point at infinity inserted by Qhull
            // when 'Qz' option is passed.
        } else {
            vertexHandle[vid] = vertex;
        }
    }
}

int hullVerticesOfDelaunayTriangulation(qhT* qh, const int numPts, coderConvHull* chStruct) {

    // Set the neighbors for each vertex.
    qh_vertexneighbors(qh); // No-op if its already set during construction.

    int numHullFacets = 0;
    vertexT** ccwHull = malloc(numPts * sizeof(vertexT*));
    memset(ccwHull, 0, numPts);
    facetT* facet;
    vertexT* startVtx = NULL;

    // Number of vertices on the convex hull is equal to total number of edges
    // between an upperdelaunay facet and lower delaunay facet.
    FORALLfacets {
        if (facet->upperdelaunay) {
            continue;
        }
        int numNeighbor = qh_setsize(qh, facet->neighbors);
        for (int i = 0; i < numNeighbor; i++) {
            facetT* neighbor = SETelemt_(facet->neighbors, i, facetT);
            if (neighbor->upperdelaunay) {
                // This is an edge between a upperdelaunay facet and a "lower" facet.
                // This edge will be part of the convex hull
                // for this triangulation.
                numHullFacets++;

                vertexT* vertexB = SETelemt_(facet->vertices, (i + 1) % numNeighbor, vertexT);
                vertexT* vertexC = SETelemt_(facet->vertices, (i + 2) % numNeighbor, vertexT);
                // Rely on vertex ordering determined by qhull.
                if (!facet->toporient) {
                    // A,B,C are in clockwise order
                    ccwHull[qh_pointid(qh, vertexC->point)] = vertexB;
                    if (!startVtx) {
                        startVtx = vertexC;
                    }
                } else {
                    // A,B,C are in ccw order
                    ccwHull[qh_pointid(qh, vertexB->point)] = vertexC;
                    if (!startVtx) {
                        startVtx = vertexB;
                    }
                }
            }
        }
    }

    vertexT** vertexHandles = malloc(numHullFacets * sizeof(vertexT*));
    int* globalHullId = malloc(numHullFacets * sizeof(int));
    vertexT* vertex = startVtx;
    int vtxCtr = 0;
    do {
        vertexT* nextVtxInCCW = ccwHull[qh_pointid(qh, vertex->point)];
        vertexHandles[vtxCtr] = nextVtxInCCW;
        globalHullId[vtxCtr++] = qh_pointid(qh, nextVtxInCCW->point);
        vertex = nextVtxInCCW;
    } while (vertex != startVtx && vtxCtr < numHullFacets);

    chStruct->vertexHandle = vertexHandles;
    chStruct->numHullPts = numHullFacets;
    chStruct->globalHullIndex = globalHullId;
    free(ccwHull);

    return numHullFacets;
}

int computeConvexHullOfDelaunayTri(void** dtStructPtr, void** chStructPtr, int* numHullVertices) {

    coderDelaunayTri* dtStruct = (coderDelaunayTri*)(*dtStructPtr);
    coderConvHull* chStruct = (coderConvHull*)(*chStructPtr);

    // Get the vertices on the hull of the triangulation
    *numHullVertices =
        hullVerticesOfDelaunayTriangulation(dtStruct->qhdl, dtStruct->numPts, chStruct);
    // Calculate centroid for the hull.
    centroidOfHull2D(chStruct->vertexHandle, chStruct->numHullPts, chStruct->centroid);

    return 0;
}

int computeTriangulationForConvexHull(void** chStructPtr,
                                      void** chTriStructPtr,
                                      const char* options) {

    coderConvHull* chStruct = (coderConvHull*)(*chStructPtr);

    int ptCtr = 0;
    vertexT** vtxHandle = chStruct->vertexHandle;
    int numHullPts = chStruct->numHullPts;
    double* convHullPts = malloc(2 * numHullPts * sizeof(double));
    for (int i = 0; i < numHullPts; i++) {
        vertexT* currVtx = vtxHandle[i];
        convHullPts[ptCtr++] = currVtx->point[0]; // x-coord
        convHullPts[ptCtr++] = currVtx->point[1]; // y-coord
    }

    int err = computeDelaunayTri(chTriStructPtr, convHullPts, 2, numHullPts, options);

    if (err > 0) {
        free(convHullPts);
        return err;
    }

    coderDelaunayTri* chTriStruct = (coderDelaunayTri*)(*chTriStructPtr);
    cacheDelaunayTriVertexHandles(chTriStruct, chTriStruct->numPts);

    free(convHullPts);

    return err;
}

int computeDelaunayTri(void** dtStructPtr,
                       double* pts,
                       int numDims,
                       int numPts,
                       const char* options) {

    coderDelaunayTri* dtStruct = (coderDelaunayTri*)(*dtStructPtr);

    dtStruct->numPts = numPts;

    // Allocate memory for qhull struct storing the delaunay triangulation.
    qhT* qh = malloc(sizeof(qhT));
    dtStruct->qhdl = qh;

    qh_init_mwerrmsg();
    qh_meminit(qh, stderr);
    qh_initqhull_start(qh, stdin, stdout, stderr);
    strcpy(qh->qhull_command, "qhull d ");
    strcat(qh->qhull_command, options);

    int err = runQhull(qh, pts, numDims, numPts, False);
    if (err > 0) {
        int curlong, totlong;
        qh_freeqhull(qh, False);
        qh_memfreeshort(qh, &curlong, &totlong);
        if (curlong || totlong) {
            return -1;
        }
        free(qh);
        dtStruct->qhdl = NULL;
        return err;
    }
    // Load facet neighbors for each vertex.
    // This will be used in linear extrapolation, dsearch.
    qh_vertexneighbors(qh);

    // Get number of triangles in delaunay
    int numTri = numDelaunayTris(dtStruct->qhdl);
    dtStruct->ntri = numTri;

    dtStruct->connList = malloc(3 * numTri * sizeof(int));
    err = connectivityList(dtStruct->qhdl, numTri, dtStruct->connList);
    dtStruct->nVtxPerSimplex = 3; // Constant, as we support only 2D inputs.
    dtStruct->ccache = NULL;
    dtStruct->vertexHandle = NULL;

    dtStruct->samplePts = qh->first_point; // Sample points, has dimensionality qh.hull_dim
    dtStruct->startFacet = NULL; // Pick the first good/non-upperdelaunay facet for tsearch.

    return err;
}

int getLocationType(facetT* facet, const double* qp, locationTypeT* locType) {
    // Set the location type for the query point w.r.t to the given facet.
    double triVtx[3][2];
    vertexT* vertex;
    vertexT** vertexp;
    int i = 0;
    FOREACHvertex_(facet->vertices) {
        triVtx[i][0] = vertex->point[0];
        triVtx[i][1] = vertex->point[1];
        if ((qp[0] == triVtx[i][0]) && (qp[1] == triVtx[i][1])) {
            *locType = VERTEX; // Exact match with a vertex, set type and return.
            return i;
        }
        i++;
    }

    const double myeps = DBL_EPSILON * 3;
    for (int k = 0; k < 3; k++) {
        double c1 = crossProd(&triVtx[(k + 1) % 3][0], &triVtx[(k + 2) % 3][0], qp);
        if (fabs(c1) < myeps) {
            // The point is considered as on/inside facet for tolerance of
            // 'myeps'
            *locType = EDGE;
            return k;
        }
        double c2 = crossProd(&triVtx[(k + 1) % 3][0], &triVtx[(k + 2) % 3][0], &triVtx[k][0]);

        if ((c1 > 0.) == (c2 > 0.)) {
            continue;
        } else {
            // Query point and vertex lie on different half-planes created
            // by the edge. The query isn't inside or on this simplex.
            *locType = OUTSIDE_CONVEX_HULL;
            return -1;
        }
    }
    // Query point is interior of this simplex
    *locType = FACET;
    return -1;
}

unsigned int isInsideFacet(facetT* facet, const double* qp) {
    // Helper for brute-force tsearch
    double triVtx[3][2];
    vertexT* vertex;
    vertexT** vertexp;
    int i = 0;
    FOREACHvertex_(facet->vertices) {
        triVtx[i][0] = vertex->point[0];
        triVtx[i][1] = vertex->point[1];
        i++;
    }
    return isInsideTriangle(triVtx, qp);
}

int bruteForceTsearch(qhT* qh, const double* qp, facetT** facetHandle) {
    int sid = -1;
    facetT* facet;
    FORALLfacets {
        if (facet->upperdelaunay) {
            continue;
        }
        if (isInsideFacet(facet, qp)) {
            if (sid < (int)facet->id) {
                sid = (int)facet->id;
            }
            if (facetHandle) {
                *facetHandle =
                    (!(*facetHandle) || (*facetHandle)->id < facet->id) ? facet : *facetHandle;
            }
        }
    }
    return sid;
}

int nextSimplex(const int* connList,
                const int id,
                const double* pts,
                const double* qp,
                const int ntri,
                const int npts,
                unsigned int* onEdge) {

    *onEdge = 0;
    int edgeId = -1;
    const int nVtxPerSimplex = 3;
    int k;
    double triVtx[3][2]; // Vertices of triangle

    for (k = 0; k < nVtxPerSimplex; k++) {
        int kthVtx = connList[id * nVtxPerSimplex + k];
        int offset =
            kthVtx * nVtxPerSimplex; // The sample points are stored as hull_dim dimensional points.
        triVtx[k][0] = pts[offset];
        triVtx[k][1] = pts[offset + 1];
    }

    // Using the tolerance from tsearchn.m
    const double myeps = DBL_EPSILON * 3;

    for (k = 0; k < nVtxPerSimplex; k++) {
        double c1 = crossProd(&triVtx[(k + 1) % 3][0], &triVtx[(k + 2) % 3][0], qp);
        if (fabs(c1) < myeps) {
            // This is on the line containing the edge.
            // Three cases can occur here,
            // 1) It is one of the vertices defining the edge.
            // 2) It lies between the two vertices i.e on the edge.
            // 3) It lies outside the edge but on the line containing the edge.
            *onEdge = 1;
            edgeId = k;
            continue;
        }
        double c2 = crossProd(&triVtx[(k + 1) % 3][0], &triVtx[(k + 2) % 3][0], &triVtx[k][0]);
        if ((c1 > 0.) == (c2 > 0.)) {
            // Query point and vertex lie on same side of edge.
            continue;
        } else { // Query point and vertex lie on opposite side of edge.

            // For case 3 above, onEdge would have been set, reset it to 0
            // as the query point is not part of this simplex.
            *onEdge = 0;
            return k;
        }
    }
    // Was on same side for all vertex-edge pairs. The point is inside this
    // simplex or on edge.
    // If inside return -1, else return the id of the vertex opposite to the
    // edge the query point is on.
    return (*onEdge) ? edgeId : -1; // Probably can return edgeId without checking for onEdge.
}

int visibilityWalk(qhT* qh,
                   const double* qp,
                   const int* connList,
                   const int ntri,
                   const double* pts,
                   const int npts,
                   facetT** facetHandle) {

    facetT* facet = NULL;
    facetT* facetNeighbor = NULL;

    if (facetHandle && *facetHandle) {
        // We have a "random" starting simplex.
        facet = *facetHandle;
        if (facet->id >= (unsigned int)ntri || facet->id < 0) { // Invalid starting facet
            facet = qh->facet_list;
            while (facet->upperdelaunay) {
                facet = facet->next;
            }
        }
    } else {
        // Start searching from first non upper-delaunay simplex.
        facet = qh->facet_list;
        while (facet->upperdelaunay) {
            facet = facet->next;
        }
    }

    if (facetHandle) {
        *facetHandle = facet;
    }

    unsigned int onEdge = 0;
    // Resort to brute force search if we don't find the simplex in ntri/4 steps.
    int numSteps = 1 + (ntri / 4);
    for (int i = 0; i < numSteps; i++) {

        int ns = nextSimplex(connList, facet->id, pts, qp, ntri, npts, &onEdge);

        if (onEdge) {
            // Query point is either a vertex or on edge of simplex.
            // No special casing for query exactly on vertex as we check for it when solving for
            // barycentric coords Have to check if special casing is needed for natural neighbour
            // interpolation.
            facetNeighbor = SETelemt_(facet->neighbors, ns, facetT);
            if (facetNeighbor->upperdelaunay) {
                // The neighbor is not part of the delaunay triangulation
                // i.e. edge is on hull return current simplex
                if (facetHandle) {
                    *facetHandle = facet;
                }
                return facet->id;
            } else {
                if (facetHandle) {
                    *facetHandle = (facet->id > facetNeighbor->id) ? facet : facetNeighbor;
                }
                return (facet->id > facetNeighbor->id) ? facet->id : facetNeighbor->id;
            }
        } else if (ns == -1) {
            // Query point is inside current simplex.
            if (facetHandle) {
                *facetHandle = facet;
            }
            return facet->id;
        } else {
            // Go to the ns'th neighbor.
            if (facetHandle) {
                *facetHandle = facet;
            }
            facet = SETelemt_(facet->neighbors, ns, facetT);
            if (facet->upperdelaunay) {
                // The neighbor is not part of the delaunay triangulation
                // i.e. point is outside convex hull.
                return -1;
            }
        }
    }

    // Failed to converge, resort to brute-force.
    if (facetHandle) {
        *facetHandle = NULL;
    }
    return bruteForceTsearch(qh, qp, facetHandle);
}

int tsearch(void** dtStructPtr, const double* qp) {

    coderDelaunayTri* dtStruct = (coderDelaunayTri*)(*dtStructPtr);

    return visibilityWalk(dtStruct->qhdl, qp, dtStruct->connList, dtStruct->ntri,
                          dtStruct->samplePts, dtStruct->numPts, &dtStruct->startFacet);
}

unsigned int isCircumTriangle(const qhT* qhdl,
                              const facetT* triHandle,
                              const double* qp,
                              const circumCache* ccache) {
    // Check if 'triHandle' is a circumtriangle of query point.
    if (!ccache->ccentreValid[triHandle->id]) {
        // degenerate triangle
        return 1;
    }

    vertexT* vertexA = SETelemt_(triHandle->vertices, 0, vertexT);
    vertexT* vertexB = SETelemt_(triHandle->vertices, 1, vertexT);
    vertexT* vertexC = SETelemt_(triHandle->vertices, 2, vertexT);

    unsigned int inC;
    double det = crossProd(vertexA->point, vertexB->point, vertexC->point);
    if (det < 0) {
        // A,B,C are in clockwise order, swap vertex B and vertex C.
        inC =  incircle(vertexA->point, vertexC->point, vertexB->point, qp);
    } else {
        // A,B,C are in ccw order
        inC = incircle(vertexA->point, vertexB->point, vertexC->point, qp);
    }
    return inC;
}

void recurseForNearestVertex(qhT* qh,
                             const circumCache* ccache,
                             const double* qp,
                             facetT* facet,
                             vertexT** nearestVertex,
                             double* distToNear) {

    facet->visitid = qh->visit_id;
    if (facet->upperdelaunay || !isCircumTriangle(qh, facet, qp, ccache)) {
        return;
    }

    vertexT* vertex;
    vertexT** vertexp;
    int i = 0;
    FOREACHvertex_(facet->vertices) {
        double dist = vectorDistanceSquare(vertex->point, qp);
        if (*nearestVertex) {
            if (dist < *distToNear) {
                *nearestVertex = vertex;
                *distToNear = dist;
            } else if (dist > *distToNear) {
                // no-op
            } else {
                // equal, we don't set tolerance explicitly we wait for >,<
                // ops to fail.
                int cid = qh_pointid(qh, vertex->point);
                int nid = qh_pointid(qh, (*nearestVertex)->point);
                if (cid > nid) {
                    // Tie-break return point with larger index.
                    *nearestVertex = vertex;
                }
            }
        } else {
            *nearestVertex = vertex;
            *distToNear = dist;
        }

        facetT* facetNeighbor = SETelemt_(facet->neighbors, i, facetT);
        if (facetNeighbor->visitid != qh->visit_id) {
            recurseForNearestVertex(qh, ccache, qp, facetNeighbor, nearestVertex, distToNear);
        }
        i++;
    }
}

void nearestFromOutsideHull(qhT* qh,
                            const double* pts,
                            const int* connList,
                            const double* qp,
                            facetT* facetHandle,
                            vertexT** nearestVtx,
                            double* distToNear) {

    if (facetHandle->visitid == qh->visit_id || facetHandle->upperdelaunay) {
        return; // Already visited facet, or not part of the triangulation, return.
    }

    facetHandle->visitid = qh->visit_id; // mark as visited.

    int id = facetHandle->id;
    const int nVtxPerSimplex = 3;
    int k;
    double triVtx[3][2]; // Vertices of triangle

    // Get the vertices of the current simplex.
    for (k = 0; k < nVtxPerSimplex; k++) {
        int kthVtx = connList[id * nVtxPerSimplex + k];
        int offset =
            kthVtx * nVtxPerSimplex; // The sample points are stored as hull_dim dimensional points.
        triVtx[k][0] = pts[offset];
        triVtx[k][1] = pts[offset + 1];
    }

    facetT* neighbor;
    facetT** neighborp;
    vertexT* vertex;

    for (k = 0; k < nVtxPerSimplex; k++) {

        // Check if this vertex is closer than current-nearest vertex.
        double dist = vectorDistanceSquare(triVtx[k], qp);
        if (*nearestVtx) {
            if (dist < *distToNear) {
                // Get vertex handle for this vertex.
                vertex = SETelemt_(facetHandle->vertices, k, vertexT);
                *nearestVtx = vertex;
                *distToNear = dist;
            } else if (dist > *distToNear) {
                // no-op
            } else {
                // Return vertex with larger vertexID.
                vertex = SETelemt_(facetHandle->vertices, k, vertexT);
                int cid = qh_pointid(qh, vertex->point);
                int nid = qh_pointid(qh, (*nearestVtx)->point);
                if (cid > nid) {
                    *nearestVtx = vertex;
                    // This vertex was at same distance as previous nearest vertex,
                    // Iterate over its neighbors before moving on.
                    FOREACHneighbor_(vertex) {
                        if (neighbor->visitid != qh->visit_id) {
                            nearestFromOutsideHull(qh, pts, connList, qp, neighbor, nearestVtx,
                                                   distToNear);
                        }
                    }
                }
            }
        } else {
            vertex = SETelemt_(facetHandle->vertices, k, vertexT);
            *nearestVtx = vertex;
            *distToNear = dist;
        }
    }

    // Visit neighboring facets of nearest vertex to see if it has any closer vertices.
    vertex = *nearestVtx;
    FOREACHneighbor_(vertex) {
        if (neighbor->visitid != qh->visit_id) {
            nearestFromOutsideHull(qh, pts, connList, qp, neighbor, nearestVtx, distToNear);
        }
    }
}

int dsearchQueryOutsideHull(qhT* qh, const double* qp) {
    // TO DO: This can definitely be optimized, we know it is outside convex hull,
    // the projection on paraboloid will have to be above the hull.
    // Doing a brute force for first pass submission.
    vertexT* nearestVtx = NULL;
    double distToNear;
    vertexT* vertex;
    boolT atInfinity = qh->ATinfinity;
    int qhTnumPoints = qh->num_points;
    FORALLvertices {
        if (atInfinity && qh_pointid(qh, vertex->point) == (qhTnumPoints - 1)) {
            // This is the point 'at infinity' added by Qhull
            // when 'Qz' option is used. Ignore this point.
            continue;
        }
        double dist = vectorDistanceSquare(vertex->point, qp);
        if (nearestVtx) {
            if (dist < distToNear) {
                nearestVtx = vertex;
                distToNear = dist;
            } else if (dist > distToNear) {
                // no-op
            } else {
                int cid = qh_pointid(qh, vertex->point);
                int nid = qh_pointid(qh, nearestVtx->point);
                if (cid > nid) {
                    nearestVtx = vertex;
                }
            }
        } else {
            nearestVtx = vertex;
            distToNear = dist;
        }
    }
    return qh_pointid(qh, nearestVtx->point) + 1;
}

int dsearch(void** dtStructPtr, const double* qp, int* isInsideHull) {

    coderDelaunayTri* dtStruct = (coderDelaunayTri*)(*dtStructPtr);

    qhT* qh = dtStruct->qhdl;
    int sid = visibilityWalk(qh, qp, dtStruct->connList, dtStruct->ntri, dtStruct->samplePts,
                             dtStruct->numPts, &dtStruct->startFacet);

    // startFacet contains the 'last valid' facet encountered during the walk.
    // Use that as seed facet to perform dsearch.
    facetT* facetHandle = dtStruct->startFacet;

    *isInsideHull = !(sid == -1);

    cacheCircumcentres(dtStruct);

    double distToNear;
    vertexT* nearestVtx = NULL;

    if (*isInsideHull) {
        qh->visit_id++;
        recurseForNearestVertex(qh, dtStruct->ccache, qp, facetHandle, &nearestVtx, &distToNear);
        return qh_pointid(qh, nearestVtx->point) + 1;
    } else {
        // Test and enable better search on points outside hull
        // qh->visit_id++;
        // nearestFromOutsideHull(qh, dtStruct->samplePts, dtStruct->connList, qp, facetHandle,
        // &nearestVtx, &distToNear); return qh_pointid(qh, nearestVtx->point) + 1;
        return dsearchQueryOutsideHull(qh, qp);
    }
}

void maxIncentreRadius(const int vxId, vertexT** chVtxHandles, double* maxICRad) {
    // Find the simplex with the largest incentre radius,
    // of which this convex hull vertex is a part.
    double simplexCoords[3][2];
    double triIctr[2];

    *maxICRad = 0;

    const vertexT* vh = chVtxHandles[vxId];

    facetT* facet;
    facetT** facetp;
    vertexT* vertex;
    vertexT** vertexp;
    // Iterate over all facets that contain this vertex
    FOREACHfacet_(vh->neighbors) {
        int i = 0;
        if (facet->upperdelaunay) {
            continue;
        }
        // Get the vertices of this facet
        FOREACHvertex_(facet->vertices) {
            simplexCoords[i][0] = vertex->point[0];
            simplexCoords[i][1] = vertex->point[1];
            i++;
        }
        double icRadius = 0;
        // Compute tricenter radius and compare
        triIncenter(simplexCoords[0], simplexCoords[1], simplexCoords[2], triIctr, &icRadius);
        if (icRadius > *maxICRad) {
            *maxICRad = icRadius;
        }
    }
}

void normalAtHullVertex(const int vxId,
                        const int ccwId,
                        const int cwId,
                        vertexT** vertexHandle,
                        double* nrmlVec) {
    // Calculate the normal at a vertex on the convex hull.

    double inVec[2];
    double outVec[2];
    vectorSubtraction(vertexHandle[cwId]->point, vertexHandle[vxId]->point, inVec);
    vectorSubtraction(vertexHandle[vxId]->point, vertexHandle[ccwId]->point, outVec);

    vectorRotate90Clockwise(inVec);
    vectorRotate90Clockwise(outVec);

    // Local tolerance to check if the hull vertices are very close to each other,
    // used to prevent div by 0 errors.
    double LOCAL_TOL =
        (fabs(vertexHandle[cwId]->point[0]) + 1) > (fabs(vertexHandle[cwId]->point[1]) + 1)
        ? (fabs(vertexHandle[cwId]->point[0]) + 1)
        : (fabs(vertexHandle[cwId]->point[1]) + 1);
    LOCAL_TOL *= 10 * DBL_EPSILON;

    normalizeVector(inVec, LOCAL_TOL);
    normalizeVector(outVec, LOCAL_TOL);

    // We will treat the sum of the two vectors as the normal at this convhull
    // vertex.
    nrmlVec[0] = inVec[0] + outVec[0];
    nrmlVec[1] = inVec[1] + outVec[1];
    normalizeVector(nrmlVec, LOCAL_TOL);
}

int sumNNWeightedValues(const int* nnIds,
                        const int nn,
                        const double* phi,
                        const double* sampleVals,
                        const int numPts,
                        const int kthFuncVal,
                        double* valueAtSite) {
    *valueAtSite = 0.;
    for (int i = 0; i < nn; i++) {
        int vtxId = nnIds[i];
        double nnWeight = phi[vtxId];
        double valAtNeighbor = sampleVals[vtxId + numPts * kthFuncVal];
        // TODO error checking for non-finite weights.
        *valueAtSite += nnWeight * valAtNeighbor;
    }
    return 1;
}

void samplingSiteForGradient(const int vxId,
                             const int ccwId,
                             const int cwId,
                             vertexT** chVtxHandles,
                             const double* hullCentroid,
                             double* samplePt) {
    // Calculate the sampling site to be used to calculate
    // gradient at a vertex point on the convex hull. This is done because gradient
    // calculation breaks down at convex hull boundary.

    double vxNormal[2];
    normalAtHullVertex(vxId, ccwId, cwId, chVtxHandles, vxNormal);

    if (vectorLengthSquare(vxNormal) == 0) { // Okay to have absolute compare
        // tolerance was used to compute the normal vector
        // This branch is excercised when the hull vertices are
        // very close together locally. In which case we use the length of
        // vector from hull centroid to vertex as the inward offset.
        vectorSubtraction(chVtxHandles[vxId]->point, hullCentroid, vxNormal);
        vxNormal[0] *= 0.01;
        vxNormal[1] *= 0.01;
    } else {
        double distToInwardSite = 0.;
        maxIncentreRadius(vxId, chVtxHandles, &distToInwardSite);

        if (distToInwardSite < 100 * DBL_EPSILON) {
            // The incentre radius is very small i.e. the simplices that contain
            // this vertex are small. Fallback to using the distance between
            // hull centroid and vertex on hull.
            double maxVec[2];
            vectorSubtraction(chVtxHandles[vxId]->point, hullCentroid, maxVec);
            distToInwardSite = 0.1 * sqrt(vectorLengthSquare(maxVec));
        }
        // Use a fraction of distance to incentre as the offset
        distToInwardSite *= -0.1;
        vxNormal[0] *= distToInwardSite;
        vxNormal[1] *= distToInwardSite;
    }
    // Offset the hull vertex by the 'vxNormal' vector to get the sampling
    // site.
    samplePt[0] = chVtxHandles[vxId]->point[0];
    samplePt[1] = chVtxHandles[vxId]->point[1];
    samplePt[0] += vxNormal[0];
    samplePt[1] += vxNormal[1];
}

double computeAreaOfInfluence(const double influenceRegion[][2], const unsigned int numInfluences) {

    double areaOfInfluence = 0;
    if (numInfluences < 3) {
        return areaOfInfluence;
    }

    double interiorPt[2] = {0};
    for (unsigned int i = 0; i < numInfluences; i++) {
        interiorPt[0] += influenceRegion[i][0];
        interiorPt[1] += influenceRegion[i][1];
    }
    interiorPt[0] /= numInfluences;
    interiorPt[1] /= numInfluences;

    for (unsigned int i = 0; i < numInfluences; i++) {
        areaOfInfluence += fabs(
            signedArea(interiorPt, influenceRegion[i], influenceRegion[((i + 1) % numInfluences)]));
    }
    return areaOfInfluence;
}

unsigned int vertexInFacet(const vertexT* vtx, const facetT* facet) {
    // Check if 'facet' contains the vertex vtx.
    vertexT* vertex;
    vertexT** vertexp;
    FOREACHvertex_(facet->vertices) {
        if (vertex == vtx) {
            return 1;
        }
    }
    return 0;
}

facetT* nextFacetWithVertex(facetT* currFacet, const vertexT* vertex) {
    // Returns the next facet in ccw order to which 'vertex' belongs.
    // returns NULL if currFacet doesn't contain the input vertex.
    vertexT* vertexA = SETelemt_(currFacet->vertices, 0, vertexT);
    vertexT* vertexB = SETelemt_(currFacet->vertices, 1, vertexT);
    vertexT* vertexC = SETelemt_(currFacet->vertices, 2, vertexT);

    unsigned int neighborId;

    if (vertexA == vertex) {
        if (!currFacet->toporient) {
            // A,B,C are in clockwise order, the next facet in ccw order is
            // opposite C.
            neighborId = 2;
        } else {
            neighborId = 1;
        }
    } else if (vertexB == vertex) {
        if (!currFacet->toporient) {
            neighborId = 0;
        } else {
            neighborId = 2;
        }
    } else if (vertexC == vertex) {
        if (!currFacet->toporient) {
            neighborId = 1;
        } else {
            neighborId = 0;
        }
    } else {
        return NULL;
    }

    return SETelemt_(currFacet->neighbors, neighborId, facetT);
}

// C implementation to mimic stack like behavior, given an array pointer sp.
#define push(sp, n) (*((sp)++) = (n))
#define pop(sp) (*(--sp))
#define top(sp) (*(sp - 1))

naturalNeighborOutcomeT computeNaturalNeighborCoords(coderDelaunayTri* dtStruct,
                                                     const double* qp,
                                                     facetT** stackPtr,
                                                     int* nnIds,
                                                     int* nn,
                                                     double* phi) {
    facetT* currFacet = NULL;
    qhT* qhdl = dtStruct->qhdl;

    int sid = visibilityWalk(dtStruct->qhdl, qp, dtStruct->connList, dtStruct->ntri,
                             dtStruct->samplePts, dtStruct->numPts, &currFacet);

    locationTypeT locType;
    int locIdx;
    if (sid == -1) {
        // Point is outside convex hull of triangulation, return and send to
        // appropiate extrapolation method.
        locType = OUTSIDE_CONVEX_HULL;
        locIdx = -1;
        return NATURAL_NBR_FAILED_OUTSIDE;
    } else {
        // Check if query is a vertex, on edge, or interior of this simplex
        locIdx = getLocationType(currFacet, qp, &locType);
    }
    if (locType == VERTEX) {
        // Query point matches ones of the vertices exactly, i.e. query point
        // is same as one of the sample points.
        vertexT* vertex = SETelemt_(currFacet->vertices, locIdx, vertexT);
        int globalVertexID = qh_pointid(qhdl, vertex->point);
        nnIds[(*nn)++] = globalVertexID;
        phi[globalVertexID] = 1.;
        return NATURAL_NBR_OK;
    }

    if (locType == EDGE) {
        // Query point lies on edge of this simplex
        facetT* neighbor = SETelemt_(currFacet->neighbors, locIdx, facetT);
        if (neighbor->upperdelaunay && (!currFacet->upperdelaunay)) {
            // The edge is on the boundary, fallback to linear interpolation.
            return NATURAL_NBR_FAILED_BOUNDARY;
        }
        if (neighbor->id > currFacet->id) {
            // Swap facets, if neighbor has larger index.
            facetT* tmpFacet = currFacet;
            currFacet = neighbor;
            neighbor = tmpFacet;
        }
        // we have landed on an internal edge, proceed with the computation
    }

    // locType == FACET, the point is interior of the simplex.
    const circumCache* ccache = dtStruct->ccache;

    // Get the edges that form the boundary of conflicts for the query point,
    // in counter-clockwise order.
    qhdl->visit_id++;
    vertexT* startVtx = NULL;
    unsigned int numConflicts = 0;

    facetT** spBottom = stackPtr;

    push(stackPtr, currFacet);

    // Pointer to next vertex in CCW order, if the the vertex is part of the
    // conflict boundary, else uninitialized.
    vertexT** ccwBoundary = malloc(dtStruct->numPts * sizeof(vertexT*));
    // Pointer to facet to which the vertex belongs.
    facetT** facetOfVtx = malloc(dtStruct->numPts * sizeof(facetT*));

    while (stackPtr - spBottom) {

        currFacet = pop(stackPtr);
        if (currFacet->upperdelaunay || currFacet->visitid == qhdl->visit_id) {
            continue;
        }
        currFacet->visitid = qhdl->visit_id;
        // Iterate over the neighbors of the triangle
        int numNeighbor = qh_setsize(qhdl, currFacet->neighbors);
        for (int i = 0; i < numNeighbor; i++) {
            facetT* neighbor = SETelemt_(currFacet->neighbors, i, facetT);

            // if the query point doesn't lie in the circumcircle of the
            // neighboring triangle, the shared edge is part of the boundary
            unsigned int isConflict = (!(neighbor->upperdelaunay) &&
                                       isCircumTriangle(qhdl, neighbor, qp, dtStruct->ccache));
            
            if (isConflict) {
                push(stackPtr, neighbor);
            } else {
                // This edge will be part of the boundary of conflicts
                numConflicts++;
                // Reorder the vertices to be in ccw order.
                vertexT* vertexA = SETelemt_(currFacet->vertices, i, vertexT);
                vertexT* vertexB = SETelemt_(currFacet->vertices, (i + 1) % numNeighbor, vertexT);
                vertexT* vertexC = SETelemt_(currFacet->vertices, (i + 2) % numNeighbor, vertexT);
                double det = crossProd(vertexA->point, vertexB->point, vertexC->point);

                flagT isCCW = det > 0;
                if(currFacet->toporient != isCCW) {
                    // Inconsistency in orientation determined by qhull and expected
                    // orientation.
                    // Warn and fallback to linear interpolation.
                    free(ccwBoundary);
                    free(facetOfVtx);
                    return NATURAL_NBR_FAILED_INSIDE;
                }

                if (!currFacet->toporient) {
                    // A,B,C are in clockwise order
                    ccwBoundary[qh_pointid(qhdl, vertexC->point)] = vertexB;
                    if (!startVtx) {
                        startVtx = vertexC;
                    }
                    facetOfVtx[qh_pointid(qhdl, vertexC->point)] = currFacet;
                } else {
                    // A,B,C are in ccw order
                    ccwBoundary[qh_pointid(qhdl, vertexB->point)] = vertexC;
                    if (!startVtx) {
                        startVtx = vertexB;
                    }
                    facetOfVtx[qh_pointid(qhdl, vertexB->point)] = currFacet;
                }
            }
        }
    }

    // Traverse the boundary and store the handles and global indices of the
    // vertex.
    vertexT** conflictHandles = malloc(numConflicts * sizeof(vertexT*));
    facetT** conflictFacet = malloc(numConflicts * sizeof(facetT*));
    unsigned int* globalVtxId = malloc(numConflicts * sizeof(unsigned int));
    vertexT* vertex = startVtx;
    unsigned int vtxCtr = 0;

    do {
        vertexT* nextVtxInCCW = ccwBoundary[qh_pointid(qhdl, vertex->point)];
        conflictHandles[vtxCtr] = nextVtxInCCW;
        globalVtxId[vtxCtr] = qh_pointid(qhdl, nextVtxInCCW->point);
        conflictFacet[vtxCtr++] = facetOfVtx[qh_pointid(qhdl, nextVtxInCCW->point)];
        vertex = nextVtxInCCW;
    } while (vertex != startVtx && vtxCtr < numConflicts);

    // Free allocated memory.
    free(ccwBoundary);
    free(facetOfVtx);

    double totalArea = 0;
    vertexT* prevVtx = conflictHandles[numConflicts - 1];
    facetT* prevFacet = conflictFacet[numConflicts - 1];
    double(*influenceRegion)[2] = malloc(2 * (numConflicts + 3) * sizeof(double));
    unsigned int numInfluences = 0;

    // Find the weight of each natural neighbor.
    for (unsigned int ii = 0; ii < numConflicts; ii++) {
        numInfluences = 0;

        vertexT* thisVtx = conflictHandles[ii];
        facetT* thisFacet = conflictFacet[ii];

        vertexT* nextVtx = conflictHandles[(ii + 1) % numConflicts];
        double newVoronoiCentre[2];
        circumcentre(thisVtx->point, nextVtx->point, qp, newVoronoiCentre);

        influenceRegion[numInfluences][0] = newVoronoiCentre[0];
        influenceRegion[numInfluences][1] = newVoronoiCentre[1];
        numInfluences++;

        if (prevFacet == thisFacet) {

            if (ccache->ccentreValid[thisFacet->id]) {
                // Only one facet in the boundary of conflicts, add the circumcentre of the
                // facet
                influenceRegion[numInfluences][0] = ccache->ccentrePt[2 * thisFacet->id];
                influenceRegion[numInfluences][1] = ccache->ccentrePt[2 * thisFacet->id + 1];
                numInfluences++;
            }

        } else {

            // The facet to which 'thisVtx' belongs. Search for all facets,
            // to which 'thisVtx' belongs to, starting here.
            currFacet = conflictFacet[ii];
            facetT* stFacet = currFacet;

            // Get all the triangles the current vertex is a part of,
            // in ccw order.
            do {

                if (ccache->ccentreValid[currFacet->id]) {
                    influenceRegion[numInfluences][0] = ccache->ccentrePt[2 * currFacet->id];
                    influenceRegion[numInfluences][1] = ccache->ccentrePt[2 * currFacet->id + 1];
                    numInfluences++;
                }
                currFacet = nextFacetWithVertex(currFacet, thisVtx);

            } while ((currFacet != stFacet) && (currFacet != prevFacet));
            // Process the last one, this will be the facet to which the previous vertex belongs
            // i.e. currFacet is equal to prevFacet.
            if (ccache->ccentreValid[currFacet->id]) {
                influenceRegion[numInfluences][0] = ccache->ccentrePt[2 * currFacet->id];
                influenceRegion[numInfluences][1] = ccache->ccentrePt[2 * currFacet->id + 1];
                numInfluences++;
            }
        }

        double prevVtxNewVoronoiCentre[2];
        circumcentre(prevVtx->point, thisVtx->point, qp, prevVtxNewVoronoiCentre);

        influenceRegion[numInfluences][0] = prevVtxNewVoronoiCentre[0];
        influenceRegion[numInfluences][1] = prevVtxNewVoronoiCentre[1];
        numInfluences++;

        // Compute the area of influence (i.e weight) for thisVtx
        double areaOfInfluence = computeAreaOfInfluence(influenceRegion, numInfluences);
        nnIds[(*nn)++] = globalVtxId[ii];
        phi[globalVtxId[ii]] = areaOfInfluence;
        totalArea += areaOfInfluence;

        prevFacet = thisFacet;
        prevVtx = thisVtx;
    }

    // Normalize weights
    for (int ii = 0; ii < (*nn); ii++) {
        phi[nnIds[ii]] /= totalArea;
    }

    free(conflictHandles);
    free(conflictFacet);
    free(globalVtxId);
    free(influenceRegion);

    return NATURAL_NBR_OK;
}

void computeCoordsForFailedSampleSite(coderDelaunayTri* dtStruct,
                                      const double* qp,
                                      facetT** stackPtr,
                                      int* nnIds,
                                      int* nn,
                                      double* phi) {
    // While calculating gradients for linear extrap method, if qhull generates an
    // invalid or degenerate triangulation, the extrapolated values are nans.
    // Recompute the gradients using only a single triangle and warn the user.
    facetT* currFacet = NULL;
    qhT* qhdl = dtStruct->qhdl;

    (void)visibilityWalk(dtStruct->qhdl, qp, dtStruct->connList, dtStruct->ntri,
                         dtStruct->samplePts, dtStruct->numPts, &currFacet);

    // locType == FACET, the point is interior of the simplex.
    const circumCache* ccache = dtStruct->ccache;

    qhdl->visit_id++;
    unsigned int numConflicts = 3;

    // Stores the vertices of this triangle in ccw(as determined by qhull) order.
    vertexT** conflictHandles = malloc(numConflicts * sizeof(vertexT*));
    facetT** conflictFacet = malloc(numConflicts * sizeof(facetT*));
    unsigned int* globalVtxId = malloc(numConflicts * sizeof(unsigned int));
    unsigned int vtxCtr = 0;

    vertexT* vertexA = SETelemt_(currFacet->vertices, 0, vertexT);
    vertexT* vertexB = SETelemt_(currFacet->vertices, 1, vertexT);
    vertexT* vertexC = SETelemt_(currFacet->vertices, 2, vertexT);

    conflictHandles[vtxCtr] = vertexA;
    globalVtxId[vtxCtr] = qh_pointid(qhdl, vertexA->point);
    conflictFacet[vtxCtr++] = currFacet;

    if (!currFacet->toporient) {
        // A,B,C are in clockwise order
        conflictHandles[vtxCtr] = vertexB;
        globalVtxId[vtxCtr] = qh_pointid(qhdl, vertexB->point);
        conflictFacet[vtxCtr++] = currFacet;
        conflictHandles[vtxCtr] = vertexC;
        globalVtxId[vtxCtr] = qh_pointid(qhdl, vertexC->point);
        conflictFacet[vtxCtr++] = currFacet;
    } else {
        // A,B,C are in ccw order
        conflictHandles[vtxCtr] = vertexC;
        globalVtxId[vtxCtr] = qh_pointid(qhdl, vertexC->point);
        conflictFacet[vtxCtr++] = currFacet;
        conflictHandles[vtxCtr] = vertexB;
        globalVtxId[vtxCtr] = qh_pointid(qhdl, vertexB->point);
        conflictFacet[vtxCtr++] = currFacet;
    }

    double totalArea = 0;
    vertexT* prevVtx = conflictHandles[numConflicts - 1];
    facetT* prevFacet = conflictFacet[numConflicts - 1];
    double(*influenceRegion)[2] = malloc(2 * (numConflicts + 3) * sizeof(double));
    unsigned int numInfluences = 0;

    // Find the weight of each natural neighbor.
    // In this function, the neighbors will be the enclosing vertices of the triangle.
    for (unsigned int ii = 0; ii < numConflicts; ii++) {
        numInfluences = 0;

        vertexT* thisVtx = conflictHandles[ii];
        facetT* thisFacet = conflictFacet[ii];

        vertexT* nextVtx = conflictHandles[(ii + 1) % numConflicts];
        double newVoronoiCentre[2];
        circumcentre(thisVtx->point, nextVtx->point, qp, newVoronoiCentre);

        influenceRegion[numInfluences][0] = newVoronoiCentre[0];
        influenceRegion[numInfluences][1] = newVoronoiCentre[1];
        numInfluences++;

        if (ccache->ccentreValid[thisFacet->id]) {
            // Only one facet in the boundary of conflicts, add the circumcentre of the
            // facet
            influenceRegion[numInfluences][0] = ccache->ccentrePt[2 * thisFacet->id];
            influenceRegion[numInfluences][1] = ccache->ccentrePt[2 * thisFacet->id + 1];
            numInfluences++;
        }

        double prevVtxNewVoronoiCentre[2];
        circumcentre(prevVtx->point, thisVtx->point, qp, prevVtxNewVoronoiCentre);

        influenceRegion[numInfluences][0] = prevVtxNewVoronoiCentre[0];
        influenceRegion[numInfluences][1] = prevVtxNewVoronoiCentre[1];
        numInfluences++;

        // Compute the area of influence (i.e weight) for thisVtx
        double areaOfInfluence = computeAreaOfInfluence(influenceRegion, numInfluences);

        nnIds[(*nn)++] = globalVtxId[ii];
        phi[globalVtxId[ii]] = areaOfInfluence;
        totalArea += areaOfInfluence;

        prevFacet = thisFacet;
        prevVtx = thisVtx;
    }

    // Normalize weights
    for (int ii = 0; ii < (*nn); ii++) {
        phi[nnIds[ii]] /= totalArea;
    }

    free(conflictHandles);
    free(conflictFacet);
    free(globalVtxId);
    free(influenceRegion);
}

void computeGradient(const double* sampleSite,
                     const double valueAtSite,
                     const double* samplePts,
                     const double* sampleVals,
                     const int numPts,
                     const int kthFuncVal,
                     const int* nnIds,
                     const int nn,
                     const double* phi,
                     double** gradArrayPtr) {

    double A[4] = {0};
    double outerProd[4] = {0};
    double b[2] = {0};
    double vxPt[2];
    double nnVec[2];

    for (int i = 0; i < nn; i++) {
        int vxId = nnIds[i];

        vxPt[0] = samplePts[3 * vxId];
        vxPt[1] = samplePts[3 * vxId + 1];

        vectorSubtraction(sampleSite, vxPt, nnVec);
        double sibsonWeight = phi[nnIds[i]] / vectorLengthSquare(nnVec);

        computeOuterProd(nnVec, outerProd);
        for (size_t j = 0; j < 4; ++j) {
            A[j] += outerProd[j] * sibsonWeight;
        }
        double deltaVal = valueAtSite - sampleVals[vxId + numPts * kthFuncVal];
        b[0] += (nnVec[0]) * sibsonWeight * deltaVal;
        b[1] += (nnVec[1]) * sibsonWeight * deltaVal;
    }

    // gx = (b*f - d*e)/(a*d - b*c)
    (*gradArrayPtr)[0] = (A[1] * b[1] - A[3] * b[0]) / (A[0] * A[3] - A[1] * A[2]);
    // gy = -(a*f - c*e)/(a*d - b*c)
    (*gradArrayPtr)[1] = -(A[0] * b[1] - A[2] * b[0]) / (A[0] * A[3] - A[1] * A[2]);

    (*gradArrayPtr) += 2;
}

int computeBoundaryGradients(void** chStructPtr,
                              void** dtStructPtr,
                              const double* sampleVals,
                              const int nFuncVal,
                              double* const bndryGradients) {

    const coderConvHull* chStruct = (coderConvHull*)(*chStructPtr);
    const int numHullPts = chStruct->numHullPts;
    vertexT** chVtxHandles = chStruct->vertexHandle;
    const double* hullCentroid = chStruct->centroid;

    coderDelaunayTri* dtStruct = (coderDelaunayTri*)(*dtStructPtr);
    cacheCircumcentres(dtStruct);
    const int numPts = dtStruct->numPts;
    const int ntri = dtStruct->ntri;
    const double* samplePts = dtStruct->samplePts;

    double(*sampleSites)[2] = malloc(2 * numHullPts * sizeof(double));

    int status = 0;
    for (int i = 0; i < numHullPts; i++) {

        // Use the current vertex and previous/next vertices in counter-clockwise ordering to
        // compute the sampling site that is used to calculate the gradient.
        /*
             *      <- (i+1)th vertex
              \
               \
                *   <- ith vertex
                |
                |
                *   <- (i-1)th vertex
               /
              /
             *

        */

        samplingSiteForGradient(i, (i + 1) % numHullPts, ((i - 1) + numHullPts) % numHullPts,
                                chVtxHandles, hullCentroid, sampleSites[i]);
    }

    // Arrays to store the indices of the natural neighbors and the associated weights.
    int nn = 0;
    int* nnIds = malloc(numPts * sizeof(int));
    // This is probably larger then necessary, can be smaller and realloced if needed.
    double* phi = malloc(numPts * sizeof(double));

    // Array to perform stack operations
    facetT** stackPtr = malloc(ntri * sizeof(facetT*));

    // Pointer to the current location to be used for storing the boundary
    // gradients. The memory required is allocated in MATLAB, the dimensions
    // of the array ensure that we need only increment the pointer in C.
    double* gradArrayPtr = bndryGradients;

    // Now compute the gradients
    for (int i = 0; i < numHullPts; ++i) {

        nn = 0;
        // Ignore outcome, we have handcrafted sample site to be interior of convex hull.
        naturalNeighborOutcomeT chNN =
            computeNaturalNeighborCoords(dtStruct, sampleSites[i], stackPtr, nnIds, &nn, phi);

        if (chNN != NATURAL_NBR_OK) {
            status = 1; // Issue warning, 
        }
        if (chNN == NATURAL_NBR_FAILED_INSIDE) {
            // Use only enclosing simplex to calculate gradient.
            computeCoordsForFailedSampleSite(dtStruct, sampleSites[i], stackPtr, nnIds, &nn, phi);
        }

        for (int k = 0; k < nFuncVal; k++) {
            double valueAtSite = 0;
            sumNNWeightedValues(nnIds, nn, phi, sampleVals, numPts, k, &valueAtSite);

            // Calculate the x,y gradient for this hull vertex and increment
            // the array pointer to point to the next storage location.
            computeGradient(sampleSites[i], valueAtSite, samplePts, sampleVals, numPts, k, nnIds,
                            nn, phi, &gradArrayPtr);
        }
    }

    // Deallocate temp arrays
    free(nnIds);
    free(phi);
    free(stackPtr);
    free(sampleSites);
    return status;
}

void cacheCircumcentres(coderDelaunayTri* dtStruct) {

    if (dtStruct->ccache) {
        return;
    }

    qhT* qh = dtStruct->qhdl;
    int ntri = dtStruct->ntri;

    circumCache* ccache = malloc(sizeof(circumCache));
    double* ccentrePt = malloc(2 * ntri * sizeof(double));
    unsigned char* ccentreValid = malloc(ntri * sizeof(unsigned char));

    ccache->ccentrePt = ccentrePt;
    ccache->ccentreValid = ccentreValid;
    dtStruct->ccache = ccache;

    facetT* facet;
    FORALLfacets {
        if (facet->upperdelaunay) {
            continue;
        }
        double area = fabs(qh_facetarea(qh, facet));
        if (area > 1.0e-15) {
            pointT* centre;
            centre = qh_facetcenter(qh, facet->vertices);
            ccentrePt[2 * facet->id] = (double)centre[0];
            ccentrePt[2 * facet->id + 1] = (double)centre[1];
            ccentreValid[facet->id] = (unsigned char)1;
        } else {
            ccentreValid[facet->id] = (unsigned char)0;
            // Assigning to ensure complete definition
            ccentrePt[2 * facet->id] = 0.;
            ccentrePt[2 * facet->id + 1] = 0.;
        }
    }
}

void expandSearch(qhT* qh, vertexT* vx, const double* qp, vertexT*** stackPtr) {

    facetT* facet;
    facetT** facetp;
    vertexT* vertex;
    vertexT** vertexp;

    FOREACHfacet_(vx->neighbors) {
        if (facet->upperdelaunay) {
            continue;
        }
        FOREACHvertex_(facet->vertices) {
            if (vertex->visitid == qh->vertex_visit) {
                continue;
            }
            vertex->visitid = qh->vertex_visit;
            push(*stackPtr, vertex);
        }
    }
}

unsigned int testPointAgainstAttachedFacets(qhT* qh,
                                            const vertexT* vxh,
                                            const vertexT* nearestVxh,
                                            const double* qp,
                                            int* vxIds,
                                            double* vxBcoords,
                                            int* numVxs,
                                            double* nearestVec) {
    // This function defines the sample points to be used for performing the
    // extrapolation and the weights to be assigned to those points.
    vertexT* vxFlowingOut;
    vertexT* vxFlowingIn;

    facetT* facet;
    facetT** facetp;

    // Search the neighbors of the vertex being currently visited for the following.
    // The vertex 'flowing in' is the previous vertex in ccw ordering of the vertices.
    // The vertex 'flowing out' is the next vertex in ccw ordering of the vertices.
    FOREACHfacet_(vxh->neighbors) {
        if (facet->upperdelaunay) {
            continue;
        }

        for (int i = 0; i < 3; i++) {
            vertexT* vertexA = SETelemt_(facet->vertices, i, vertexT);
            if (vertexA == vxh) {
                vertexT* vertexB = SETelemt_(facet->vertices, (i + 1) % 3, vertexT);
                vertexT* vertexC = SETelemt_(facet->vertices, (i + 2) % 3, vertexT);
                facetT* facetB =
                    SETelemt_(facet->neighbors, (i + 1) % 3, facetT); // facet opposite vertexB.
                facetT* facetC =
                    SETelemt_(facet->neighbors, (i + 2) % 3, facetT); // facet opposite vertexC.
                
                if (!facet->toporient) {
                    // A,B,C are in clockwise order, swap vertexB, vertexC
                    // to get ccw ordering of facet vertices.
                    vertexT* tVtx = vertexB;
                    vertexB = vertexC;
                    vertexC = tVtx;
                    // Swap the facets, so that facetB remains opposite vertexB
                    facetT* tFacet = facetB;
                    facetB = facetC;
                    facetC = tFacet;
                }
                // If a facet is upper delaunay, the vertex that is not opposite,
                // this facet will be connected to vetexA
                if (facetB->upperdelaunay) {
                    vxFlowingIn = vertexC;
                }

                if (facetC->upperdelaunay) {
                    vxFlowingOut = vertexB;
                }
            }
        }
    }

    double vecToIn[2];
    vectorSubtraction(vxh->point, vxFlowingIn->point, vecToIn);
    double vecToOut[2];
    vectorSubtraction(vxh->point, vxFlowingOut->point, vecToOut);
    double vecToQuery[2];
    vectorSubtraction(vxh->point, qp, vecToQuery);

    double tParamIn = dotProd(vecToQuery, vecToIn) / vectorLengthSquare(vecToIn);
    double tParamOut = dotProd(vecToQuery, vecToOut) / vectorLengthSquare(vecToOut);

    if ((!leftTurn(vxFlowingIn->point, vxh->point, qp)) && (tParamIn > 0.) && (tParamIn < 1.)) {
        double rootPt[2];
        rootPt[0] = vxh->point[0] + (vecToIn[0] * tParamIn);
        rootPt[1] = vxh->point[1] + (vecToIn[1] * tParamIn);
        vectorSubtraction(rootPt, qp, nearestVec);
        vxIds[0] = qh_pointid(qh, vxh->point);
        vxIds[1] = qh_pointid(qh, vxFlowingIn->point);
        vxBcoords[0] = 1.0 - tParamIn;
        vxBcoords[1] = tParamIn;
        *numVxs = 2;
        return 1;

    } else if ((!leftTurn(vxh->point, vxFlowingOut->point, qp)) && (tParamOut > 0.) &&
               (tParamOut < 1.)) {
        double rootPt[2];
        rootPt[0] = vxh->point[0] + (vecToOut[0] * tParamOut);
        rootPt[1] = vxh->point[1] + (vecToOut[1] * tParamOut);
        vectorSubtraction(rootPt, qp, nearestVec);
        vxIds[0] = qh_pointid(qh, vxh->point);
        vxIds[1] = qh_pointid(qh, vxFlowingOut->point);
        vxBcoords[0] = 1.0 - tParamOut;
        vxBcoords[1] = tParamOut;
        *numVxs = 2;
        return 1;

    } else if ((vxh == nearestVxh) && (!leftTurn(vxFlowingIn->point, vxFlowingOut->point, qp))) {
        // The vertex currently being visited is the same as the nearest vertex
        vectorSubtraction(vxh->point, qp, nearestVec);
        vxIds[0] = qh_pointid(qh, vxh->point);
        vxBcoords[0] = 1.0;
        *numVxs = 1;
        return 1;

    } else {
        return 0;
    }
}

void hullVxIds2TriVxIds(const int* globalHullIndex, const int* numVxs, int* hullIds, int* vxIds) {
    // Convert the index of the convex hull vertex to the gloabl index
    // i.e. index of the point in the input points array.
    for (int i = 0; i < (*numVxs); i++) {
        vxIds[i] = globalHullIndex[hullIds[i]] + 1; // Add 1 to get MATLAB index.
        hullIds[i] += 1;                            // Add 1 to get MATLAB index.
    }
}

void nearestOnHull(void** chTriStructPtr,
                   void** chStructPtr,
                   const double* qp,
                   int* vxIds,
                   int* hullIds, // Points to the same vertices as vxIds, stores the index of the
                   // vertex wrt the convex hull inputs.
                   double* vxBcoords,
                   int* numVxs,
                   double* nearestVec) {

    // Figure out the points on the convex hull that will be used to
    // extrpolate at the query point.
    coderDelaunayTri* chTriStruct = (coderDelaunayTri*)(*chTriStructPtr);
    qhT* qh = chTriStruct->qhdl;

    // Find the nearest point on the convex hull using dsearch
    int vxId = dsearchQueryOutsideHull(qh, qp);
    vxId--; // dsearch returns MATLAB based index.
    vertexT* nearestVxh = chTriStruct->vertexHandle[vxId];

    // Seed the search
    vertexT** stackPtr = malloc((chTriStruct->ntri) * sizeof(vertexT*));
    vertexT** spBottom = stackPtr;

    qh->vertex_visit++;
    push(stackPtr, nearestVxh);
    nearestVxh->visitid = qh->vertex_visit;

    vertexT* neighborHandle = NULL;
    while (stackPtr - spBottom) {
        vertexT* currVxh = pop(stackPtr);

        if (neighborHandle) {
            // A previous vertex is a possible nearest neighbor. Check if the vertex
            // that we are currently visiting is closer to the query than the previous vertex.
            // If not we can skip this vertex.
            if (!(vectorDistanceSquare(currVxh->point, qp) <
                  vectorDistanceSquare(neighborHandle->point, qp))) {
                continue;
            }
        }
        // Test to check if sample point being currently visited is a good fit
        // to use for extrapolation.
        if (testPointAgainstAttachedFacets(qh, currVxh, nearestVxh, qp, hullIds, vxBcoords, numVxs,
                                           nearestVec)) {
            neighborHandle = currVxh; // Neighbor search succeeded, store the vertex that was used.
        }
        // Search for the sample point to be used among the adjacent vertces.
        expandSearch(qh, currVxh, qp, &stackPtr);
    }

    coderConvHull* chStruct = (coderConvHull*)(*chStructPtr);

    if (neighborHandle) {
        hullVxIds2TriVxIds(chStruct->globalHullIndex, numVxs, hullIds, vxIds);
    } else {
        // If we get to here the nearest search algorithm failed
        // to find a facet or edge in the neighborhood.
        // Default to nearest vertex.
        vectorSubtraction(nearestVxh->point, qp, nearestVec);
        hullIds[0] = vxId;
        vxBcoords[0] = 1;
        *numVxs = 1;
        hullVxIds2TriVxIds(chStruct->globalHullIndex, numVxs, hullIds, vxIds);
    }
    free(spBottom);
}

int naturalNeighborInterpolation(void** dtStructPtr,
                                 const double* sampleVals,
                                 const int nFuncVal,
                                 const double* qp,
                                 double* singleQueryOut) {

    coderDelaunayTri* dtStruct = (coderDelaunayTri*)(*dtStructPtr);
    cacheCircumcentres(dtStruct);
    const int numPts = dtStruct->numPts;
    const int ntri = dtStruct->ntri;

    // Arrays to store the indices of the natural neighbors and the associated weights.
    int nn = 0;
    int* nnIds = malloc(numPts * sizeof(int));
    double* phi = malloc(numPts * sizeof(double));

    // Array to perform stack operations
    facetT** stackPtr = malloc(ntri * sizeof(facetT*));

    // Compute natural neighbors
    nn = 0;
    naturalNeighborOutcomeT nnOutcome =
        computeNaturalNeighborCoords(dtStruct, qp, stackPtr, nnIds, &nn, phi);

    if (nnOutcome == NATURAL_NBR_OK) {
        // natural neighbor computation was successful, calculate the interpolated value
        // at the query point.
        for (int k = 0; k < nFuncVal; k++) { // Loop for multiple sets of values
            double valueAtQuery = 0;
            sumNNWeightedValues(nnIds, nn, phi, sampleVals, numPts, k, &valueAtQuery);
            singleQueryOut[k] = valueAtQuery;
        }
    }
    // Other possible cases are :
    // 1) nnOutcome == NATURAL_NBR_FAILED_BOUNDARY || nnOutcome == NATURAL_NBR_FAILED_INSIDE
    //    The point is either on boundary of convex hull or we encountered non-finites
    //    during natural neighbor computation. Fallback to linear interpolation.
    // 2) nnOutCome == NATURAL_NBR_FAILED_OUTSIDE
    //    Point is outside convex hull, call the extrapolation method.

    // Deallocate temp arrays
    free(nnIds);
    free(phi);
    free(stackPtr);
    // return outcome of natural neighbor interpolation.
    return nnOutcome;
}

void createQhullWrapperStructs(void** dtStructPtr, void** chStructPtr, void** chTriStructPtr) {

    // Allocate memory to store the delaunay triangulation of the input
    // sample points
    coderDelaunayTri* dtStruct = malloc(sizeof(coderDelaunayTri));
    dtStruct->connList = NULL;
    dtStruct->vertexHandle = NULL;
    dtStruct->ccache = NULL;
    dtStruct->startFacet = NULL;
    dtStruct->qhdl = NULL;
    *dtStructPtr = (void*)dtStruct;

    // Allocate memory to store the vertices on the convex hull of the delaunay
    // triangulation.
    coderConvHull* chStruct = malloc(sizeof(coderConvHull));
    chStruct->vertexHandle = NULL;
    chStruct->globalHullIndex = NULL;
    *chStructPtr = (void*)chStruct;

    // Allocate memory to store the delaunay triangulation of the vertices
    // on the convex hull.
    coderDelaunayTri* chTriStruct = malloc(sizeof(coderDelaunayTri));
    chTriStruct->connList = NULL;
    chTriStruct->vertexHandle = NULL;
    chTriStruct->ccache = NULL;
    chTriStruct->startFacet = NULL;
    chTriStruct->qhdl = NULL;
    *chTriStructPtr = (void*)chTriStruct;
}


int deleteDelaunayTriStruct(void** dtStructPtr) {

    if (!(*dtStructPtr)) {
        return 0;
    }

    // Free memory allocated to the delaunay triangulation structure.
    coderDelaunayTri* dtStruct = (coderDelaunayTri*)(*dtStructPtr);

    // Delete qhT struct of delaunay triangulation.
    if (dtStruct->qhdl) {
        qhT* qh = dtStruct->qhdl;
        int curlong, totlong;
        qh_freeqhull(qh, False);
        qh_memfreeshort(qh, &curlong, &totlong);
        if (curlong || totlong) {
            return -1;
        }
        free(qh);
        dtStruct->qhdl = NULL;
    }

    // Delete connectivity list.
    if (dtStruct->connList) {
        free(dtStruct->connList);
        dtStruct->connList = NULL;
    }
    // Delete circumcentre cache
    if (dtStruct->ccache) {
        free(dtStruct->ccache->ccentrePt);
        free(dtStruct->ccache->ccentreValid);
        free(dtStruct->ccache);
        dtStruct->ccache = NULL;
    }
    // Delete the vertex handles
    if (dtStruct->vertexHandle) {
        free(dtStruct->vertexHandle);
    }
    // Finally, delete the structure itself
    free(dtStruct);
    // NULL out the dtStructPtr
    *dtStructPtr = NULL;
    return 0;
}


int deleteConvexHullStruct(void** chStructPtr) {

    if (!(*chStructPtr)) {
        return 0;
    }

    // Free memory allocated to the convex hull structure.
    coderConvHull* chStruct = (coderConvHull*)(*chStructPtr);

    // Delete the vertex handles
    if (chStruct->vertexHandle) {
        free(chStruct->vertexHandle);
    }
    // Delete the index array
    if (chStruct->globalHullIndex) {
        free(chStruct->globalHullIndex);
    }
    // Finally, delete the structure itself
    free(chStruct);
    // NULL out the chStructPtr
    *chStructPtr = NULL;
    return 0;
}

int getQhullErrorStr(char errString[]) {
    int errstrlen = (qh_buflen > 4096) ? 4096 : qh_buflen;
    strncpy(errString, mwerrmsg, errstrlen);
    return strlen(errString);
}
