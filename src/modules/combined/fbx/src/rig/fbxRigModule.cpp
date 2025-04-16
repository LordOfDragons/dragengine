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



#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *FBXRigCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif
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

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class fbxRigModuleInternal : public deInternalModule{
public:
	fbxRigModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("FBXRig");
		SetDescription("Handles rigs in the binary FBX format.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtRig);
		SetDirectoryName("fbxrig");
		GetPatternList().Add(".fbx");
		SetDefaultExtension(".fbx");
		SetPriority(1);
		SetNoSaving(true);
	}
	
	void CreateModule() override{
		SetModule(FBXRigCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *fbxRigRegisterInternalModule(deModuleSystem *system){
	return new fbxRigModuleInternal(system);
}
#endif
