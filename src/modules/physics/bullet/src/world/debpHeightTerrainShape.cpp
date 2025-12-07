/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>

#include "debpHeightTerrainShape.h"
#include "../dePhysicsBullet.h"
#include "../coldet/collision/debpDCollisionBox.h"
#include "../coldet/collision/debpDCollisionSphere.h"
#include "../coldet/collision/debpCollisionBox.h"
#include "../coldet/collision/debpDECollisionDetection.h"
#include "../terrain/heightmap/debpHTSector.h"
#include "../terrain/heightmap/debpHeightTerrain.h"

#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/decal/deDecalList.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/exceptions.h>



// Class debpHeightTerrainShape
/////////////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	static decTimer timer2;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS				timer.Reset(); timer2.Reset()
	#define DEBUG_PRINT_TIMER(what)			pSector->GetHeightTerrain()->GetBullet()->LogInfoFormat("HTShape: %s = %iys", what, (int)(timer.GetElapsedTime() * 1e6))
	#define DEBUG_PRINT_TIMER2(what)		pSector->GetHeightTerrain()->GetBullet()->LogInfoFormat("HTShape: %s = %iys", what, (int)(timer2.GetElapsedTime() * 1e6))
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER(what)
	#define DEBUG_PRINT_TIMER2(what)
#endif



// Constructor, destructor
////////////////////////////

debpHeightTerrainShape::debpHeightTerrainShape(debpHTSector *sector){
	if(!sector){
		DETHROW(deeInvalidParam);
	}
	
	const float sectorDim = sector->GetHeightTerrain()->GetHeightTerrain()->GetSectorSize();
	const int imageDim = sector->GetImageDimension();
	
	pSector = sector;
	
	m_shapeType = TERRAIN_SHAPE_PROXYTYPE;
	//m_shapeType = CUSTOM_CONCAVE_SHAPE_TYPE;
	
	pPointCount = imageDim;
	
	pOffset = (btScalar)(sectorDim * 0.5f);
	pInvScale = (btScalar)(imageDim - 1) / (btScalar)sectorDim;
	pStep = (btScalar)sectorDim / (btScalar)(imageDim - 1);
	
	if(pSector->GetClusterCount() > 0){
		const debpHTSCluster &cluster = pSector->GetClusters()[0];
		const int clusterStepX = cluster.pointCountX - 1;
		const int clusterStepZ = cluster.pointCountZ - 1;
		pClusterCountX = ((imageDim - 1) / clusterStepX) + 1;
		pClusterCountZ = ((imageDim - 1) / clusterStepZ) + 1;
		pClusterInvScaleX = pInvScale / (btScalar)clusterStepX;
		pClusterInvScaleZ = pInvScale / (btScalar)clusterStepZ;
		
	}else{
		pClusterCountX = 0;
		pClusterCountZ = 0;
		pClusterInvScaleX = (btScalar)1.0;
		pClusterInvScaleZ = (btScalar)1.0;
	}
	
	pLocalScaling.setValue(btScalar(1.0), btScalar(1.0), btScalar(1.0));
}

debpHeightTerrainShape::~debpHeightTerrainShape(){
}



// Management
///////////////

