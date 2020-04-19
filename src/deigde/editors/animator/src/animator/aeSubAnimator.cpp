/* 
 * Drag[en]gine IGDE Animator Editor
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

#include "aeSubAnimator.h"
#include "link/aeLink.h"
#include "locomotion/aeAnimatorLocomotion.h"
#include "aeAnimator.h"
#include "controller/aeController.h"
#include "rule/aeRule.h"
#include "../loadsave/aeLoadSaveSystem.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/deAnimatorInstanceManager.h>
#include "dragengine/resources/animator/deAnimatorLink.h"
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleInverseKinematic.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/component/deComponent.h>



// Class aeSubAnimator
////////////////////////

// Constructor, destructor
////////////////////////////

aeSubAnimator::aeSubAnimator( deEngine *engine ){
	if( ! engine ) DETHROW( deeInvalidParam );
	
	pEngine = engine;
	
	pEngAnimator = NULL;
	pEngAnimatorInstance = NULL;
	
	try{
		pEngAnimator = engine->GetAnimatorManager()->CreateAnimator();
		
		pEngAnimatorInstance = engine->GetAnimatorInstanceManager()->CreateAnimatorInstance();
		pEngAnimatorInstance->SetAnimator( pEngAnimator );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

aeSubAnimator::~aeSubAnimator(){
	pCleanUp();
}



// Management
///////////////

void aeSubAnimator::SetPathAnimator( const char *path ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	pPathAnimator = path;
}

void aeSubAnimator::LoadAnimator( aeLoadSaveSystem &lssys ){
	pEngAnimatorInstance->SetAnimation( NULL );
	
	if( pEngAnimator ){
		pEngAnimator->FreeReference();
		pEngAnimator = NULL;
	}
	
	if( pPathAnimator.IsEmpty() ){
		pEngAnimatorInstance->SetAnimator( NULL );
		return;
	}
	
	deAnimatorController *engController = NULL;
	deAnimatorLink *engLink = NULL;
	deAnimatorRule *engRule = NULL;
	aeAnimator *animator = NULL;
	int i;
	
	pEngine->GetLogger()->LogInfoFormat( "Animator Editor",
		"Loading animator %s", pPathAnimator.GetString() );
	
	try{
		// load from file
		animator = lssys.LoadAnimator( pPathAnimator );
		
		const int controllerCount = animator->GetControllers().GetCount();
		const int linkCount = animator->GetLinks().GetCount();
		const int ruleCount = animator->GetRules().GetCount();
		
		// create animator
		pEngAnimator = pEngine->GetAnimatorManager()->CreateAnimator();
		pEngAnimator->SetRig( animator->GetEngineAnimator()->GetRig() );
		pEngAnimator->SetAnimation( animator->GetEngineAnimator()->GetAnimation() );
		
		// add controllers
		for( i=0; i<controllerCount; i++ ){
			const aeController &controller = *animator->GetControllers().GetAt( i );
			
			engController = new deAnimatorController;
			engController->SetName( controller.GetName() );
			engController->SetValueRange( controller.GetMinimumValue(), controller.GetMaximumValue() );
			engController->SetFrozen( controller.GetFrozen() );
			engController->SetClamp( controller.GetClamp() );
			engController->SetVector( controller.GetVector() );
			
			pEngAnimator->AddController( engController );
			engController = NULL;
		}
		
		// add links
		for( i=0; i<linkCount; i++ ){
			const aeLink &link = *animator->GetLinks().GetAt( i );
			
			engLink = new deAnimatorLink;
			
			if( link.GetController() ){
				engLink->SetController( animator->GetControllers().IndexOf( link.GetController() ) );
			}
			engLink->SetCurve( link.GetCurve() );
			engLink->SetRepeat( link.GetRepeat() );
			
			pEngAnimator->AddLink( engLink );
			engLink = NULL;
		}
		
		// add rules
		for( i=0; i<ruleCount; i++ ){
			engRule = animator->GetRules().GetAt( i )->CreateEngineRule();
			pEngAnimator->AddRule( engRule );
			engRule->FreeReference();
			engRule = NULL;
		}
		
		// free the loaded animator as it is no more needed
		animator->FreeReference();
		
	}catch( const deException &e ){
		pEngine->GetLogger()->LogException( "Animator Editor", e );
		if( engRule ){
			engRule->FreeReference();
		}
		if( engLink ){
			delete engLink;
		}
		if( engController ){
			delete engController;
		}
		if( animator ){
			animator->FreeReference();
		}
		if( pEngAnimator ){
			pEngAnimator->FreeReference();
			pEngAnimator = NULL;
		}
	}
	
	pEngAnimatorInstance->SetAnimator( pEngAnimator );
}



void aeSubAnimator::ClearAnimator(){
	/* this seems wrong
	pEngAnimator->RemoveAllRules();
	pEngAnimator->RemoveAllLinks();
	pEngAnimator->RemoveAllControllers();
	*/
}

