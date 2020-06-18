/* 
 * FBX Animation Module
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

#include "fbxAnimModule.h"
#include "../shared/fbxConnection.h"
#include "../shared/fbxNode.h"
#include "../shared/fbxProperty.h"
#include "../shared/fbxScene.h"
#include "../shared/managed/fbxRig.h"
#include "../shared/managed/fbxRigBone.h"
#include "../shared/managed/fbxAnimation.h"
#include "../shared/managed/fbxAnimationMove.h"
#include "../shared/property/fbxPropertyArrayDouble.h"
#include "../shared/property/fbxPropertyArrayFloat.h"
#include "../shared/property/fbxPropertyString.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationBone.h>
#include <dragengine/resources/animation/deAnimationKeyframe.h>
#include <dragengine/resources/animation/deAnimationKeyframeList.h>
#include <dragengine/resources/animation/deAnimationMove.h>


// Export definition
//////////////////////

#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *FBXAnimCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif



// Entry function
///////////////////

deBaseModule *FBXAnimCreateModule( deLoadableModule *loadableModule ){
	try{
		return new fbxAnimModule( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
}



// Class fbxAnimModule
////////////////////////

// Constructor, destructor
////////////////////////////

fbxAnimModule::fbxAnimModule( deLoadableModule &loadableModule ) :
deBaseAnimationModule( loadableModule ){
}

fbxAnimModule::~fbxAnimModule(){
}



// Management
///////////////

void fbxAnimModule::LoadAnimation( decBaseFileReader &reader, deAnimation &animation ){
	try{
		fbxScene scene( reader );
		scene.Prepare( *this );
		
		pLoadAnimation( animation, scene );
		
	}catch( const deException & ){
		LogErrorFormat( "Failed reading file '%s' at file position %d",
			reader.GetFilename(), reader.GetPosition() );
		throw;
	}
}

void fbxAnimModule::SaveAnimation( decBaseFileWriter &writer, const deAnimation &animation ){
	// nothing yet
}



// Private Functions
//////////////////////

void fbxAnimModule::pLoadAnimation( deAnimation &animation, fbxScene &scene ){
	deObjectReference refLoadAnimation;
	refLoadAnimation.TakeOver( new fbxAnimation( scene ) );
	fbxAnimation &loadAnimation = ( fbxAnimation& )( deObject& )refLoadAnimation;
	
	fbxNode * const nodePose = scene.FirstNodeNamedOrNull( "Pose" );
	deObjectReference refLoadRig;
	fbxRig *loadRig = NULL;
	if( nodePose ){
		refLoadRig.TakeOver( new fbxRig( scene, *nodePose ) );
		loadRig = ( fbxRig* )( deObject* )refLoadRig;
		//loadRig->DebugPrintStructure( *this, "LoadModel ", true );
		loadAnimation.MatchRig( *loadRig );
	}
	
	loadAnimation.DebugPrintStructure( *this, "", true );
}
