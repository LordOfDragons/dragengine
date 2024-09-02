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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deoglOptimizerLight.h"
#include "../light/deoglRLight.h"
#include "../world/deoglWorld.h"
#include "../deoglBasics.h"
#include "../shadow/deoglSCSolid.h"
#include "../shadow/deoglShadowCaster.h"
#include "../shadow/deoglSCTransparent.h"
#include "../triangles/deoglTriangleSorter.h"
#include "../collidelist/deoglCollideList.h"
#include "../visitors/deoglTransformVolume.h"
#include "../deGraphicOpenGl.h"
#include "../utils/collision/deoglDCollisionTriangle.h"
#include "../utils/collision/deoglDCollisionSphere.h"

#include "dragengine/resources/light/deLight.h"
#include "dragengine/resources/world/deWorld.h"
#include "dragengine/common/math/decConvexVolume.h"
#include "dragengine/common/math/decConvexVolumeFace.h"
#include "dragengine/common/math/decConvexVolumeList.h"
#include "dragengine/common/utils/decTimer.h"
#include "dragengine/common/exceptions.h"



// Definitions
////////////////

enum eCropStages{
	ecsBuildTriangleList,
	ecsCropVolumes,
	ecsDone
};

enum eCasterStages{
	easBuildCollideList,
	easTestComponents,
	easBuildShadowCaster,
	easDone
};