void aeSubAnimator::AddController( const char *name, float minimum, float maximum, bool clamp ){
	deAnimatorController *engController = NULL;
	
	pEngAnimatorInstance->SetAnimator( NULL );
	
	try{
		engController = new deAnimatorController;
		engController->SetName( name );
		engController->SetValueRange( minimum, maximum );
		engController->SetFrozen( false );
		engController->SetClamp( clamp );
		
		pEngAnimator->AddController( engController );
		
	}catch( const deException &e ){
		if( engController ) delete engController;
		throw;
	}
	
	pEngAnimatorInstance->SetAnimator( pEngAnimator );
}

void aeSubAnimator::SetControllerValue( int controller, float value ){
	pEngAnimatorInstance->GetControllerAt( controller ).SetCurrentValue( value );
	pEngAnimatorInstance->NotifyControllerChangedAt( controller );
}

void aeSubAnimator::AddLink( int controller, const decCurveBezier &curve ){
	deAnimatorLink *engLink = NULL;
	
	try{
		engLink = new deAnimatorLink;
		engLink->SetController( controller );
		engLink->SetCurve( curve );
		
		pEngAnimator->AddLink( engLink );
		
	}catch( const deException &e ){
		if( engLink ) delete engLink;
		throw;
	}
}

void aeSubAnimator::AddRuleSS(){
	deAnimatorRuleStateSnapshot *engRule = NULL;
	
	try{
		engRule = new deAnimatorRuleStateSnapshot;
		engRule->SetUseLastState( true );
		pEngAnimator->AddRule( engRule );
		engRule->FreeReference();
		
	}catch( const deException &e ){
		if( engRule ){
			engRule->FreeReference();
		}
		throw;
	}
}

void aeSubAnimator::AddRuleIK( const decVector &localPosition, const decVector &localOrientation,
const char *solverBone, int linkBlendFactor ){
	deAnimatorRuleInverseKinematic *engRule = NULL;
	
	try{
		engRule = new deAnimatorRuleInverseKinematic;
		if( ! engRule ) DETHROW( deeOutOfMemory );
		
		engRule->SetLocalPosition( localPosition );
		engRule->SetLocalOrientation( decMatrix::CreateRotation( localOrientation * DEG2RAD ).ToQuaternion() );
		engRule->SetUseSolverBone( true );
		engRule->SetSolverBone( solverBone );
		engRule->SetAdjustOrientation( true );
		engRule->SetBlendMode( deAnimatorRule::ebmBlend );
		
		if( linkBlendFactor != -1 ){
			engRule->GetTargetBlendFactor().AddLink( linkBlendFactor );
		}
		
		pEngAnimator->AddRule( engRule );
		engRule->FreeReference();
		
	}catch( const deException &e ){
		if( engRule ){
			engRule->FreeReference();
		}
		throw;
	}
}

void aeSubAnimator::AddBoneToRule( int rule, const char *bone ){
	pEngAnimator->GetRuleAt( rule )->GetListBones().Add( bone );
	pEngAnimator->NotifyRulesChanged();
}

void aeSubAnimator::EnableRule( int rule, bool enable ){
	pEngAnimator->GetRuleAt( rule )->SetEnabled( enable );
	pEngAnimator->NotifyRulesChanged();
}



void aeSubAnimator::SetComponentAndAnimation( deComponent *component, deAnimation *animation ){
	pEngAnimator->SetAnimation( NULL );
	pEngAnimator->SetRig( NULL );
	pEngAnimatorInstance->SetComponent( NULL );
	
	pEngAnimator->SetAnimation( animation );
	if( component ){
		pEngAnimator->SetRig( component->GetRig() );
		
	}else{
		pEngAnimator->SetRig( NULL );
	}
	pEngAnimatorInstance->SetComponent( component );
}

void aeSubAnimator::CopyControllers( deAnimatorInstance &instance ){
	const int count = pEngAnimatorInstance->GetControllerCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deAnimatorController &to = pEngAnimatorInstance->GetControllerAt( i );
		const int index = instance.IndexOfControllerNamed( to.GetName() );
		if( index == -1 ){
			continue;
		}
		
		const deAnimatorController &from = instance.GetControllerAt( index );
		to.SetValueRange( from.GetMinimumValue(), from.GetMaximumValue() );
		to.SetCurrentValue( from.GetCurrentValue() );
		to.SetVector( from.GetVector() );
		pEngAnimatorInstance->NotifyControllerChangedAt( i );
	}
}

void aeSubAnimator::Apply(){
	pEngAnimatorInstance->Apply();
}



// Private Functions
//////////////////////

void aeSubAnimator::pCleanUp(){
	if( pEngAnimatorInstance ){
		pEngAnimatorInstance->SetAnimator( NULL );
		pEngAnimatorInstance->SetComponent( NULL );
		pEngAnimatorInstance->FreeReference();
	}
	
	if( pEngAnimator ){
		pEngAnimator->SetRig( NULL );
		pEngAnimator->SetAnimation( NULL );
		pEngAnimator->FreeReference();
	}
}
