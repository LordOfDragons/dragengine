/* 
 * Drag[en]gine OpenXR VR Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <stdlib.h>
#include <string.h>

#include "deoxrHiddenMesh.h"
#include "deoxrSystem.h"
#include "deoxrInstance.h"
#include "deVROpenXR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/model/deModelBuilder.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelTextureCoordinatesSet.h>



// Class deoxrHiddenMesh
///////////////////////////

// Constructor, destructor
////////////////////////////

deoxrHiddenMesh::deoxrHiddenMesh( deoxrSession &session,
	XrViewConfigurationType viewConfig, uint32_t viewIndex ) :
pSession( session ),
pViewConfig( viewConfig ),
pViewIndex( viewIndex ){
}

deoxrHiddenMesh::~deoxrHiddenMesh(){
}



// Management
///////////////

void deoxrHiddenMesh::SetFov( XrFovf fov ){
	pFov = fov;
}



class deoxrHiddenMesh_BuildModel : public deModelBuilder {
private:
	const XrVisibilityMaskKHR &pMask;
	
public:
	deoxrHiddenMesh_BuildModel( const XrVisibilityMaskKHR &mask ) : pMask( mask ){}
	
	virtual void BuildModel( deModel *model ){
		deModelLOD * const lod = new deModelLOD;
		model->AddLOD( lod );
		
		model->AddTexture( new deModelTexture( "material", 256, 256 ) );
		model->GetTextureAt( 0 )->SetDoubleSided( true );
		
		// add vertices
		int i;
		const int vertexCount = ( int )pMask.vertexCountOutput;
		
		lod->SetVertexCount( vertexCount );
		deModelVertex * const vertices = lod->GetVertices();
		
		for( i=0; i<vertexCount; i++ ){
			const XrVector2f &p = pMask.vertices[ i ];
			vertices[ i ].SetPosition( decVector( p.x, p.y, 0.0f ) );
		}
		
		lod->SetNormalCount( 1 );
		lod->SetTangentCount( 1 );
		
		// add faces
		const int faceCount = ( int )( pMask.indexCountOutput / 3 );
		
		lod->SetFaceCount( faceCount );
		deModelFace * const faces = lod->GetFaces();
		
		for( i=0; i<faceCount; i++ ){
			faces[ i ].SetTexture( 0 );
			
			faces[ i ].SetVertex1( ( int )pMask.indices[ i * 3 ] );
			faces[ i ].SetVertex2( ( int )pMask.indices[ i * 3 + 1 ] );
			faces[ i ].SetVertex3( ( int )pMask.indices[ i * 3 + 2 ] );
			
			faces[ i ].SetNormal1( 0 );
			faces[ i ].SetNormal2( 0 );
			faces[ i ].SetNormal3( 0 );
			
			faces[ i ].SetTangent1( 0 );
			faces[ i ].SetTangent2( 0 );
			faces[ i ].SetTangent3( 0 );
			
			faces[ i ].SetTextureCoordinates1( 0 );
			faces[ i ].SetTextureCoordinates2( 0 );
			faces[ i ].SetTextureCoordinates3( 0 );
		}
		
		// add texture coordinates
		model->GetTextureCoordinatesSetList().Add( "default" );
		
		lod->SetTextureCoordinatesCount( 1 );
		
		lod->SetTextureCoordinatesSetCount( 1 );
		deModelTextureCoordinatesSet &tcset = lod->GetTextureCoordinatesSetAt( 0 );
		
		tcset.SetTextureCoordinatesCount( 1 );
		tcset.SetTextureCoordinatesAt( 0, decVector2() );
	}
};

void deoxrHiddenMesh::UpdateModel(){
	pModel = nullptr;
	
	deVROpenXR &oxr = pSession.GetSystem().GetInstance().GetOxr();
	
	oxr.LogInfoFormat( "Loading hidden mesh for view config %d for view %d", pViewConfig, pViewIndex );
	
	XrVisibilityMaskKHR mask;
	memset( &mask, 0, sizeof( mask ) );
	mask.type = XR_TYPE_VISIBILITY_MASK_KHR;
	
	try{
		// fetch and process mask
		pFetchData( mask );
		
		if( mask.vertexCountOutput == 0 || mask.indexCountOutput == 0 ){
			return;
		}
		
		pProjectVertices( mask );
		pMapVerticesToWindow( mask );
		pFitVertices( mask );
		
		// create model
		decString path;
		path.Format( "/openxr/hiddenMesh/config%d_view%d.demodel", pViewConfig, pViewIndex );
		
		deoxrHiddenMesh_BuildModel builder( mask );
		pModel.TakeOver( oxr.GetGameEngine()->GetModelManager()->CreateModel( path, builder ) );
		
		delete [] mask.vertices;
		delete [] mask.indices;
		
	}catch( const deException & ){
		delete [] mask.vertices;
		delete [] mask.indices;
		throw;
	}
}



// Private Functions
//////////////////////

void deoxrHiddenMesh::pFetchData( XrVisibilityMaskKHR &mask ) const{
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	deVROpenXR &oxr = instance.GetOxr();
	
	// get counts
	OXR_CHECK( oxr, instance.xrGetVisibilityMaskKHR( pSession.GetSession(),
		pViewConfig, pViewIndex, XR_VISIBILITY_MASK_TYPE_HIDDEN_TRIANGLE_MESH_KHR, &mask ) );
	
	if( mask.vertexCountOutput == 0 || mask.indexCountOutput == 0 ){
		return;
	}
	
	// allocate temporary memory and get data
	mask.vertices = new XrVector2f[ mask.vertexCountOutput ];
	mask.indices = new uint32_t[ mask.indexCountOutput ];
	
	mask.vertexCapacityInput = mask.vertexCountOutput;
	mask.indexCapacityInput = mask.indexCountOutput;
	
	OXR_CHECK( oxr, instance.xrGetVisibilityMaskKHR( pSession.GetSession(),
		pViewConfig, pViewIndex, XR_VISIBILITY_MASK_TYPE_HIDDEN_TRIANGLE_MESH_KHR, &mask ) );
}

void deoxrHiddenMesh::pProjectVertices( XrVisibilityMaskKHR &mask ) const{
	// calculate scaling. according to specification the returned triangle mesh
	// is an actual 3d mesh with the implicit z coordinate -1 . the graphic module
	// expects the mesh though in image space. we have to calculate the projection
	// to the image plane to apply the correct scaling to fix the values
	// 
	// since z is -1 (or rather 1 in dragengine coordinate system) the division by
	// w is not required. we only have to deal with the x and y scaling which is:
	// 
	//   idx = 1 / (projection.right - projection.left)
	//   idy = 1 / (projection.bottom - projection.top)
	//   sx = projection.right + projection.left
	//   sy = projection.bottom + projection.top
	//   
	//   a11 = 2 * idx
	//   a13 = -sx * idx
	//   a22 = 2 * idy
	//   a23 = -sy * idy
	// 
	//   x' = x*a11 + z*a13 = x*a11 + a13
	//   y' = y*a22 + z*a23 = y*a22 + a23
	// 
	// keep in mind openxr uses angles for projection so the projection values above
	// are actually tan(x) (and up/down is flipped).
	// 
	// no idea why they made it so complicated. it is a view hidden mesh. a 2d mesh
	// would have been more than enough
	const float projLeft = tanf( pFov.angleLeft );
	const float projRight = tanf( pFov.angleRight );
	const float projTop = tanf( -pFov.angleUp );
	const float projBottom = tanf( -pFov.angleDown );
	
	const float idx = 1.0f / ( projRight - projLeft );
	const float idy = 1.0f / ( projBottom - projTop );
	const float sx = projRight + projLeft;
	const float sy = projBottom + projTop;
	
	const decVector2 scale( 2.0f * idx, 2.0f * idy );
	const decVector2 offset( -sx * idx, -sy * idy );
	
	// project vertices
	uint32_t i;
	for( i=0; i<mask.vertexCountOutput; i++ ){
		XrVector2f &p = mask.vertices[ i ];
		p.x = p.x * scale.x + offset.x;
		p.y = p.y * scale.y + offset.y;
	}
}

void deoxrHiddenMesh::pMapVerticesToWindow( XrVisibilityMaskKHR &mask ) const{
	uint32_t i;
	for( i=0; i<mask.vertexCountOutput; i++ ){
		XrVector2f &p = mask.vertices[ i ];
		p.x = p.x * 0.5f + 0.5f;
		p.y = p.y * 0.5f + 0.5f;
	}
}

void deoxrHiddenMesh::pFitVertices( XrVisibilityMaskKHR &mask ) const{
	// hidden mask returned by openxr are often slightly off by a pixel or two.
	// this leaves small gaps which are rendered which is suboptimal.
	// stretch the mask to fit tightly into the window.
	float minX = 0.5f, maxX = 0.5f, minY = 0.5f, maxY = 0.5f;
	uint32_t i;
	
	for( i=0; i<mask.vertexCountOutput; i++ ){
		const XrVector2f &p = mask.vertices[ i ];
		
		if( p.x < minX ){
			minX = p.x;
			
		}else if( p.x > maxX ){
			maxX = p.x;
		}
		
		if( p.y < minY ){
			minY = p.y;
			
		}else if( p.y > maxY ){
			maxY = p.y;
		}
	}
	
	const float scaleX = 1.0f / ( maxX - minX );
	const float scaleY = 1.0f / ( maxY - minY );
	const float offsetX = scaleX * minX;
	const float offsetY = scaleY * minY;
	
	for( i=0; i<mask.vertexCountOutput; i++ ){
		XrVector2f &p = mask.vertices[ i ];
		p.x = p.x * scaleX - offsetX;
		p.y = p.y * scaleY - offsetY;
	}
}
