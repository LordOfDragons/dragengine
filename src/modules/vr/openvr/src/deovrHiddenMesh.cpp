/* 
 * Drag[en]gine OpenVR VR Module
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

#include "deovrHiddenMesh.h"
#include "deVROpenVR.h"

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



// Class deovrHiddenMesh
///////////////////////////

// Constructor, destructor
////////////////////////////

deovrHiddenMesh::deovrHiddenMesh( deVROpenVR &ovr, vr::Hmd_Eye eye ) :
pOvr( ovr ),
pEye( eye )
{
	pLoadHiddenMesh();
}

deovrHiddenMesh::~deovrHiddenMesh(){
}



// Management
///////////////



// Private Functions
//////////////////////

class deovrHiddenMesh_BuildModel : public deModelBuilder {
private:
	const vr::HiddenAreaMesh_t &pMesh;
	
public:
	deovrHiddenMesh_BuildModel( const vr::HiddenAreaMesh_t &mesh ) : pMesh( mesh ){ }
	
	virtual void BuildModel( deModel *model ){
		deModelLOD * const lod = new deModelLOD;
		model->AddLOD( lod );
		
		model->AddTexture( new deModelTexture( "material", 256, 256 ) );
		model->GetTextureAt( 0 )->SetDoubleSided( true );
		
		// add vertices
		int i;
		const int vertexCount = ( int )pMesh.unTriangleCount * 3;
		
		lod->SetVertexCount( vertexCount );
		deModelVertex * const vertices = lod->GetVertices();
		
		for( i=0; i<vertexCount; i++ ){
			const vr::HmdVector2_t &p = pMesh.pVertexData[ i ];
			vertices[ i ].SetPosition( decVector( p.v[ 0 ], p.v[ 1 ], 0.0f ) );
		}
		
		lod->SetNormalCount( 1 );
		lod->SetTangentCount( 1 );
		
		// add faces
		const int faceCount = ( int )pMesh.unTriangleCount;
		
		lod->SetFaceCount( faceCount );
		deModelFace * const faces = lod->GetFaces();
		
		for( i=0; i<faceCount; i++ ){
			faces[ i ].SetTexture( 0 );
			
			faces[ i ].SetVertex1( i * 3 );
			faces[ i ].SetVertex2( i * 3 + 1 );
			faces[ i ].SetVertex3( i * 3 + 2 );
			
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

void deovrHiddenMesh::pLoadHiddenMesh(){
	const vr::HiddenAreaMesh_t mesh = pOvr.GetVRSystem().GetHiddenAreaMesh( pEye, vr::k_eHiddenAreaMesh_Standard );
	if( ! mesh.pVertexData || mesh.unTriangleCount == 0 ){
		return;
	}
	
	pOvr.LogInfoFormat( "Loading hidden mesh for %s eye", pEye == vr::Eye_Left ? "left" : "right" );
	
	const decString path( pEye == vr::Eye_Left
		? "/openvr/hiddenMesh/leftEye.demodel"
		: "/openvr/hiddenMesh/rightEye.demodel" );
	
	deovrHiddenMesh_BuildModel builder( mesh );
	pModel.TakeOver( pOvr.GetGameEngine()->GetModelManager()->CreateModel( path, builder ) );
}
