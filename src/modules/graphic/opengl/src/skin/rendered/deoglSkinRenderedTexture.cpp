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
	sMirrorMatrix mirrorMatrix;
	
	if( pSkinRendered.GetOwnerComponent() ){
		mirrorMatrix.ownerMatrix = pSkinRendered.GetOwnerComponent()->GetMatrix();
		
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
	if( pSkinRendered.GetOwnerComponent() ){
		pPlaneFromTexture( mirrorMatrix );
		
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
	
	pMirrorMatrix( plan.GetInverseCameraMatrix(), mirrorMatrix );
	
	// set the plan properties
	pPlan->SetWorld( plan.GetWorld() );
	pPlan->SetCamera( nullptr ); // since this is a fake camera
	pPlan->CopyCameraParametersFrom( plan );
	pPlan->SetCameraMatrix( mirrorMatrix.mirrorMatrix );
	pPlan->SetCameraMatrixNonMirrored( mirrorMatrix.mirrorFreeMatrix );
	pPlan->UpdateRefPosCameraMatrix();
	
	pPlan->SetFlipCulling( ! plan.GetFlipCulling() );
	pPlan->SetNoRenderedOccMesh( true );
	
	pPlan->SetNoReflections( true ); // HACK prevent re-entrant problem for the time being
	
	// use the parent plan gi state but without modifying it. allows to use GI with
	// no extra cost and witout messing up parent GI state. for mirrors this is good
	// enough since mirrors need to be in view of the parent camera
	pPlan->SetUseConstGIState( plan.GetRenderGIState() );
	pPlan->SetUseGIState( pPlan->GetUseConstGIState() != NULL );
	
	// calculate the frustum for this texture
	//matrixMVP = ( ownerMatrix * pPlan->GetCameraMatrix() ).ToMatrix() * pPlan->GetProjectionMatrix();
	const decDMatrix matrixInvCamera( decDMatrix::CreateScale( -1.0, 1.0, 1.0 ) * pPlan->GetInverseCameraMatrix() );
	//matrixInvCamera = mirrorMatrix;
	
	if( pSkinRendered.GetOwnerComponent() ){
		const decMatrix matrixMVP( ( mirrorMatrix.ownerMatrix * plan.GetCameraMatrix() ).ToMatrix() * plan.GetProjectionMatrix() );
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
	maskedPlan.SetClipNormal( mirrorMatrix.mirrorNormal.ToVector() );
	maskedPlan.SetClipDistance( ( float )( mirrorMatrix.mirrorNormal * mirrorMatrix.mirrorRefPoint ) );
	
	// stereo rendering
	if( plan.GetRenderStereo() ){
		pPlan->SetRenderStereo( true );
		
		sMirrorMatrix mirrorMatrixStereo( mirrorMatrix );
		
		pMirrorMatrix( plan.GetCameraMatrix().QuickMultiply(
			plan.GetCameraStereoMatrix() ).QuickInvert(), mirrorMatrixStereo );
		
		// camMatStereo = invert(invert(camMatStereo) * camMat)
		//              = invert(camMat) * invert(invert(camMatStereo))
		//              = invert(camMat) * camMatStereo
		pPlan->SetCameraStereoMatrix( mirrorMatrix.mirrorMatrix.QuickInvert().
			QuickMultiply( mirrorMatrixStereo.mirrorMatrix ) );
		
		// mirror-free matrix is not used by anybody. we skipp it for the time being
		//pPlan->SetCameraStereoMatrixNonMirrored( mirrorMatrix.mirrorFreeMatrix );
		//pPlan->UpdateRefPosCameraStereoMatrix();
		
		// we use only the left eye frustum for the time being
		//pFrustumFromTexture( width, height, projX, projY, projNear, projFar, matrixInvCamera, matrixMVP );
		
		maskedPlan.SetClipNormalStereo( mirrorMatrixStereo.mirrorNormal.ToVector() );
		maskedPlan.SetClipDistanceStereo( ( float )( mirrorMatrixStereo.mirrorNormal * mirrorMatrixStereo.mirrorRefPoint ) );
		
	}else{
		pPlan->SetRenderStereo( false );
	}
	
	// prepare the just added plan for rendering
	pPlan->PrepareRender( &maskedPlan );
}

void deoglSkinRenderedTexture::pPlaneFromTexture( sMirrorMatrix &mirrorMatrix ) const{
	if( ! pSkinRendered.GetOwnerComponent() ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRComponent &component = *pSkinRendered.GetOwnerComponent();
	deoglRComponentLOD &componentLOD = component.GetLODAt( 0 );
	componentLOD.PrepareNormalsTangents();
	const oglVector3 * const compFaceNormals = componentLOD.GetFaceNormals();
	const oglVector3 * const compPositions = componentLOD.GetPositions();
	
	const deoglModelLOD &modelLOD = component.GetModel()->GetLODAt( 0 );
	const oglModelVertex * const modelVertices = modelLOD.GetVertices();
	deoglModelFace * const faces = modelLOD.GetFaces();
	const int faceCount = modelLOD.GetFaceCount();
	int i;
	
	int planePositionCount = 0;
	
	mirrorMatrix.planeNormal.SetZero();
	
	// hack for the time being
	if( component.GetRenderMode() == deoglRComponent::ermDynamic ){
		for( i=0; i<faceCount; i++ ){
			const deoglModelFace &face = faces[ i ];
			
			if( face.GetTexture() == pModelTexture ){
				const oglVector3 &v1 = compPositions[ modelVertices[ face.GetVertex1() ].position ];
				mirrorMatrix.planePosition += decVector( v1.x, v1.y, v1.z );
				planePositionCount++;
				
				const oglVector3 &v2 = compPositions[ modelVertices[ face.GetVertex2() ].position ];
				mirrorMatrix.planePosition += decVector( v2.x, v2.y, v2.z );
				planePositionCount++;
				
				const oglVector3 &v3 = compPositions[ modelVertices[ face.GetVertex3() ].position ];
				mirrorMatrix.planePosition += decVector( v3.x, v3.y, v3.z );
				planePositionCount++;
				
				const oglVector3 &fn = compFaceNormals[ i ];
				mirrorMatrix.planeNormal += decVector( fn.x, fn.y, fn.z );
			}
		}
		
	}else{
		for( i=0; i<faceCount; i++ ){
			const oglModelPosition * const positions = modelLOD.GetPositions();
			const deoglModelFace &face = faces[ i ];
			
			if( face.GetTexture() == pModelTexture ){
				mirrorMatrix.planePosition += positions[ modelVertices[ face.GetVertex1() ].position ].position;
				planePositionCount++;
				
				mirrorMatrix.planePosition += positions[ modelVertices[ face.GetVertex2() ].position ].position;
				planePositionCount++;
				
				mirrorMatrix.planePosition += positions[ modelVertices[ face.GetVertex3() ].position ].position;
				planePositionCount++;
				
				mirrorMatrix.planeNormal += face.GetFaceNormal();
			}
		}
	}
	
	float length = mirrorMatrix.planeNormal.Length();
	if( length != 0.0f ){
		mirrorMatrix.planeNormal /= length;
		
	}else{
		mirrorMatrix.planeNormal.Set( 0.0f, 0.0f, 1.0f );
	}
	
	if( planePositionCount > 0 ){
		mirrorMatrix.planePosition /= ( float )planePositionCount;
	}
}

void deoglSkinRenderedTexture::pFrustumFromTexture( int width, int height, double projX, double projY,
double near, double far, const decDMatrix &matrixInvCamera, const decMatrix &matrixMVP ) const{
	if( ! pSkinRendered.GetOwnerComponent() ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRComponent &component = *pSkinRendered.GetOwnerComponent();
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
		const oglVector3 * const compVertices = componentLOD.GetPositions();
		
		for( f=0; f<faceCount; f++ ){
			const deoglModelFace &face = faces[ f ];
			
			if( face.GetTexture() == pModelTexture ){
				const oglVector3 &v1 = compVertices[ modelVertices[ face.GetVertex1() ].position ];
				const oglVector3 &v2 = compVertices[ modelVertices[ face.GetVertex2() ].position ];
				const oglVector3 &v3 = compVertices[ modelVertices[ face.GetVertex3() ].position ];
				
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

void deoglSkinRenderedTexture::pMirrorMatrix( const decDMatrix &invCamMatrix, sMirrorMatrix &mirrorMatrix ){
	mirrorMatrix.mirrorNormal = mirrorMatrix.ownerMatrix.TransformNormal( mirrorMatrix.planeNormal );
	mirrorMatrix.mirrorNormal.Normalize();
	mirrorMatrix.mirrorRefPoint = mirrorMatrix.ownerMatrix * mirrorMatrix.planePosition;
	const double mirrorDot = mirrorMatrix.mirrorNormal * mirrorMatrix.mirrorRefPoint;
	
	// calculate mirror properties
	const decDVector cameraPos( invCamMatrix.GetPosition() );
	const decDVector cameraView( invCamMatrix.TransformView() );
	const decDVector cameraUp( invCamMatrix.TransformUp() );
	const decDVector cameraRight( invCamMatrix.TransformRight() );
	
	const decDVector mirrorPos( cameraPos - mirrorMatrix.mirrorNormal
		* ( ( cameraPos * mirrorMatrix.mirrorNormal - mirrorDot ) * 2.0 ) );
	const decDVector mirrorView( cameraView - mirrorMatrix.mirrorNormal
		* ( ( cameraView * mirrorMatrix.mirrorNormal ) * 2.0 ) );
	const decDVector mirrorUp( cameraUp - mirrorMatrix.mirrorNormal
		* ( ( cameraUp * mirrorMatrix.mirrorNormal ) * 2.0 ) );
	const decDVector mirrorRight( cameraRight - mirrorMatrix.mirrorNormal
		* ( ( cameraRight * mirrorMatrix.mirrorNormal ) * 2.0 ) );
	
	mirrorMatrix.mirrorMatrix.a11 = mirrorRight.x;
	mirrorMatrix.mirrorMatrix.a12 = mirrorRight.y;
	mirrorMatrix.mirrorMatrix.a13 = mirrorRight.z;
	mirrorMatrix.mirrorMatrix.a14 = -( mirrorPos * mirrorRight );
	mirrorMatrix.mirrorMatrix.a21 = mirrorUp.x;
	mirrorMatrix.mirrorMatrix.a22 = mirrorUp.y;
	mirrorMatrix.mirrorMatrix.a23 = mirrorUp.z;
	mirrorMatrix.mirrorMatrix.a24 = -( mirrorPos * mirrorUp );
	mirrorMatrix.mirrorMatrix.a31 = mirrorView.x;
	mirrorMatrix.mirrorMatrix.a32 = mirrorView.y;
	mirrorMatrix.mirrorMatrix.a33 = mirrorView.z;
	mirrorMatrix.mirrorMatrix.a34 = -( mirrorPos * mirrorView );
	mirrorMatrix.mirrorMatrix.a41 = 0.0;
	mirrorMatrix.mirrorMatrix.a42 = 0.0;
	mirrorMatrix.mirrorMatrix.a43 = 0.0;
	mirrorMatrix.mirrorMatrix.a44 = 1.0;
	
	mirrorMatrix.mirrorNormal = mirrorMatrix.mirrorMatrix.TransformNormal( mirrorMatrix.mirrorNormal );
	mirrorMatrix.mirrorRefPoint = mirrorMatrix.mirrorMatrix * mirrorMatrix.mirrorRefPoint;
	
	mirrorMatrix.mirrorFreeMatrix = mirrorMatrix.mirrorMatrix;
	mirrorMatrix.mirrorFreeMatrix.a11 = -mirrorMatrix.mirrorMatrix.a11;
	mirrorMatrix.mirrorFreeMatrix.a12 = -mirrorMatrix.mirrorMatrix.a12;
	mirrorMatrix.mirrorFreeMatrix.a13 = -mirrorMatrix.mirrorMatrix.a13;
	mirrorMatrix.mirrorFreeMatrix.a14 = -mirrorMatrix.mirrorMatrix.a14;
}
