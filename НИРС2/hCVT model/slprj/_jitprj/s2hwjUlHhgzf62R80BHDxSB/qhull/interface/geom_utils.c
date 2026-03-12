/* Copyright 2024 The MathWorks, Inc. */

#include <math.h>
#include <float.h>
void vectorSubtraction(const double* a, const double* b, double* vec) {
    // vec = b - a
    vec[0] = b[0] - a[0];
    vec[1] = b[1] - a[1];
}

void vectorRotate90Clockwise(double* v) {
    // Rotate vector(x,y), **inplace**, 90 degrees in clockwise direction (y,-x)
    double temp = v[0];
    v[0] = v[1];
    v[1] = -1 * temp;
}

double vectorLengthSquare(const double* v) {
    // Return square of vector length, used instead of length in comparisons
    return v[0] * v[0] + v[1] * v[1];
}

void normalizeVector(double* v, const double LOCAL_TOL) {
    // Normalize vector in-place, LOCAL_TOL defines the tolerance below which
    // we consider vector to have zero length

    double vecLen = vectorLengthSquare(v);
    vecLen = sqrt(vecLen);

    if (vecLen < LOCAL_TOL) {
        // Vector has zero length, manually set it to origin
        v[0] = 0;
        v[1] = 0;
    } else {
        v[0] /= vecLen;
        v[1] /= vecLen;
    }
}

double vectorDistanceSquare(const double* v1, const double* v2) {
    // Return square of distance between vectors, used instead of length in comparisons.
    double vec[2];
    vectorSubtraction(v1, v2, vec);
    return vectorLengthSquare(vec);
}

void computeOuterProd(const double* nnVec, double* outerProd) {
    outerProd[0] = nnVec[0] * nnVec[0];
    outerProd[1] = nnVec[0] * nnVec[1];
    outerProd[2] = outerProd[1];
    outerProd[3] = nnVec[1] * nnVec[1];
}

double crossProd(const double* v1, const double* v2, const double* p1) {
    /* Cross product of the vectors
     * (v1x - p1x)i + (v1y - p1y)j
     * (v2x - p1x)i + (v2y - p1y)j
     */
    return ((v1[0] - p1[0]) * (v2[1] - p1[1])) - ((v2[0] - p1[0]) * (v1[1] - p1[1]));
}

unsigned int leftTurn(const double* p, const double* q, const double* r) {
    // Returns true if points p, q, r form a left turn
    double pq[2];
    double qr[2];
    vectorSubtraction(p, q, pq);
    vectorSubtraction(q, r, qr);
    double origin[2] = {0};
    return (crossProd(pq, qr, origin) > 0);
}

double dotProd(const double* v1, const double* v2) {
    // Dot product of two vector
    return v1[0] * v2[0] + v1[1] * v2[1];
}

void triIncenter(const double* a,
                 const double* b,
                 const double* c,
                 double* incenter,
                 double* inRadius) {
    // Compute the incenter of triangle. This code is replicated from MATLAB cgeo_constructions.
    double xba = b[0] - a[0];
    double yba = b[1] - a[1];
    double xca = c[0] - a[0];
    double yca = c[1] - a[1];
    double xbc = b[0] - c[0];
    double ybc = b[1] - c[1];

    /* Lengths of the edges. */
    double balength = sqrt(xba * xba + yba * yba);
    double calength = sqrt(xca * xca + yca * yca);
    double bclength = sqrt(xbc * xbc + ybc * ybc);
    /* Perimeter */
    double tperim = balength + calength + bclength;
    incenter[0] = (a[0] * bclength + b[0] * calength + c[0] * balength) / tperim;
    incenter[1] = (a[1] * bclength + b[1] * calength + c[1] * balength) / tperim;

    if (inRadius) {
        // Compute radius of incentre
        tperim = tperim * 0.5;
        *inRadius = ((tperim - balength) * (tperim - calength) * (tperim - bclength)) / tperim;
        *inRadius = sqrt(*inRadius);
    }
}

void circumcentre(const double* x1, const double* x2, const double* x3, double* cc) {
    double A[2][2];
    // Shift x3 to origin and create matrix [x1;x2]
    vectorSubtraction(x3, x1, A[0]);
    vectorSubtraction(x3, x2, A[1]);

    // Get x1-x3 and x2-x3
    double b[2];
    b[0] = vectorLengthSquare(A[0]);
    b[1] = vectorLengthSquare(A[1]);

    // Determinant for the shifted [x1;x2] matrix
    double D = (A[0][0] * A[1][1]) - (A[0][1] * A[1][0]);
    D *= 2;
    double t = A[0][0];
    // Invert matrix
    A[0][0] = A[1][1];
    A[1][1] = t;
    A[0][1] *= -1;
    A[1][0] *= -1;

    // Calculate circumcenter
    cc[0] = A[0][0] * b[0] + A[0][1] * b[1];
    cc[1] = A[1][0] * b[0] + A[1][1] * b[1];
    cc[0] /= D;
    cc[1] /= D;

    cc[0] += x3[0];
    cc[1] += x3[1];
}

double signedArea(const double* x1, const double* x2, const double* x3) {
    double v = crossProd(x1, x2, x3);
    return v / 2;
}

unsigned int isInsideTriangle(const double triVtx[3][2], const double* qp) {
    // Returns 1 if point is inside or on the triangle, 0 otherwise
    const double myeps = DBL_EPSILON * 3;
    for (int k = 0; k < 3; k++) {
        double c1 = crossProd(&triVtx[(k + 1) % 3][0], &triVtx[(k + 2) % 3][0], qp);
        if (fabs(c1) < myeps) {
            // This is on the edge, treat as inside.
            continue;
        }
        double c2 = crossProd(&triVtx[(k + 1) % 3][0], &triVtx[(k + 2) % 3][0], &triVtx[k][0]);

        if ((c1 > 0.) == (c2 > 0.)) {
            continue;
        } else {
            // Query point and vertex lie on different half-planes created
            // by the edge.
            return 0;
        }
    }
    // Query point and vertex lie on same side of edge for all 3 combinations,
    // only possible when point is inside simplex.
    return 1;
}

unsigned int incircle(const double* v1, const double* v2, const double* v3, const double* qp) {
    // Check if query point 'qp' lies inside the circumcircle of the triangle formed
    // by v1, v2, v3.
    // v1, v2 and v3 must be in counter clockwise order.

    const double adx = v1[0] - qp[0];
    const double ady = v1[1] - qp[1];
    const double bdx = v2[0] - qp[0];
    const double bdy = v2[1] - qp[1];
    const double cdx = v3[0] - qp[0];
    const double cdy = v3[1] - qp[1];

    const double abdet = adx * bdy - bdx * ady;
    const double bcdet = bdx * cdy - cdx * bdy;
    const double cadet = cdx * ady - adx * cdy;
    const double alift = adx * adx + ady * ady;
    const double blift = bdx * bdx + bdy * bdy;
    const double clift = cdx * cdx + cdy * cdy;

    const double myeps = -1 * (DBL_EPSILON * 3);
    return (alift * bcdet + blift * cadet + clift * abdet) > myeps;
}