// Class deoglOptimizerLight
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOptimizerLight::deoglOptimizerLight( deoglRLight *light, deoglWorld *world ){
	if( ! light || ! world ) DETHROW( deeInvalidParam );
	pLight = light;
	pWorld = world;
	
	pInitWarmUpTime = 0;
	pWarmUpTime = 0;
	
	pDoOptimizeLightVolume = true;
	pCropStage = ecsBuildTriangleList;
	pTriangleSorter = NULL;
	pConvexVolumeList = NULL;
	pMaxVolumeCount = 30;
	
	pDoOptimizeShadowCaster = false; //true;
	pCasterStage = easBuildCollideList;
	pFinalCollideList = NULL;
	pCollideList = NULL;
	pShadowCaster = NULL;
	
	try{
		pTriangleSorter = new deoglTriangleSorter;
		if( ! pTriangleSorter ) DETHROW( deeOutOfMemory );
		
		pCollideList = new deoglCollideList;
		if( ! pCollideList ) DETHROW( deeOutOfMemory );
		
		pFinalCollideList = new deoglCollideList;
		if( ! pFinalCollideList ) DETHROW( deeOutOfMemory );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglOptimizerLight::~deoglOptimizerLight(){
	pCleanUp();
	pLight->ClearOptimizer();
}



// Management
///////////////

bool deoglOptimizerLight::Run( int timeSlice ){
	//printf( "[OGL-LIGHT] Optimizer ( %p ): Running %i ys, stage %i\n", pLight, timeSlice, pCropStage );
	
	// wait until the warm up time elapsed
	if( pWarmUpTime > 0 ){
		pWarmUpTime -= ( int )( pWarmUpTimer.GetElapsedTime() * 1000 );
		if( pWarmUpTime > 0 ){
			return true;
		}
	}
	
	// run light optimization if required
	if( pDoOptimizeLightVolume && pCropStage != ecsDone ){
		if( pOptimizeLightVolume( timeSlice ) ) return true;
	}
	
	// run shadow casters optimization if required
	if( pDoOptimizeShadowCaster && pCasterStage != easDone ){
		if( pOptimizeShadowCaster( timeSlice ) ) return true;
	}
	
	// there is nothing more to be done
	return false;
}



void deoglOptimizerLight::SetInitialWarmUpTime( int warmUpTime ){
	if( warmUpTime < 0 ) DETHROW( deeInvalidParam );
	pInitWarmUpTime = warmUpTime;
}



void deoglOptimizerLight::SetOptimizeLightVolume( bool optimizeLightVolume ){
	pDoOptimizeLightVolume = optimizeLightVolume;
}

void deoglOptimizerLight::ResetLightVolume(){
	ResetAllOptimizations();
}

void deoglOptimizerLight::SetMaximalVolumeCount( int maximalVolumeCount ){
	if( maximalVolumeCount < 1 ) DETHROW( deeInvalidParam );
	pMaxVolumeCount = maximalVolumeCount;
}



void deoglOptimizerLight::SetOptimizeShadowCaster( bool optimizeShadowCaster ){
	pDoOptimizeShadowCaster = optimizeShadowCaster;
}

void deoglOptimizerLight::ResetShadowCaster(){
	pCasterStage = easBuildCollideList;
	pWarmUpTimer.Reset();
	pWarmUpTime = pInitWarmUpTime;
}

void deoglOptimizerLight::ResetAllOptimizations(){
	pCropStage = ecsBuildTriangleList;
	pCasterStage = easBuildCollideList;
	pWarmUpTimer.Reset();
	pWarmUpTime = pInitWarmUpTime;
}



// Private Functions
//////////////////////

void deoglOptimizerLight::pCleanUp(){
	if( pShadowCaster ) delete pShadowCaster;
	if( pCollideList ) delete pCollideList;
	if( pFinalCollideList ) delete pFinalCollideList;
	if( pConvexVolumeList ) delete pConvexVolumeList;
	if( pTriangleSorter ) delete pTriangleSorter;
}

bool deoglOptimizerLight::pOptimizeLightVolume( int timeSlice ){
	// build a list of sorted triangles
	if( pCropStage == ecsBuildTriangleList ){
		pBuildTriangleList();
		
		// if there are no triangles skip any further optimizations
		if( pTriangleSorter->GetTriangleCount() == 0 ){
			pCropStage = ecsDone;
			return false;
		}
		
		// calculate some values required later and switch to next stage
		pPrepareCroping();
		pCropStage = ecsCropVolumes;
		
		// for the time being we consider this a time consuming action hence
		// doing this once consumes our slice time
		return true;
	}
	
	// crop the light volume by a given number of triangles
	if( pCropStage == ecsCropVolumes ){
		if( pCropLightVolume( timeSlice ) ){
			return true;
		}
	}
	
	// there is nothing more to be done so stop further optimizing
//	printf( "[OGL-LIGHT] Optimizer ( %p ): volume count %i\n", pLight, pLight->GetConvexVolumeList()->GetVolumeCount() );
	pCropStage = ecsDone;
	return false;
}

void deoglOptimizerLight::pBuildTriangleList(){
}

void deoglOptimizerLight::pPrepareCroping(){
	decConvexVolume *volume = NULL;
	float xn, yn, xf, yf;
	float range;
	float offset;
	
	// get some light parameters
	range = pLight->GetRange();
	const decVector lightPosition = pLight->GetMatrix().GetPosition().ToVector();
	
	// determine the safe radius
	if( pLight->GetLightType() == deLight::eltPoint ){
		yf = xf = yn = xn = 0.0f;
		
		offset = ( sqrtf( 2.0f ) - 1.0f ) * range;
		
		pSafeRadius = sqrtf( offset * offset * 2.0f + range * range );
		
	}else{
		xn = 0.0f; //light->GetSpotSizeX() * 0.5f;
		yn = 0.0f; //light->GetSpotSizeY() * 0.5f;
		if( pLight->GetLightType() == deLight::eltSpot ){
			xf = xn + tanf( pLight->GetSpotAngle() * 0.5f ) * range;
			yf = yn + tanf( pLight->GetSpotAngle() * pLight->GetSpotRatio() * 0.5f ) * range;
		}else{
			xf = xn + tanf( pLight->GetSpotAngle() * 0.5f ) * range;
			yf = yn + tanf( pLight->GetSpotAngle() * pLight->GetSpotRatio() * 0.5f ) * range;
		}
		
		pSafeRadius = sqrtf( xf * xf + yf * yf + range * range );
	}
	
	// set the next triangle to use for croping
	pNextTriangle = 0;
	
	// create a convex volume list
	
	// update the convex volume list
	if( pConvexVolumeList ){
		pConvexVolumeList->RemoveAllVolumes();
		
	}else{
		pConvexVolumeList = new decConvexVolumeList;
		if( ! pConvexVolumeList ) DETHROW( deeInvalidParam );
	}
	
	if( pLight->GetLightType() == deLight::eltPoint ){
		offset = ( sqrtf( 2.0f ) - 1.0f ) * range;
		
		// add the light volume
		try{
			// create convex volume
			volume = new decConvexVolume;
			if( ! volume ) DETHROW( deeOutOfMemory );
			
			// add vertices
			volume->AddVertex( lightPosition + decVector( -offset, range, -offset ) );
			volume->AddVertex( lightPosition + decVector( offset, range, -offset ) );
			volume->AddVertex( lightPosition + decVector( -offset, range, offset ) );
			volume->AddVertex( lightPosition + decVector( offset, range, offset ) );
			
			volume->AddVertex( lightPosition + decVector( -range, offset, -offset ) );
			volume->AddVertex( lightPosition + decVector( -offset, offset, -range ) );
			volume->AddVertex( lightPosition + decVector( offset, offset, -range ) );
			volume->AddVertex( lightPosition + decVector( range, offset, -offset ) );
			volume->AddVertex( lightPosition + decVector( -range, offset, offset ) );
			volume->AddVertex( lightPosition + decVector( -offset, offset, range ) );
			volume->AddVertex( lightPosition + decVector( offset, offset, range ) );
			volume->AddVertex( lightPosition + decVector( range, offset, offset ) );
			
			volume->AddVertex( lightPosition + decVector( -range, -offset, -offset ) );
			volume->AddVertex( lightPosition + decVector( -offset, -offset, -range ) );
			volume->AddVertex( lightPosition + decVector( offset, -offset, -range ) );
			volume->AddVertex( lightPosition + decVector( range, -offset, -offset ) );
			volume->AddVertex( lightPosition + decVector( -range, -offset, offset ) );
			volume->AddVertex( lightPosition + decVector( -offset, -offset, range ) );
			volume->AddVertex( lightPosition + decVector( offset, -offset, range ) );
			volume->AddVertex( lightPosition + decVector( range, -offset, offset ) );
			
			volume->AddVertex( lightPosition + decVector( -offset, -range, -offset ) );
			volume->AddVertex( lightPosition + decVector( offset, -range, -offset ) );
			volume->AddVertex( lightPosition + decVector( -offset, -range, offset ) );
			volume->AddVertex( lightPosition + decVector( offset, -range, offset ) );
			
			// add faces
			pVolumeAddFace( volume, 2, 3, 1, 0, decVector( 0.0f, 1.0f, 0.0f ) ); // top
			
			pVolumeAddFace( volume, 0, 1, 6, 5, decVector( 0.0f, 1.0f, -1.0f ) ); // first band
			pVolumeAddFace( volume, 1, 7, 6, decVector( 1.0f, 1.0f, -1.0f ) );
			pVolumeAddFace( volume, 1, 3, 11, 7, decVector( 1.0f, 1.0f, 0.0f ) );
			pVolumeAddFace( volume, 3, 10, 11, decVector( 1.0f, 1.0f, 1.0f ) );
			pVolumeAddFace( volume, 3, 2, 9, 10, decVector( 0.0f, 1.0f, 1.0f ) );
			pVolumeAddFace( volume, 2, 8, 9, decVector( -1.0f, 1.0f, 1.0f ) );
			pVolumeAddFace( volume, 2, 0, 4, 8, decVector( -1.0f, 1.0f, 0.0f ) );
			pVolumeAddFace( volume, 0, 5, 4, decVector( -1.0f, 1.0f, -1.0f ) );
			
			pVolumeAddFace( volume, 5, 6, 14, 13, decVector( 0.0f, 0.0f, -1.0f ) ); // second band
			pVolumeAddFace( volume, 6, 7, 15, 14, decVector( 1.0f, 0.0f, -1.0f ) );
			pVolumeAddFace( volume, 7, 11, 19, 15, decVector( 1.0f, 0.0f, 0.0f ) );
			pVolumeAddFace( volume, 11, 10, 18, 19, decVector( 1.0f, 0.0f, 1.0f ) );
			pVolumeAddFace( volume, 10, 9, 17, 18, decVector( 0.0f, 0.0f, 1.0f ) );
			pVolumeAddFace( volume, 9, 8, 16, 17, decVector( -1.0f, 0.0f, 1.0f ) );
			pVolumeAddFace( volume, 8, 4, 12, 16, decVector( -1.0f, 0.0f, 0.0f ) );
			pVolumeAddFace( volume, 4, 5, 13, 12, decVector( -1.0f, 0.0f, -1.0f ) );
			
			pVolumeAddFace( volume, 13, 14, 21, 20, decVector( 0.0f, -1.0f, -1.0f ) ); // third band
			pVolumeAddFace( volume, 14, 15, 21, decVector( 1.0f, -1.0f, -1.0f ) );
			pVolumeAddFace( volume, 15, 19, 23, 21, decVector( 1.0f, -1.0f, 0.0f ) );
			pVolumeAddFace( volume, 19, 18, 23, decVector( 1.0f, -1.0f, 1.0f ) );
			pVolumeAddFace( volume, 18, 17, 22, 23, decVector( 0.0f, -1.0f, 1.0f ) );
			pVolumeAddFace( volume, 17, 16, 22, decVector( -1.0f, -1.0f, 1.0f ) );
			pVolumeAddFace( volume, 16, 12, 20, 22, decVector( -1.0f, -1.0f, 0.0f ) );
			pVolumeAddFace( volume, 12, 13, 20, decVector( -1.0f, -1.0f, -1.0f ) );
			
			pVolumeAddFace( volume, 20, 21, 23, 22, decVector( 0.0f, -1.0f, 0.0f ) ); // bottom face
			
			// add volume to list
			pConvexVolumeList->AddVolume( volume );
			
		}catch( const deException & ){
			if( volume ) delete volume;
			throw;
		}
		
	}else{
		const decVector2 angles( pLight->GetSpotAngle() * 0.5f,
			pLight->GetSpotAngle() * pLight->GetSpotRatio() * 0.5f );
		
		float sinX = sinf( angles.x );
		float cosX = cosf( angles.x );
		float sinY = sinf( angles.y );
		float cosY = cosf( angles.y );
		const decMatrix matrix( pLight->GetMatrix() );
		
		try{
			// create convex volume
			volume = new decConvexVolume;
			if( ! volume ) DETHROW( deeOutOfMemory );
			
			// add vertices
			volume->AddVertex( matrix * decVector( -xn, yn, 0.0f ) );
			volume->AddVertex( matrix * decVector( xn, yn, 0.0f ) );
			volume->AddVertex( matrix * decVector( xn, -yn, 0.0f ) );
			volume->AddVertex( matrix * decVector( -xn, -yn, 0.0f ) );
			volume->AddVertex( matrix * decVector( -xf, yf, range ) );
			volume->AddVertex( matrix * decVector( xf, yf, range ) );
			volume->AddVertex( matrix * decVector( xf, -yf, range ) );
			volume->AddVertex( matrix * decVector( -xf, -yf, range ) );
			
			// add faces
			pVolumeAddFace( volume, 1, 5, 6, 2, matrix.TransformNormal( decVector( cosX, 0.0f, -sinX ) ) ); // right
			pVolumeAddFace( volume, 4, 0, 3, 7, matrix.TransformNormal( decVector( -cosX, 0.0f, -sinX ) ) ); // left
			pVolumeAddFace( volume, 4, 5, 1, 0, matrix.TransformNormal( decVector( 0.0f, cosY, -sinY ) ) ); // top
			pVolumeAddFace( volume, 3, 2, 6, 7, matrix.TransformNormal( decVector( 0.0f, -cosY, -sinY ) ) ); // bottom
			pVolumeAddFace( volume, 5, 4, 7, 6, matrix.TransformView() ); // back
			pVolumeAddFace( volume, 0, 1, 2, 3, -matrix.TransformView() ); // front
			
			// add volume to list
			pConvexVolumeList->AddVolume( volume );
			
		}catch( const deException & ){
			if( volume ) delete volume;
			throw;
		}
	}
}

bool deoglOptimizerLight::pCropLightVolume( int timeSlice ){
	decConvexVolume splitVolume;
	deoglDCollisionTriangle colTri;
	decConvexVolumeFace *face = NULL;
	float projectionDistance;
	decVector pf1, pf2, pf3;
	decVector closestPoint;
	int triangleCount;
	decVector normal;
	decTimer timer;
	int elapsed;
	
	// determine values required
	const decVector lightPosition = pLight->GetMatrix().GetPosition().ToVector();
	triangleCount = pTriangleSorter->GetTriangleCount();
	
	// protect this place
	try{
		// crop the volume until time runs out
		elapsed = timeSlice;
		while( elapsed > 0.0f && pNextTriangle < triangleCount ){
			// retrieves the triangle
			const decVector &p1 = pTriangleSorter->GetTriangleVertex1( pNextTriangle );
			const decVector &p2 = pTriangleSorter->GetTriangleVertex2( pNextTriangle );
			const decVector &p3 = pTriangleSorter->GetTriangleVertex3( pNextTriangle );
			
			// determine the closest point on the triangle to calculate the projection
			// distance. moving points this far away from the light source ensures that
			// that all volumes are croped
			colTri.SetCorners( p1, p2, p3 );
			closestPoint = colTri.ClosestPointTo( lightPosition ).ToVector();
			projectionDistance = ( pSafeRadius / ( closestPoint - lightPosition ).Length() ) * 1.01f;
			
			// calculate the projected triangle
			pf1 = p1 + ( p1 - lightPosition ) * projectionDistance;
			pf2 = p2 + ( p2 - lightPosition ) * projectionDistance;
			pf3 = p3 + ( p3 - lightPosition ) * projectionDistance;
			
			// prepare the split volume
			splitVolume.SetEmpty();
			
			// add vertices to convex volume
			splitVolume.AddVertex( p1 );
			splitVolume.AddVertex( p2 );
			splitVolume.AddVertex( p3 );
			splitVolume.AddVertex( pf1 );
			splitVolume.AddVertex( pf2 );
			splitVolume.AddVertex( pf3 );
			
			// add faces
			normal = ( p2 - p1 ) % ( p3 - p2 );
			pVolumeAddFace( &splitVolume, 0, 1, 2, normal ); // back
			pVolumeAddFace( &splitVolume, 5, 4, 3, -normal ); // front
			pVolumeAddFace( &splitVolume, 0, 3, 4, 1, ( p2 - p1 ) % ( lightPosition - p2 ) ); // side
			pVolumeAddFace( &splitVolume, 1, 4, 5, 2, ( p3 - p2 ) % ( lightPosition - p3 ) ); // side
			pVolumeAddFace( &splitVolume, 2, 5, 3, 0, ( p1 - p3 ) % ( lightPosition - p1 ) ); // side
			
			// split volumes
			pConvexVolumeList->SplitByVolume( splitVolume );
			
			// switch to the next triangle
			pNextTriangle++;
			
			// update the remaining time
			elapsed -= ( int )( timer.GetElapsedTime() * 1000000 );
			
			// should the volume count raise too high we better stop
			// since high volume counts are as bad as huge volumes
			//if( convexVolumeList->GetVolumeCount() > 20 ){
			//	pNextTriangle = triangleCount;
			//}
		}
		
	}catch( const deException & ){
		if( face ) delete face;
		throw;
	}
	
	// notify the light that the volume improved
	//pLight->LightVolumeImproved();
	
	// check if all triangles have been used up
	if( pNextTriangle == triangleCount ){
		// should the volume count raise too high we drop the new volumes
		// since high volume counts are as bad as huge volumes
		if( pConvexVolumeList->GetVolumeCount() > pMaxVolumeCount ){
			delete pConvexVolumeList;
			
		}else{
			// otherwise replace the light volume with the new one
			pLight->ReplaceLightVolume( pConvexVolumeList );
		}
		
		pConvexVolumeList = NULL;
		return false;
		
	}
	
	// otherwise we take another round
	return true;
}

bool deoglOptimizerLight::pOptimizeShadowCaster( int timeSlice ){
	// build a collide list
	if( pCasterStage == easBuildCollideList ){
		pSCBuildCollideList();
		return true;
		
	// test components if they are a shadow caster
	}else if( pCasterStage == easTestComponents ){
		//if( pSCTestComponents( timeSlice ) ) return true;
		return true;
		
	// build a shadow caster out of the found faces
	}else if( pCasterStage == easBuildShadowCaster ){
		pSCBuildShadowCaster();
//		printf( "[OGL-LIGHT] Optimizer ( %p ): shadow caster ready\n", pLight );
	}
	
	pCasterStage = easDone;
	return false;
}

void deoglOptimizerLight::pSCBuildCollideList(){
	// calculate some values required later and switch to next stage
	pNextTexture = 0;
	pNextFace = 0;
	pCasterStage = easTestComponents;
}

/*
bool deoglOptimizerLight::pSCTestComponents( int timeSlice ){
	pCasterStage = easBuildShadowCaster;
	
	return true;
}
*/

void deoglOptimizerLight::pSCBuildShadowCaster(){
	// create a new shadow caster if not existing already
	if( pShadowCaster ){
		pShadowCaster->Clear();
	}else{
		pShadowCaster = new deoglShadowCaster( pLight->GetRenderThread() );
		if( ! pShadowCaster ) DETHROW( deeOutOfMemory );
	}
	
	// build shadow caster
	pFinalCollideList->Clear();
	
	// replace the shadow caster int the light
	pLight->ReplaceShadowCaster( pShadowCaster );
	pShadowCaster = NULL;
	
	// our work is done now
	pCasterStage = easDone;
}

decVector deoglOptimizerLight::pGetSectorOffset( const decVector &sectorSize, const decPoint3 &sectorFrom, const decPoint3 &sectorTo ) const{
	return decVector( sectorSize.x * ( sectorTo.x - sectorFrom.x ),
		sectorSize.y * ( sectorTo.y - sectorFrom.y ),
		sectorSize.z * ( sectorTo.z - sectorFrom.z ) );
}

void deoglOptimizerLight::pVolumeAddFace( decConvexVolume *volume, int p1, int p2, int p3, const decVector &normal ){
	decConvexVolumeFace *face = NULL;
	decVector faceNormal;
	
	try{
		face = new decConvexVolumeFace;
		if( ! face ) DETHROW( deeOutOfMemory );
		
		faceNormal = normal;
		faceNormal.Normalize();
		face->SetNormal( faceNormal );
		
		face->AddVertex( p1 );
		face->AddVertex( p2 );
		face->AddVertex( p3 );
		
		volume->AddFace( face );
		
	}catch( const deException & ){
		if( face ) delete face;
		throw;
	}
}

void deoglOptimizerLight::pVolumeAddFace( decConvexVolume *volume, int p1, int p2, int p3, int p4, const decVector &normal ){
	decConvexVolumeFace *face = NULL;
	decVector faceNormal;
	
	try{
		face = new decConvexVolumeFace;
		if( ! face ) DETHROW( deeOutOfMemory );
		
		faceNormal = normal;
		faceNormal.Normalize();
		face->SetNormal( faceNormal );
		
		face->AddVertex( p1 );
		face->AddVertex( p2 );
		face->AddVertex( p3 );
		face->AddVertex( p4 );
		
		volume->AddFace( face );
		
	}catch( const deException & ){
		if( face ) delete face;
		throw;
	}
}
