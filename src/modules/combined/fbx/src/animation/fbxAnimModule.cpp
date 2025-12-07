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

#include "fbxAnimModule.h"
#include "../shared/fbxConnection.h"
#include "../shared/fbxNode.h"
#include "../shared/fbxProperty.h"
#include "../shared/fbxScene.h"
#include "../shared/managed/fbxRig.h"
#include "../shared/managed/fbxRigBone.h"
#include "../shared/managed/fbxAnimation.h"
#include "../shared/managed/fbxAnimationCurve.h"
#include "../shared/managed/fbxAnimationMove.h"
#include "../shared/managed/fbxAnimationMoveCurves.h"
#include "../shared/managed/fbxAnimationEvaluateCurve.h"
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

#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *FBXAnimCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif
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
	fbxAnimation::Ref loadAnimation(fbxAnimation::Ref::NewWith(scene));
	
	fbxNode * const nodePose = scene.FirstNodeNamedOrNull( "Pose" );
	fbxRig::Ref loadRig(fbxRig::Ref::NewWith(scene, nodePose));
	// loadRig->DebugPrintStructure( *this, "LoadAnimation ", true );
	loadAnimation->MatchRig( *loadRig );
	
	//loadAnimation->DebugPrintStructure( *this, "", true );
	
	const int boneCount = loadRig->GetBoneCount();
	deAnimationBone *bone = NULL;
	int i;
	for( i=0; i<boneCount; i++ ){
		bone = new deAnimationBone;
		bone->SetName( loadRig->GetBoneAt( i )->GetName() );
		animation.AddBone( bone );
	}
	
	pLoadMoves( animation, *loadAnimation );
}

void fbxAnimModule::pLoadMoves( deAnimation &animation, const fbxAnimation &loadAnimation ){
	const int moveCount = loadAnimation.GetMoveCount();
	deAnimationMove *move = NULL;
	int i;
	
	try{
		for( i=0; i<moveCount; i++ ){
			const fbxAnimationMove &loadMove = *loadAnimation.GetMoveAt( i );
			move = new deAnimationMove;
			pLoadMove( animation, *move, loadMove );
			animation.AddMove( move );
			move = NULL;
		}
		
	}catch( const deException & ){
		if( move ){
			delete move;
		}
		throw;
	}
}

void fbxAnimModule::pLoadMove( deAnimation &animation, deAnimationMove &move,
const fbxAnimationMove &loadMove ){
	//const decMatrix &sceneTransform = loadMove.GetAnimation().GetScene().GetTransformation();
	const int countCurves = loadMove.GetCurvesCount();
	int i, j, k;
	
	move.SetName( loadMove.GetName() );
	
	const int boneCount = animation.GetBoneCount();
	for( i=0; i<boneCount; i++ ){
		move.AddKeyframeList( new deAnimationKeyframeList  );
	}
	
	int playtime = 0;
	
	for( i=0; i<countCurves; i++ ){
		const fbxAnimationMoveCurves &moveCurves = *loadMove.GetCurvesAt( i );
		if( ! moveCurves.GetRigBone()
		|| moveCurves.GetTargetProperty() == fbxAnimationMoveCurves::etpUnsupported ){
			continue;
		}
		
		const fbxAnimationCurve * const curves[ 3 ] = {
			moveCurves.GetCurveX(), moveCurves.GetCurveY(), moveCurves.GetCurveZ() };
		if( ! curves[ 0 ] && ! curves[ 1 ] && ! curves[ 2 ] ){
			continue;
		}
		
		int lastTime = 0;
		for( j=0; j<3; j++ ){
			if( ! curves[ j ] ){
				continue;
			}
			
			const int timeCount = curves[ j ]->GetPropertyTime().GetValueCount();
			if( timeCount > 0 ){
				lastTime = decMath::max( lastTime, loadMove.TimeToFrame( fbxAnimation::ConvTime(
					curves[ j ]->GetPropertyTime().GetValueAtAsLong( timeCount - 1 ) ) ) );
			}
		}
		
		deAnimationKeyframeList &kflist = *move.GetKeyframeList( moveCurves.GetRigBone()->GetIndex() );
		for( j=0; j<=lastTime; j++ ){
			kflist.AddKeyframe( new deAnimationKeyframe );
		}
		
		playtime = decMath::max( playtime, lastTime );
		
		const decMatrix &animMatrix = moveCurves.GetRigBone()->GetAnimMatrix();
		const fbxScene::eRotationOrder rotationOrder = moveCurves.GetRigBone()->GetRotationOrder();
		
		fbxAnimationEvaluateCurve evaluators[ 3 ] = {
			{ loadMove, curves[ 0 ], moveCurves.GetDefaultValue().x },
			{ loadMove, curves[ 1 ], moveCurves.GetDefaultValue().y },
			{ loadMove, curves[ 2 ], moveCurves.GetDefaultValue().z } };
		
		switch( moveCurves.GetTargetProperty() ){
		case fbxAnimationMoveCurves::etpPosition:
			for( k=0; k<=lastTime; k++ ){
				deAnimationKeyframe &kf = *kflist.GetKeyframe( k );
				kf.SetTime( loadMove.FrameToTime( k ) );
				kf.SetPosition( animMatrix.Transform( evaluators[ 0 ].NextValue(),
					evaluators[ 1 ].NextValue(), evaluators[ 2 ].NextValue() ) );
			}
			break;
			
		case fbxAnimationMoveCurves::etpRotation:
			for( k=0; k<=lastTime; k++ ){
				deAnimationKeyframe &kf = *kflist.GetKeyframe( k );
				kf.SetTime( loadMove.FrameToTime( k ) );
				kf.SetRotation(
					fbxScene::CreateRotationMatrix(
						decVector( evaluators[ 0 ].NextValue(), evaluators[ 1 ].NextValue(),
							evaluators[ 2 ].NextValue() ) * DEG2RAD,
						rotationOrder )
					.QuickMultiply( animMatrix )
					.Normalized()
					.GetEulerAngles() );
			}
			break;
			
		case fbxAnimationMoveCurves::etpScale:
			for( k=0; k<=lastTime; k++ ){
				deAnimationKeyframe &kf = *kflist.GetKeyframe( k );
				kf.SetTime( loadMove.FrameToTime( k ) );
				kf.SetScale(
					decMatrix::CreateScale( evaluators[ 0 ].NextValue(),
						evaluators[ 1 ].NextValue(), evaluators[ 2 ].NextValue() )
					.QuickMultiply( animMatrix )
					.GetScale() );
			}
			break;
			
		default:
			break;
		}
	}
	
	move.SetPlaytime( loadMove.FrameToTime( playtime ) );
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class fbxAnimModuleInternal : public deInternalModule{
public:
	fbxAnimModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("FBXAnim");
		SetDescription("Handles animations in the binary FBX model format.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtAnimation);
		SetDirectoryName("fbxanim");
		GetPatternList().Add(".fbx");
		SetDefaultExtension(".fbx");
		SetPriority(1);
		SetNoSaving(true);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(FBXAnimCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *fbxAnimRegisterInternalModule(deModuleSystem *system){
	return new fbxAnimModuleInternal(system);
}
#endif
