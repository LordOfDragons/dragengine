/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglSkinRendered.h"
#include "deoglSkinRenderedTexture.h"
#include "../deoglRSkin.h"
#include "../deoglSkinTexture.h"
#include "../../component/deoglRComponent.h"
#include "../../component/deoglRComponentLOD.h"
#include "../../component/deoglRComponentTexture.h"
#include "../../model/deoglRModel.h"
#include "../../model/deoglModelLOD.h"
#include "../../model/face/deoglModelFace.h"
#include "../../rendering/light/deoglRenderGI.h"
#include "../../rendering/light/deoglRenderLight.h"
#include "../../rendering/plan/deoglRenderPlan.h"
#include "../../rendering/plan/deoglRenderPlanMasked.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"

#include <dragengine/common/exceptions.h>

#ifdef OS_W32
#undef near
#undef far
#endif



// Class deoglSkinRenderedTexture
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinRenderedTexture::deoglSkinRenderedTexture( deoglSkinRendered &skinRendered,
deoglRSkin &skin, int texture, int modelTexture ) :
pSkinRendered( skinRendered ),
pSkin( skin ),
pTexture( texture ),
pModelTexture( modelTexture ),
pPlan( NULL )
{
	pPlan = new deoglRenderPlan( skinRendered.GetRenderThread() );
}

deoglSkinRenderedTexture::~deoglSkinRenderedTexture(){
	if( pPlan ){
		delete pPlan;
	}
}



// Management
///////////////


void deoglSkinRenderedTexture::AddRenderPlans( deoglRenderPlan &plan ){
	const deoglSkinTexture &skinTexture = pSkin.GetTextureAt( pTexture );
	if( skinTexture.GetMirror() ){
		pMirrorAddRenderPlans( plan );
	}
}

void deoglSkinRenderedTexture::DropDelayedDeletionObjects(){
	pPlan->SetWorld( NULL );
}



// Private Functions
//////////////////////