void debpHeightTerrainShape::getAabb(const btTransform &trans, btVector3 &aabbMin, btVector3 &aabbMax) const{
	// NOTE
	// this method seems to be called a couple of times during creation but during simulation it is not
	// called anymore. this suggests that upon changing the height of the terrain in a way that the
	// bounding box is no more valid requires a trigger somewhere in bullet. where though is not know
	// yet. thus keep in mind if the simulation fails for some strange reason it is most probably this
	// call not matching bullet.
	const decDVector halfExtent((pSector->GetMaximumExtend() - pSector->GetMinimumExtend()) * 0.5);
	const decDVector center((pSector->GetMinimumExtend() + pSector->GetMaximumExtend()) * 0.5);
	
	const btScalar margin = getMargin();
	const btVector3 bpHalfExtendOrg((btScalar)halfExtent.x + margin, (btScalar)halfExtent.y + margin, (btScalar)halfExtent.z + margin);
	
	const btMatrix3x3 abs_b(trans.getBasis().absolute());
	const btVector3 bpCenter(trans * btVector3((btScalar)center.x, (btScalar)center.y, (btScalar)center.z));
	const btVector3 bpHalfExtent(abs_b[0].dot(bpHalfExtendOrg), abs_b[1].dot(bpHalfExtendOrg), abs_b[2].dot(bpHalfExtendOrg));
	
	aabbMin = bpCenter - bpHalfExtent;
	aabbMax = bpCenter + bpHalfExtent;
	
	//pSector->GetHeightTerrain()->GetBullet()->LogInfoFormat( "debpHeightTerrainShape.getAabb: ORG min=(%f,%f,%f) max=(%f,%f,%f)", pSector->GetMinimumExtend().x,
	//	pSector->GetMinimumExtend().y, pSector->GetMinimumExtend().z, pSector->GetMaximumExtend().x,
	//	pSector->GetMaximumExtend().y, pSector->GetMaximumExtend().z );
	//pSector->GetHeightTerrain()->GetBullet()->LogInfoFormat( "debpHeightTerrainShape.getAabb: min=(%f,%f,%f) max=(%f,%f,%f)", aabbMin.getX(), aabbMin.getY(), aabbMin.getZ(), aabbMax.getX(), aabbMax.getY(), aabbMax.getZ() );
}



void debpHeightTerrainShape::processRaycastAllTriangles(btTriangleRaycastCallback *callback,
const btVector3 &raySource, const btVector3 &rayTarget){
	// HACK: if there is no texture assigned to the height terrain ignore collisions
	if(pSector->GetSector()->GetTextureCount() == 0){
		return;
	}
	
	if(pSector->GetClusterCount() == 0){
		return;
	}
	
	// calculate the full area covered by the convex cast
	btVector3 rayAabbMinLocal(raySource);
	rayAabbMinLocal.setMin(rayTarget);
	
	btVector3 rayAabbMaxLocal(raySource);
	rayAabbMaxLocal.setMax(rayTarget);
	
	// determine the area to test
	int fromX, toX, fromZ, toZ;
	CalculateBoxPointIndices(rayAabbMinLocal, rayAabbMaxLocal, fromX, toX, fromZ, toZ);
	
	// if the number of triangles to test is small (5x5 for example) there is no need to optimize it
	if((toX - fromX) * (toZ - fromZ) <= 25){
		ProcessAllTrianglesIn(*callback, fromX, toX, fromZ, toZ);
		
	}else{
		// determine the covered clusters to optimize the test
		int clusterFromX, clusterToX, clusterFromZ, clusterToZ;
		CalculateBoxClusterIndices(rayAabbMinLocal, rayAabbMaxLocal, clusterFromX, clusterToX, clusterFromZ, clusterToZ);
		
		// process clusters
		const btVector3 btDisplacement = rayTarget - raySource;
		const decVector displacement((float)btDisplacement.x(), (float)btDisplacement.y(), (float)btDisplacement.z());
		const decVector rayFrom((float)raySource.x(), (float)raySource.y(), (float)raySource.z());
		
		debpHTSCluster * const clusters = pSector->GetClusters();
		int x, z;
		
		for(z=clusterFromZ; z<=clusterToZ; z++){
			const int linebase = pClusterCountX * z;
			
			for(x=clusterFromX; x<=clusterToX; x++){
				const debpHTSCluster &cluster = clusters[linebase + x];
				
				float dummyDistance;
				if(!debpDECollisionDetection::RayHitsBox(rayFrom, displacement, cluster.center, cluster.halfExtends, dummyDistance)){
					continue;
				}
				
				const int clusterPointX2 = cluster.firstPointX + cluster.pointCountX - 1;
				const int clusterPointZ2 = cluster.firstPointZ + cluster.pointCountZ - 1;
				
				const int subFromX = (fromX > cluster.firstPointX) ? fromX : cluster.firstPointX;
				const int subFromZ = (fromZ > cluster.firstPointZ) ? fromZ : cluster.firstPointZ;
				const int subToX = (toX < clusterPointX2) ? toX : clusterPointX2;
				const int subToZ = (toZ < clusterPointZ2) ? toZ : clusterPointZ2;
				
				ProcessAllTrianglesIn(*callback, subFromX, subToX, subFromZ, subToZ);
			}
		}
	}
DEBUG_PRINT_TIMER("processRaycastAllTriangles");
}

