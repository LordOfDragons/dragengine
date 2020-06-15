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

#include "fbxScene.h"
#include "fbxNode.h"

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



void fbxScene::Prepare( deBaseModule &module ){
	pNode->Prepare( module );
	
	const fbxNode * const settings = pNode->FirstNodeNamed( "GlobalSettings" );
	if( settings ){
		pUpAxis = pGetAxis( settings->GetPropertyInt( "UpAxis", 2 ), settings->GetPropertyInt( "UpAxisSign", 1 ) );
		pFrontAxis = pGetAxis( settings->GetPropertyInt( "FrontAxis", 1 ), settings->GetPropertyInt( "FrontAxisSign", 1 ) );
		pCoordAxis = pGetAxis( settings->GetPropertyInt( "CoordAxis", 0 ), settings->GetPropertyInt( "CoordAxisSign", 1 ) );
		pUnitScaleFactor = settings->GetPropertyFloat( "UnitScaleFactor", 1.0f );
		printf("upAxis=%d frontAxis=%d coordAxis=%d unitScale=%g\n", pUpAxis, pFrontAxis, pCoordAxis, pUnitScaleFactor);
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