void deoglSkinRenderedTexture::pMirrorAddRenderPlans( deoglRenderPlan &plan ){
	decDMatrix ownerMatrix;
	if( pSkinRendered.GetOwnerComponent() ){
		ownerMatrix = pSkinRendered.GetOwnerComponent()->GetMatrix();
		
	}else if( pSkinRendered.GetOwnerBillboard() ){
		// similar to what is set in the SPB parameters. needs to be turned into a function
		return;
		
	}else if( pSkinRendered.GetOwnerDecal() ){
		//ownerMatrix = pSkinRendered.GetOwnerDecal()->GetParentComponent()->GetMatrix();
		return;
		
	}else{
		return;
	}
	
	// get the plane parrameters. this is done first since if the plane is facing
	// away from the camera the entire mirror rendering can be skipped altogether
	decVector planeNormal;
	decVector planePosition;
	
	if( pSkinRendered.GetOwnerComponent() ){
		pPlaneFromTexture( planeNormal, planePosition );
		
	}else if( pSkinRendered.GetOwnerBillboard() ){
		// depends on billboard orientation
		
	}else if( pSkinRendered.GetOwnerDecal() ){
		// similar to component
	}
	
	/* doesn't work properly
	const deoglSkinTexture &texture = pSkin.GetTextureAt( pTexture );
	
	if( matrixMV.TransformNormal( planeNormal ).z > 0.0f ){
		pOgl->LogInfoFormat( "mirror texture %i: facing away from camera.", t );
		
	}else{
		pOgl->LogInfoFormat( "mirror texture %i: facing towards camera.", t );
	}
	*/
	
	decDVector mirrorNormal( ownerMatrix.TransformNormal( decDVector( planeNormal ) ) );
	mirrorNormal.Normalize();
	decDVector mirrorRefPoint( ownerMatrix * decDVector( planePosition ) );
	const double mirrorDot = mirrorNormal * mirrorRefPoint;
	
	// calculate mirror properties
	const decDMatrix &invCamMat = plan.GetInverseCameraMatrix();
	const decDVector cameraPos( invCamMat.GetPosition() );
	const decDVector cameraView( invCamMat.TransformView() );
	const decDVector cameraUp( invCamMat.TransformUp() );
	const decDVector cameraRight( invCamMat.TransformRight() );
	
	const decDVector mirrorPos( cameraPos - mirrorNormal * ( ( cameraPos * mirrorNormal - mirrorDot ) * 2.0 ) );
	const decDVector mirrorView( cameraView - mirrorNormal * ( ( cameraView * mirrorNormal ) * 2.0 ) );
	const decDVector mirrorUp( cameraUp - mirrorNormal * ( ( cameraUp * mirrorNormal ) * 2.0 ) );
	const decDVector mirrorRight( cameraRight - mirrorNormal * ( ( cameraRight * mirrorNormal ) * 2.0 ) );
	
	decDMatrix mirrorMatrix;
	mirrorMatrix.a11 = mirrorRight.x;
	mirrorMatrix.a12 = mirrorRight.y;
	mirrorMatrix.a13 = mirrorRight.z;
	mirrorMatrix.a14 = -( mirrorPos * mirrorRight );
	mirrorMatrix.a21 = mirrorUp.x;
	mirrorMatrix.a22 = mirrorUp.y;
	mirrorMatrix.a23 = mirrorUp.z;
	mirrorMatrix.a24 = -( mirrorPos * mirrorUp );
	mirrorMatrix.a31 = mirrorView.x;
	mirrorMatrix.a32 = mirrorView.y;
	mirrorMatrix.a33 = mirrorView.z;
	mirrorMatrix.a34 = -( mirrorPos * mirrorView );
	mirrorMatrix.a41 = 0.0;
	mirrorMatrix.a42 = 0.0;
	mirrorMatrix.a43 = 0.0;
	mirrorMatrix.a44 = 1.0;
	
	mirrorNormal = mirrorMatrix.TransformNormal( mirrorNormal );
	mirrorRefPoint = mirrorMatrix * mirrorRefPoint;
	
	// set the plan properties
	pPlan->SetWorld( plan.GetWorld() );
	pPlan->SetCamera( NULL ); // since this is a fake camera
	pPlan->CopyCameraParametersFrom( plan );
	pPlan->SetCameraMatrix( mirrorMatrix );
	pPlan->UpdateRefPosCameraMatrix();
	
	pPlan->SetFlipCulling( ! plan.GetFlipCulling() );
	pPlan->SetNoRenderedOccMesh( true );
	
	pPlan->SetNoReflections( true ); // HACK prevent re-entrant problem for the time being
	
	// use the parent plan gi state but without modifying it. allows to use GI with
	// no extra cost and witout messing up parent GI state. for mirrors this is good
	// enough since mirrors need to be in view of the parent camera
	pPlan->SetUseConstGIState( pSkinRendered.GetRenderThread().GetRenderers().GetLight().GetRenderGI().GetRenderGIState( plan ) );
	pPlan->SetUseGIState( pPlan->GetUseConstGIState() != NULL );
	
	// calculate the frustum for this texture
	//matrixMVP = ( ownerMatrix * pPlan->GetCameraMatrix() ).ToMatrix() * pPlan->GetProjectionMatrix();
	const decDMatrix matrixInvCamera( decDMatrix::CreateScale( -1.0, 1.0, 1.0 ) * pPlan->GetInverseCameraMatrix() );
	//matrixInvCamera = mirrorMatrix;
	
	if( pSkinRendered.GetOwnerComponent() ){
		const decMatrix matrixMVP( ( ownerMatrix * plan.GetCameraMatrix() ).ToMatrix() * plan.GetProjectionMatrix() );
		double projX = 1.0 / ( double )plan.GetProjectionMatrix().a11;
		double projY = 1.0 / ( double )plan.GetProjectionMatrix().a22;
		double projNear = ( double )plan.GetCameraImageDistance();
		double projFar = ( double )plan.GetCameraViewDistance();
		const int width = plan.GetViewportWidth();
		const int height = plan.GetViewportHeight();
		
		pFrustumFromTexture( width, height, projX, projY, projNear, projFar, matrixInvCamera, matrixMVP );
		
	}else{
		// for billboard the billboard size has to be used
		// 
		// for decal something similar to component is used
	}
	
	// add the plan
	deoglRenderPlanMasked &maskedPlan = *plan.AddMaskedPlanFor( pPlan );
	
	maskedPlan.SetComponent( pSkinRendered.GetOwnerComponent(), pModelTexture );
	
	maskedPlan.SetUseClipPlane( true );
	maskedPlan.SetClipNormal( mirrorNormal.ToVector() );
	maskedPlan.SetClipDistance( mirrorNormal * mirrorRefPoint );
	
	// prepare the just added plan for rendering
	pPlan->PrepareRender();
}