void debpHeightTerrainShape::processConvexcastAllTriangles(btTriangleConvexcastCallback *callback,
const btVector3 &boxSource, const btVector3 &boxTarget, const btVector3 &boxMin, const btVector3 &boxMax){
DEBUG_RESET_TIMERS;
	// HACK: if there is no texture assigned to the height terrain ignore collisions
	if(pSector->GetSector()->GetTextureCount() == 0){
		return;
	}
	
	if(pSector->GetClusterCount() == 0){
		return;
	}
	
	// calculate the full area covered by the convex cast
	btVector3 rayAabbMinLocal(boxSource);
	rayAabbMinLocal.setMin(boxTarget);
	rayAabbMinLocal += boxMin;
	
	btVector3 rayAabbMaxLocal(boxSource);
	rayAabbMaxLocal.setMax(boxTarget);
	rayAabbMaxLocal += boxMax;
	
	// determine the area to test
	int fromX, toX, fromZ, toZ;
	CalculateBoxPointIndices(rayAabbMinLocal, rayAabbMaxLocal, fromX, toX, fromZ, toZ);
	
	// if the number of triangles to test is small (5x5 for example) there is no need to optimize it
	if((toX - fromX) * (toZ - fromZ) <= 25){
		ProcessAllTrianglesIn(*callback, fromX, toX, fromZ, toZ);
		
	}else{
		// determine the covered clusters to optimize the test
		int clusterFromX, clusterToX, clusterFromZ, clusterToZ;
		CalculateBoxClusterIndices(rayAabbMinLocal, rayAabbMaxLocal, clusterFromX, clusterToX, clusterFromZ, clusterToZ);
		
		// process clusters
		debpCollisionBox colBoxShape;
		colBoxShape.SetFromExtends(
			decVector((float)boxMin.x(), (float)boxMin.y(), (float)boxMin.z()),
			decVector((float)boxMax.x(), (float)boxMax.y(), (float)boxMax.z()));
		colBoxShape.MoveBy(decVector((float)boxSource.x(), (float)boxSource.y(), (float)boxSource.z()));
		const btVector3 btDisplacement = boxTarget - boxSource;
		const decVector displacement((float)btDisplacement.x(), (float)btDisplacement.y(), (float)btDisplacement.z());
		
		debpHTSCluster * const clusters = pSector->GetClusters();
		int x, z;
		
#ifdef DO_TIMING
		int timingTotalClusterCount = 0;
		int timingProcessedClusterCount = 0;
		int timingProcessedTriangleCount = 0;
#endif
		
		for(z=clusterFromZ; z<=clusterToZ; z++){
			const int linebase = pClusterCountX * z;
			
			for(x=clusterFromX; x<=clusterToX; x++){
#ifdef DO_TIMING
				timingTotalClusterCount++;
#endif
				const debpHTSCluster &cluster = clusters[linebase + x];
				
				debpCollisionBox colBoxCluster(cluster.center, cluster.halfExtends);
				if(colBoxCluster.BoxMoveHitsBox(&colBoxShape, displacement, NULL) == 1.0f){
					continue;
				}
				
				const int clusterPointX2 = cluster.firstPointX + cluster.pointCountX - 1;
				const int clusterPointZ2 = cluster.firstPointZ + cluster.pointCountZ - 1;
				
				const int subFromX = (fromX > cluster.firstPointX) ? fromX : cluster.firstPointX;
				const int subFromZ = (fromZ > cluster.firstPointZ) ? fromZ : cluster.firstPointZ;
				const int subToX = (toX < clusterPointX2) ? toX : clusterPointX2;
				const int subToZ = (toZ < clusterPointZ2) ? toZ : clusterPointZ2;
				
#ifdef DO_TIMING
				timingProcessedClusterCount++;
				timingProcessedTriangleCount += (subToX - subFromX) * (subToZ - subFromZ) * 2;
#endif
//	 			pSector->GetHeightTerrain()->GetBullet()->LogInfoFormat( "processConvexcastAllTriangles cluster(%i,%i) (%f,%f,%f)(%f,%f,%f)", x, z,
// 					cluster.center.x, cluster.center.y, cluster.center.z, cluster.halfExtends.x, cluster.halfExtends.y, cluster.halfExtends.z );
// 				pSector->GetHeightTerrain()->GetBullet()->LogInfoFormat( "processConvexcastAllTriangles process [%i,%i] (%i,%i)->(%i,%i)",
// 					x, z, subFromX, subFromZ, subToX, subToZ );
				ProcessAllTrianglesIn(*callback, subFromX, subToX, subFromZ, subToZ);
			}
		}
	}
DEBUG_PRINT_TIMER("processConvexcastAllTriangles");
	
#if 0
#ifdef DO_TIMING
	pSector->GetHeightTerrain()->GetBullet()->LogInfoFormat("processConvexcastAllTriangles aabb (%f,%f,%f)->(%f,%f,%f)",
		rayAabbMinLocal.getX(), rayAabbMinLocal.getY(), rayAabbMinLocal.getZ(),
		rayAabbMaxLocal.getX(), rayAabbMaxLocal.getY(), rayAabbMaxLocal.getZ());
	
	pSector->GetHeightTerrain()->GetBullet()->LogInfoFormat("processConvexcastAllTriangles displacement (%f,%f,%f)->(%f,%f,%f)",
		boxSource.getX(), boxSource.getY(), boxSource.getZ(), boxTarget.getX(), boxTarget.getY(), boxTarget.getZ());
	
	pSector->GetHeightTerrain()->GetBullet()->LogInfoFormat("processConvexcastAllTriangles point (%i,%i)->(%i,%i)",
		fromX, fromZ, toX, toZ);
	
	pSector->GetHeightTerrain()->GetBullet()->LogInfoFormat("processConvexcastAllTriangles cluster (%i,%i)->(%i,%i)",
		clusterFromX, clusterFromZ, clusterToX, clusterToZ);
	
	pSector->GetHeightTerrain()->GetBullet()->LogInfoFormat("processConvexcastAllTriangles process clusters=%i/%i triangles=%i/%i",
		timingProcessedClusterCount, timingTotalClusterCount, timingProcessedTriangleCount, (toX - fromX) * (toZ - fromZ) * 2);
#endif
#endif
}

