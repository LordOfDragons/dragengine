/* 
 * FBX Rig Module
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
#include <inttypes.h>

#include "fbxRigModule.h"
#include "../shared/fbxConnection.h"
#include "../shared/fbxNode.h"
#include "../shared/fbxProperty.h"
#include "../shared/fbxScene.h"
#include "../shared/managed/fbxRig.h"
#include "../shared/managed/fbxRigBone.h"
#include "../shared/property/fbxPropertyArrayDouble.h"
#include "../shared/property/fbxPropertyArrayFloat.h"
#include "../shared/property/fbxPropertyString.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *FBXRigCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif

deBaseModule *FBXRigCreateModule( deLoadableModule *loadableModule ){
	try{
		return new fbxRigModule( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
}



// Class fbxRigModule
////////////////////////

// Constructor, destructor
////////////////////////////

fbxRigModule::fbxRigModule( deLoadableModule &loadableModule ) :
deBaseRigModule( loadableModule ){
}

fbxRigModule::~fbxRigModule(){
}



// Loading and Saving
///////////////////////

void fbxRigModule::LoadRig( decBaseFileReader &reader, deRig &rig ){
	try{
		fbxScene scene( reader );
		scene.Prepare( *this );
		
		pLoadRig( rig, scene );
		
	}catch( const deException & ){
		LogErrorFormat( "Failed reading file '%s' at file position %d",
			reader.GetFilename(), reader.GetPosition() );
		throw;
	}
}

void fbxRigModule::SaveRig(decBaseFileWriter &writer, const deRig &rig){
	// nothing yet
	DETHROW_INFO( deeInvalidAction, "Saving not supported yet" );
}



// Private Functions
//////////////////////

void fbxRigModule::pLoadRig( deRig &rig, fbxScene &scene ){
	fbxNode * const nodePose = scene.FirstNodeNamed( "Pose" );
	const fbxRig::Ref loadRig( fbxRig::Ref::New( new fbxRig( scene, nodePose ) ) );
	/*
	decVector r(loadRig->GetMatrix().GetEulerAngles() * RAD2DEG);
	LogInfoFormat("rigmat (%f,%f,%f)", r.x, r.y, r.z);
	if( loadRig->GetBoneCount() > 0 ){
		r = loadRig->GetBoneAt(0)->GetOrientation().GetEulerAngles() * RAD2DEG;
		LogInfoFormat("bonemat (%f,%f,%f)", r.x, r.y, r.z);
	}
	r = scene.GetTransformation().GetEulerAngles();
	LogInfoFormat("scenetrans (%f,%f,%f)", r.x, r.y, r.z);
	*/
	// scene.DebugPrintStructure( *this, true );
	
	const int boneCount = loadRig->GetBoneCount();
	deRigBone *rigBone = nullptr;
	int i;
	
	try{
		for( i=0; i<boneCount; i++ ){
			const fbxRigBone &loadBone = *loadRig->GetBoneAt( i );
			rigBone = new deRigBone( loadBone.GetName() );
			
			rigBone->SetPosition( loadBone.GetPosition() );
			rigBone->SetRotation( loadBone.GetOrientation().GetEulerAngles() );
			if( loadBone.GetParent() ){
				rigBone->SetParent( loadBone.GetParent()->GetIndex() );
			}
			
			rig.AddBone( rigBone );
			rigBone = nullptr;
		}
		
	}catch( const deException & ){
		if( rigBone ){
			delete rigBone;
		}
		throw;
	}
}