void deoglSkinRenderedTexture::pPlaneFromTexture( decVector &planeNormal, decVector &planePosition ) const{
	if( ! pSkinRendered.GetOwnerComponent() ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRComponent &component = *pSkinRendered.GetOwnerComponent();
	if( ! component.GetModel() ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRComponentLOD &componentLOD = component.GetLODAt( 0 );
	componentLOD.PrepareNormalsTangents();
	const oglVector * const compFaceNormals = componentLOD.GetFaceNormals();
	const oglVector * const compPositions = componentLOD.GetPositions();
	
	const deoglModelLOD &modelLOD = component.GetModel()->GetLODAt( 0 );
	const oglModelVertex * const modelVertices = modelLOD.GetVertices();
	deoglModelFace * const faces = modelLOD.GetFaces();
	const int faceCount = modelLOD.GetFaceCount();
	int i;
	
	int planePositionCount = 0;
	
	planeNormal.SetZero();
	
	// hack for the time being
	if( component.GetRenderMode() == deoglRComponent::ermDynamic ){
		for( i=0; i<faceCount; i++ ){
			const deoglModelFace &face = faces[ i ];
			
			if( face.GetTexture() == pModelTexture ){
				const oglVector &v1 = compPositions[ modelVertices[ face.GetVertex1() ].position ];
				planePosition += decVector( v1.x, v1.y, v1.z );
				planePositionCount++;
				
				const oglVector &v2 = compPositions[ modelVertices[ face.GetVertex2() ].position ];
				planePosition += decVector( v2.x, v2.y, v2.z );
				planePositionCount++;
				
				const oglVector &v3 = compPositions[ modelVertices[ face.GetVertex3() ].position ];
				planePosition += decVector( v3.x, v3.y, v3.z );
				planePositionCount++;
				
				const oglVector &fn = compFaceNormals[ i ];
				planeNormal += decVector( fn.x, fn.y, fn.z );
			}
		}
		
	}else{
		for( i=0; i<faceCount; i++ ){
			const oglModelPosition * const positions = modelLOD.GetPositions();
			const deoglModelFace &face = faces[ i ];
			
			if( face.GetTexture() == pModelTexture ){
				planePosition += positions[ modelVertices[ face.GetVertex1() ].position ].position;
				planePositionCount++;
				
				planePosition += positions[ modelVertices[ face.GetVertex2() ].position ].position;
				planePositionCount++;
				
				planePosition += positions[ modelVertices[ face.GetVertex3() ].position ].position;
				planePositionCount++;
				
				planeNormal += face.GetFaceNormal();
			}
		}
	}
	
	float length = planeNormal.Length();
	if( length != 0.0f ){
		planeNormal /= length;
		
	}else{
		planeNormal.Set( 0.0f, 0.0f, 1.0f );
	}
	
	if( planePositionCount > 0 ){
		planePosition /= ( float )planePositionCount;
	}
}

void deoglSkinRenderedTexture::pFrustumFromTexture( int width, int height, double projX, double projY,
double near, double far, const decDMatrix &matrixInvCamera, const decMatrix &matrixMVP ) const{
	if( ! pSkinRendered.GetOwnerComponent() ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRComponent &component = *pSkinRendered.GetOwnerComponent();
	if( ! component.GetModel() ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRComponentLOD &componentLOD = component.GetLODAt( 0 );
	const deoglModelLOD &modelLOD = component.GetModel()->GetLODAt( 0 );
	const oglModelVertex * const modelVertices = modelLOD.GetVertices();
	deoglModelFace * const faces = modelLOD.GetFaces();
	int f, faceCount = modelLOD.GetFaceCount();
	
	float left = 1.0f;
	float top = -1.0f;
	float right = -1.0f;
	float bottom = 1.0f;
	decVector4 vector;
	
	// hack for the time being
	if( component.GetRenderMode() == deoglRComponent::ermDynamic ){
		componentLOD.PreparePositions();
		const oglVector * const compVertices = componentLOD.GetPositions();
		
		for( f=0; f<faceCount; f++ ){
			const deoglModelFace &face = faces[ f ];
			
			if( face.GetTexture() == pModelTexture ){
				const oglVector &v1 = compVertices[ modelVertices[ face.GetVertex1() ].position ];
				const oglVector &v2 = compVertices[ modelVertices[ face.GetVertex2() ].position ];
				const oglVector &v3 = compVertices[ modelVertices[ face.GetVertex3() ].position ];
				
				// vertex 1
				vector = matrixMVP * decVector4( v1.x, v1.y, v1.z, 1.0f );
				if( vector.w > 0.0f ){
					vector.x /= vector.w;
					vector.y /= vector.w;
					
					if( vector.x < left ) left = vector.x;
					if( vector.x > right ) right = vector.x;
					if( vector.y > top ) top = vector.y;
					if( vector.y < bottom ) bottom = vector.y;
				}
				
				// vertex 2
				vector = matrixMVP * decVector4( v2.x, v2.y, v2.z, 1.0f );
				if( vector.w > 0.0f ){
					vector.x /= vector.w;
					vector.y /= vector.w;
					
					if( vector.x < left ) left = vector.x;
					if( vector.x > right ) right = vector.x;
					if( vector.y > top ) top = vector.y;
					if( vector.y < bottom ) bottom = vector.y;
				}
				
				// vertex 3
				vector = matrixMVP * decVector4( v3.x, v3.y, v3.z, 1.0f );
				if( vector.w > 0.0f ){
					vector.x /= vector.w;
					vector.y /= vector.w;
					
					if( vector.x < left ) left = vector.x;
					if( vector.x > right ) right = vector.x;
					if( vector.y > top ) top = vector.y;
					if( vector.y < bottom ) bottom = vector.y;
				}
			}
		}
		
	}else{
		for( f=0; f<faceCount; f++ ){
			const oglModelPosition * const positions = modelLOD.GetPositions();
			const deoglModelFace &face = faces[ f ];
			
			if( face.GetTexture() == pModelTexture ){
				const decVector &v1 = positions[ modelVertices[ face.GetVertex1() ].position ].position;
				const decVector &v2 = positions[ modelVertices[ face.GetVertex2() ].position ].position;
				const decVector &v3 = positions[ modelVertices[ face.GetVertex3() ].position ].position;
				
				// vertex 1
				vector = matrixMVP * decVector4( v1.x, v1.y, v1.z, 1.0f );
				if( vector.w > 0.0f ){
					vector.x /= vector.w;
					vector.y /= vector.w;
					
					if( vector.x < left ) left = vector.x;
					if( vector.x > right ) right = vector.x;
					if( vector.y > top ) top = vector.y;
					if( vector.y < bottom ) bottom = vector.y;
				}
				
				// vertex 2
				vector = matrixMVP * decVector4( v2.x, v2.y, v2.z, 1.0f );
				if( vector.w > 0.0f ){
					vector.x /= vector.w;
					vector.y /= vector.w;
					
					if( vector.x < left ) left = vector.x;
					if( vector.x > right ) right = vector.x;
					if( vector.y > top ) top = vector.y;
					if( vector.y < bottom ) bottom = vector.y;
				}
				
				// vertex 3
				vector = matrixMVP * decVector4( v3.x, v3.y, v3.z, 1.0f );
				if( vector.w > 0.0f ){
					vector.x /= vector.w;
					vector.y /= vector.w;
					
					if( vector.x < left ) left = vector.x;
					if( vector.x > right ) right = vector.x;
					if( vector.y > top ) top = vector.y;
					if( vector.y < bottom ) bottom = vector.y;
				}
			}
		}
	}
	
	if( left < -1.0f ) left = -1.0f;
	if( right > 1.0f ) right = 1.0f;
	if( top > 1.0f ) top = 1.0f;
	if( bottom < -1.0f ) bottom = -1.0f;
	
	if( left < right && top > bottom ){
		double sleft = projX * ( double )left;
		double stop = projY * ( double )top;
		double sright = projX * ( double )right;
		double sbottom = projY * ( double )bottom;
		
		decDVector topLeft( matrixInvCamera * decDVector( sleft, stop, 1.0 ) );
		decDVector topRight( matrixInvCamera * decDVector( sright, stop, 1.0 ) );
		decDVector bottomLeft( matrixInvCamera * decDVector( sleft, sbottom, 1.0 ) );
		decDVector bottomRight( matrixInvCamera * decDVector( sright, sbottom, 1.0 ) );
		
		double screenScaleX = ( double )width * 0.5;
		double screenScaleY = ( double )height * 0.5;
		
		decBoundary screenArea;
		
		pPlan->SetCustomFrustumBoundaries( matrixInvCamera.GetPosition(),
			topLeft, topRight, bottomLeft, bottomRight, near, far );
		
		/*
		double denominator = 1.0 / ( double )( far - near );
		decDMatrix mp;
		mp.a11 = 2.0 / ( double )( right - left );
		mp.a12 = 0.0;
		mp.a13 = 0.0;
		mp.a14 = 0.0;
		mp.a21 = 0.0;
		mp.a22 = 2.0 / ( double )( top - bottom );
		mp.a23 = 0.0;
		mp.a24 = 0.0;
		mp.a31 = 0.0;
		mp.a32 = 0.0;
		mp.a33 = ( double )( far + near ) * denominator;
		mp.a34 = -2.0 * ( double )( far * near ) * denominator;
		mp.a41 = 0.0;
		mp.a42 = 0.0;
		mp.a43 = 1.0;
		mp.a44 = 0.0;
		plan.SetCustomFrustumMatrix( matrixInvCamera * mp );
		*/
		
		screenArea.x1 = ( int )( screenScaleX * ( left + 1.0 ) );
		screenArea.y1 = ( int )( screenScaleY * ( 1.0 - top ) );
		screenArea.x2 = ( int )( screenScaleX * ( right + 1.0 ) );
		screenArea.y2 = ( int )( screenScaleY * ( 1.0 - bottom ) );
		pPlan->SetCustomFrustumScreenArea( screenArea );
		
		pPlan->SetUseCustomFrustum( false/*true*/ ); // for some unknown reason this simply doesn't work
		
		//deoglConfiguration &config = *pOgl->GetConfiguration();
		//if( config.GetDebugSnapshot() == 6 ){
		//	pOgl->LogInfoFormat( "frustum from texture: f=(%.3f,%.3f,%.3f,%.3f) s=(%i,%i,%i,%i)", left, top, right, bottom,
		//		screenArea.x1, screenArea.y1, screenArea.x2, screenArea.y2 );
		//	config.SetDebugSnapshot( 0 );
		//}
		
	}else{
		pPlan->SetUseCustomFrustum( false );
	}
}