void debpHeightTerrainShape::processAllTriangles(btTriangleCallback *callback, const btVector3 &aabbMin, const btVector3 &aabbMax) const{
DEBUG_RESET_TIMERS;
	// HACK: if there is no texture assigned to the height terrain ignore collisions
	if(pSector->GetSector()->GetTextureCount() == 0){
		return;
	}
	
	//pSector->GetHeightTerrain()->GetBullet()->LogInfoFormat( "[SHAPE-HM] AABB (%g,%g,%g) -> (%g,%g,%g)", aabbMin.getX(), aabbMin.getY(), aabbMin.getZ(), aabbMax.getX(), aabbMax.getY(), aabbMax.getZ() );
	// determine the area to test
	int fromX, toX, fromZ, toZ;
	CalculateBoxPointIndices(aabbMin, aabbMax, fromX, toX, fromZ, toZ);
	//pSector->GetHeightTerrain()->GetBullet()->LogInfoFormat( "[SHAPE-HM] AREA (%i,%i) -> (%i,%i)", fromX, fromZ, toX, toZ );
	
	// process the triangles inside this area if visible
	ProcessAllTrianglesIn(*callback, fromX, toX, fromZ, toZ);
DEBUG_PRINT_TIMER("processAllTriangles");
}

void debpHeightTerrainShape::ProcessAllTrianglesIn(btTriangleCallback &callback, int fromX, int toX, int fromZ, int toZ) const{
	// NOTE one triangle in bullet costs around 1.55ys
	deHeightTerrainSector &engSector = *pSector->GetSector();
	const decVector * const points = pSector->GetPoints();
	btVector3 vertices[3];
	int x, z;
	
	for(z=fromZ; z<=toZ; z++){
		const int linebase = pPointCount * z;
		const btScalar topZ = pOffset - pStep * (btScalar)z;
		const btScalar bottomZ = pOffset - pStep * (btScalar)(z + 1);
		
		for(x=fromX; x<=toX; x++){
			if(engSector.GetFaceVisibleAt(x, z)){
				vertices[0].setValue(pStep * (btScalar)x - pOffset, (btScalar)points[linebase + x].y, topZ);
				vertices[1].setValue(vertices[0].getX() + pStep, (btScalar)points[linebase + x + 1].y, topZ);
				vertices[2].setValue(vertices[1].getX(), (btScalar)points[linebase + x + 1 + pPointCount].y, bottomZ);
				//pSector->GetHeightTerrain()->GetBullet()->LogInfoFormat( "- t1: (%g,%g,%g) (%g,%g,%g) (%g,%g,%g)", vertices[ 0 ].getX(), vertices[ 0 ].getY(), vertices[ 0 ].getZ(),
				//	vertices[ 1 ].getX(), vertices[ 1 ].getY(), vertices[ 1 ].getZ(), vertices[ 2 ].getX(), vertices[ 2 ].getY(), vertices[ 2 ].getZ() );
				callback.processTriangle(vertices, 0, pPointCount * z + x);
				
				vertices[1] = vertices[2];
				vertices[2].setValue(vertices[0].getX(), (btScalar)points[linebase + x + pPointCount].y, bottomZ);
				//pSector->GetHeightTerrain()->GetBullet()->LogInfoFormat( "- t2: (%g,%g,%g) (%g,%g,%g) (%g,%g,%g)", vertices[ 0 ].getX(), vertices[ 0 ].getY(), vertices[ 0 ].getZ(),
				//	vertices[ 1 ].getX(), vertices[ 1 ].getY(), vertices[ 1 ].getZ(), vertices[ 2 ].getX(), vertices[ 2 ].getY(), vertices[ 2 ].getZ() );
				callback.processTriangle(vertices, 0, pPointCount * z + x);
			}
		}
	}
}



