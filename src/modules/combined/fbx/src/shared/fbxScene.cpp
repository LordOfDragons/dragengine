/* 
 * FBX Model Module
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

#include "fbxConnection.h"
#include "fbxNode.h"
#include "fbxScene.h"
#include "fbxProperty.h"
#include "property/fbxPropertyString.h"

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxScene
///////////////////

// Constructor, destructor
////////////////////////////

fbxScene::fbxScene() :
pVersion( 0 ),
pNode( new fbxNode ),
pUpAxis( eaYPos ),
pFrontAxis( eaZNeg ),
pCoordAxis( eaXPos ),
pUnitScaleFactor( 1.0f ),
pScaleFactor( 100.0f )
{
}

fbxScene::fbxScene( decBaseFileReader &reader ) :
pVersion( 0 ),
pNode( new fbxNode ),
pUpAxis( eaYPos ),
pFrontAxis( eaZNeg ),
pCoordAxis( eaXPos ),
pUnitScaleFactor( 1.0f ),
pScaleFactor( 100.0f )
{
	// header
	char signature[ 21 ]; // 0-terminator at index 20 included !
	reader.Read( signature, 21 );
	if( strcmp( "Kaydara FBX Binary  ", signature ) != 0 ){ // compare including 0-terminator
		DETHROW_INFO( deeInvalidFileFormat, "signature mismatch" );
	}
	
	reader.SkipUShort(); // [0x1A, 0x00] (unknown but all observed files show these bytes)
	
	pVersion = reader.ReadUInt(); // 7300 for version 7.3 for example.
	
	deObjectReference node;
	while( true ){
		const int checkEndOffset = reader.ReadUInt();
		if( checkEndOffset == 0 ){
			reader.MovePosition( 9 );
			break;
		}
		
		node.TakeOver( new fbxNode( reader, checkEndOffset ) );
		pNode->AddNode( ( fbxNode* )( deObject* )node );
	}
	
	// footer
}

fbxScene::~fbxScene(){
}



// Loading and Saving
///////////////////////

void fbxScene::SetVersion( int version ){
	pVersion = version;
}



void fbxScene::FindConnections( int64_t id, decPointerList &list ) const{
	const int count = pConnections.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		fbxConnection * const connection = ( fbxConnection* )pConnections.GetAt( i );
		if( connection->GetSource() == id || connection->GetTarget() == id ){
			list.Add( connection );
		}
	}
}

fbxNode *fbxScene::NodeWithID( int64_t id ) const{
	if( ! pNodeObjects ){
		DETHROW_INFO( deeInvalidParam, "missing node 'Objects'" );
	}
	return pNodeObjects->NodeWithID( id );
}

fbxNode *fbxScene::NodeWithIDOrNull( int64_t id ) const{
	return pNodeObjects ? pNodeObjects->NodeWithIDOrNull( id ) : NULL;
}



decVector2 fbxScene::ConvUVFbxToDe( const decVector2 &uv ){
	return decVector2( uv.x, 1.0f - uv.y );
}

fbxScene::eMappingInformationType fbxScene::ConvMappingInformationType( const fbxNode &node ){
	const decString &string = node.FirstNodeNamed( "MappingInformationType" )->
		GetPropertyAt( 0 )->CastString().GetValue();
	
	if( string == "AllSame" ){
		return emitAllSame;
		
	}else if( string == "ByPolygon" ){
		return emitByPolygon;
		
	}else if( string == "ByVertice" ){
		return emitByVertex;
		
	}else if( string == "ByPolygonVertex" ){
		return emitByPolygonVertex;
		
	}else{
		DETHROW_INFO( deeInvalidParam, decString( "unsupported MappingInformationType: " ) + string );
	}
}

fbxScene::eReferenceInformationType fbxScene::ConvReferenceInformationType( const fbxNode &node ){
	const decString &string = node.FirstNodeNamed( "ReferenceInformationType" )->
		GetPropertyAt( 0 )->CastString().GetValue();
	
	if( string == "Direct" ){
		return eritDirect;
		
	}else if( string == "IndexToDirect" ){
		return eritIndexToDirect;
		
	}else{
		DETHROW_INFO( deeInvalidParam, decString( "unsupported ReferenceInformationType: " ) + string );
	}
}

fbxScene::eRotationOrder fbxScene::ConvRotationOrder( int value ){
	switch( value ){
	case 0:
		return eroXYZ;
		
	case 1:
		return eroXZY;
		
	case 2:
		return eroYZX;
		
	case 3:
		return eroYXZ;
		
	case 4:
		return eroZXY;
		
	case 5:
		return eroZYX;
		
	case 6:
		return eroSphericXYZ;
		
	default:
		decString message( "unsupported rotation order: " );
		message.AppendValue( value );
		DETHROW_INFO( deeInvalidParam, message );
	}
}

decMatrix fbxScene::CreateRotationMatrix( const decVector &rotation, eRotationOrder rotationOrder ){
	// according to FBX SDK doc: right handed system
	
	switch( rotationOrder ){
	case eroXYZ:
		return decMatrix::CreateRotationX( -rotation.x )
			* decMatrix::CreateRotationY( -rotation.y )
			* decMatrix::CreateRotationZ( rotation.z );
		
	case eroXZY:
		return decMatrix::CreateRotationX( -rotation.x )
			* decMatrix::CreateRotationY( rotation.z )
			* decMatrix::CreateRotationZ( -rotation.y );
		
	case eroYZX:
		return decMatrix::CreateRotationX( -rotation.y )
			* decMatrix::CreateRotationY( rotation.z )
			* decMatrix::CreateRotationZ( -rotation.x );
		
	case eroYXZ:
		return decMatrix::CreateRotationX( -rotation.y )
			* decMatrix::CreateRotationY( -rotation.x )
			* decMatrix::CreateRotationZ( rotation.z );
		
	case eroZXY:
		return decMatrix::CreateRotationX( rotation.z )
			* decMatrix::CreateRotationY( -rotation.x )
			* decMatrix::CreateRotationZ( -rotation.y );
		
	case eroZYX:
		return decMatrix::CreateRotationX( rotation.z )
			* decMatrix::CreateRotationY( -rotation.y )
			* decMatrix::CreateRotationZ( -rotation.x );
		
	case eroSphericXYZ:
		return decMatrix::CreateRotationX( -rotation.x )
			* decMatrix::CreateRotationY( -rotation.y )
			* decMatrix::CreateRotationZ( rotation.z );
		
	default:
		DETHROW( deeInvalidParam );
	}
}



void fbxScene::Prepare( deBaseModule &module ){
	pNode->Prepare( module );
	
	const fbxNode * const settings = pNode->FirstNodeNamedOrNull( "GlobalSettings" );
	if( settings ){
		const int upAxis = settings->GetPropertyInt( "UpAxis", 2 );
		const int frontAxis = settings->GetPropertyInt( "FrontAxis", 1 );
		const int coordAxis = settings->GetPropertyInt( "CoordAxis", 0 );
		const int upAxisSign = settings->GetPropertyInt( "UpAxisSign", 1 );
		const int frontAxisSign = settings->GetPropertyInt( "FrontAxisSign", 1 );
		const int coordAxisSign = settings->GetPropertyInt( "CoordAxisSign", 1 );
		
		pUpAxis = pGetAxis( upAxis, upAxisSign );
		pFrontAxis = pGetAxis( frontAxis, frontAxisSign );
		pCoordAxis = pGetAxis( coordAxis, coordAxisSign );
		pUnitScaleFactor = settings->GetPropertyFloat( "UnitScaleFactor", 1.0f );
		//printf("upAxis=%d frontAxis=%d coordAxis=%d unitScale=%g\n", pUpAxis, pFrontAxis, pCoordAxis, pUnitScaleFactor);
		
		switch( upAxis ){
		case 0:
			pTransformation.a12 = upAxisSign; pTransformation.a22 = 0.0f; pTransformation.a32 = 0.0f;
			break;
			
		case 1:
			pTransformation.a12 = 0.0f; pTransformation.a22 = upAxisSign; pTransformation.a32 = 0.0f;
			break;
			
		case 2:
			pTransformation.a12 = 0.0f; pTransformation.a22 = 0.0f; pTransformation.a32 = upAxisSign;
			break;
		}
		
		switch( frontAxis ){
		case 0:
			pTransformation.a13 = frontAxisSign; pTransformation.a23 = 0.0f; pTransformation.a33 = 0.0f;
			break;
			
		case 1:
			pTransformation.a13 = 0.0f; pTransformation.a23 = frontAxisSign; pTransformation.a33 = 0.0f;
			break;
			
		case 2:
			pTransformation.a13 = 0.0f; pTransformation.a23 = 0.0f; pTransformation.a33 = frontAxisSign;
			break;
		}
		
		switch( coordAxis ){
		case 0:
			pTransformation.a11 = coordAxisSign; pTransformation.a21 = 0.0f; pTransformation.a31 = 0.0f;
			break;
			
		case 1:
			pTransformation.a11 = 0.0f; pTransformation.a21 = coordAxisSign; pTransformation.a31 = 0.0f;
			break;
			
		case 2:
			pTransformation.a11 = 0.0f; pTransformation.a21 = 0.0f; pTransformation.a31 = coordAxisSign;
			break;
		}
	}
	
	// FBX unit = 1cm according to docs. but it looks like majority of people use 1 unit = 1m.
	const float fbxUnitScale = pUnitScaleFactor * 1.0f; // 0.01f;
	pTransformation *= decMatrix::CreateScale( fbxUnitScale, fbxUnitScale, fbxUnitScale );
	
	pNodeObjects = pNode->FirstNodeNamed( "Objects" );
	
	pNodeConnections = pNode->FirstNodeNamed( "Connections" );
	const int count = pNodeConnections->GetNodeCount();
	deObjectReference connection;
	int i;
	
	for( i=0; i<count; i++ ){
		fbxNode &nodeConnection = *pNodeConnections->GetNodeAt( i );
		if( nodeConnection.GetName() != "C" ){
			DETHROW_INFO( deeInvalidFileFormat, "node inside 'Connections' is not named 'C'" );
		}
		
		const decString &type = nodeConnection.GetPropertyAt( 0 )->CastString().GetValue();
		if( type == "OO" ){
			connection.TakeOver( new fbxConnection(
				nodeConnection.GetPropertyAt( 1 )->GetValueAsLong(),
				nodeConnection.GetPropertyAt( 2 )->GetValueAsLong() ) );
			pConnections.Add( connection );
			
		}else if( type == "OP" ){
			connection.TakeOver( new fbxConnection(
				nodeConnection.GetPropertyAt( 1 )->GetValueAsLong(),
				nodeConnection.GetPropertyAt( 2 )->GetValueAsLong(),
				nodeConnection.GetPropertyAt( 3 )->CastString().GetValue() ) );
			pConnections.Add( connection );
		}
	}
}

void fbxScene::Save(decBaseFileWriter &writer ){
}

void fbxScene::DebugPrintStructure( deBaseModule &module, bool verbose ) const{
	module.LogInfoFormat( "FBX Scene (version %d.%d):", pVersion / 1000, pVersion % 1000 );
	pNode->DebugPrintStructure( module, "", verbose );
}

fbxScene::eAxis fbxScene::pGetAxis( int axisType, int axisSign ) const{
	switch( axisType ){
	case 0:
		return axisSign > 0 ? eaXPos : eaXNeg;
		
	case 1:
		return axisSign > 0 ? eaYPos : eaYNeg;
		
	case 2:
		return axisSign > 0 ? eaZPos : eaZNeg;
		
	default:
		DETHROW_INFO( deeInvalidParam, "invalid axis type" );
	}
}
