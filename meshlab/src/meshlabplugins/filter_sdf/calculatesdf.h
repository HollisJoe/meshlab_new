#ifndef __CALCULATE_SDF_H_
#define __CALCULATE_SDF_H_

// If this flag is enabled, only one ray for cone is casted
#define DEBUG_SDF_SINGLERAY

#include <qobject.h>
#include <qthread.h>
#include <QApplication.h>
#include <vcg/space/index/aabb_binary_tree/aabb_binary_tree.h>

//--- ATA2: these have been added to bridge with MeshLAB
// the dialog utilities m_progDlg have been removed
// completely. The Mesh type has been replaced by CMeshO
// under the assumption the vertex array has been compacted
#include "common/interfaces.h"
#include "vcg/space/point3.h"
using namespace vcg;
const float DEG2RAD = 3.1415/180.0;
//--- END

class CalculateSDF{
public:
    CalculateSDF(CMeshO* mesh){
        this->mesh = mesh;
        // this->m_rayIntersect = new RayIntersect(mesh);
        this->raysInCone = 20;
        this->coneAperture = 70; // DEGREES
        // this->gaussianWeights = true;
        this->smoothing = false;
        this->smoothingAnisotropic = false;
        this->smoothingIterations = 1;
        // this->gridsize = 20;
    }

private:
    // Original mesh and intersectoin accellerator
    vcg::AABBBinaryTreeIndex<CFaceO, float, vcg::EmptyClass> sIndex;
    // RayIntersect* m_rayIntersect;
    CMeshO* mesh;

    // Processing data structures
    vector<Ray3f> origins;
    vector<float>* results;

    // Options
    int raysInCone;
    float coneAperture;
    bool smoothing;
    bool smoothingAnisotropic;
    int smoothingIterations;

    // int gridsize;


protected:
    void makeFacesNVolume(bool smoothing, bool smoothingAnisotropic, int smoothingIterations);

    /// Compute approximate volume for a point and its normal
    /// @return the calculated volume
    float traceSdfConeAt( const Point3f& p, const Point3f& n );

	/**
	 *	write results to mesh
	 *
	 * @param mesh pointer to mesh
	 * @param onVertices if true calculates on vertices, otherwise on facets
	 * @param results array in which results are stored (not normalized)
	 * @param normalize if true normalize from 0 to 1
	 */
    // bool postprocess( CMeshO* mesh, const bool onVertices, number_type* results, const NormalizeTypes normalize, const bool smoothing);
	
public:
    enum SDFMODE {VERTICES,FACES};
    void init(SDFMODE mode, vector<float>& results);
    void compute( vector<float>& results, vcg::CallBackPos* cb=0 );
};

#endif