void debpHeightTerrainShape::calculateLocalInertia(btScalar mass, btVector3 &inertia) const{
	inertia.setValue((btScalar)0.0, (btScalar)0.0, (btScalar)0.0); // moving concave objects not supported
}

void debpHeightTerrainShape::setLocalScaling(const btVector3 &scaling){
	pLocalScaling = scaling;
}

const btVector3 &debpHeightTerrainShape::getLocalScaling() const{
	return pLocalScaling;
}

const char *debpHeightTerrainShape::getName() const{
	return "BP_HEIGHTTERRAIN";
}



void debpHeightTerrainShape::CalculateBoxPointIndices(const btVector3 &boxMin, const btVector3 boxMax,
int &pointMinX, int &pointMaxX, int &pointMinZ, int &pointMaxZ) const{
	const float fudgeFactor = 0.001f;
	
	pointMinX = decMath::max((int)((boxMin.getX() - fudgeFactor + pOffset) * pInvScale), 0);
	pointMaxX = decMath::min((int)((boxMax.getX() + fudgeFactor + pOffset) * pInvScale), pPointCount - 2);
	pointMinZ = decMath::max((int)((pOffset - boxMax.getZ() - fudgeFactor) * pInvScale), 0);
	pointMaxZ = decMath::min((int)((pOffset - boxMin.getZ() + fudgeFactor) * pInvScale), pPointCount - 2);
}

void debpHeightTerrainShape::CalculateBoxClusterIndices(const btVector3 &boxMin, const btVector3 boxMax,
int &clusterMinX, int &clusterMaxX, int &clusterMinZ, int &clusterMaxZ) const{
	const float fudgeFactor = 0.001f;
	
	clusterMinX = decMath::max((int)((boxMin.getX() - fudgeFactor + pOffset) * pClusterInvScaleX), 0);
	clusterMaxX = decMath::min((int)((boxMax.getX() + fudgeFactor + pOffset) * pClusterInvScaleX), pClusterCountX - 2);
	clusterMinZ = decMath::max((int)((pOffset - boxMax.getZ() - fudgeFactor) * pClusterInvScaleZ), 0);
	clusterMaxZ = decMath::min((int)((pOffset - boxMin.getZ() + fudgeFactor) * pClusterInvScaleZ), pClusterCountZ - 2);
}
