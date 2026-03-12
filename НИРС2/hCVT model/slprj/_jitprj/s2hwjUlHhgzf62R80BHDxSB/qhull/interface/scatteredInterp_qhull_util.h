/* Copyright 2024-2025 The MathWorks, Inc. */
#ifndef CODER_SCATTEREDINTERP_UTIL_H
#define CODER_SCATTEREDINTERP_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

int computeDelaunayTri(void** dtStructPtr,
                       double* pts,
                       int numDims,
                       int numPts,
                       const char* options);

int tsearch(void** dtStructPtr, const double* qp);

int dsearch(void** dtStructPtr, const double* qp, int* isInsideHull);

void getVtxIDsOfSimplex(void** dtStructPtr, const int sid, int* vxID);

void createQhullWrapperStructs(void** dtStructPtr, void** chStructPtr, void** chTriStructPtr);

int deleteDelaunayTriStruct(void** dtStructPtr);

int deleteConvexHullStruct(void** chStructPtr);

int computeConvexHullOfDelaunayTri(void** dtStructPtr, void** chStructPtr, int* nummHullVertices);
int computeTriangulationForConvexHull(void** chStructPtr,
                                      void** chTriStructPtr,
                                      const char* options);

int computeBoundaryGradients(void** chStructPtr,
                              void** dtStructPtr,
                              const double* sampleVals,
                              const int nFuncVal,
                              double* const bndryGradients);

int naturalNeighborInterpolation(void** dtStructPtr,
                                 const double* sampleVals,
                                 const int nFuncVal,
                                 const double* qp,
                                 double* singleQueryOut);

void nearestOnHull(void** chTriStructPtr,
                   void** chStructPtr,
                   const double* qp,
                   int* vxIds,
                   int* hullIds,
                   double* vxBcoords,
                   int* numVxs,
                   double* nearestVec);

int getQhullErrorStr(char errString[]);

#ifdef __cplusplus
}
#endif

#endif // CODER_SCATTEREDINTERP_UTIL_H
