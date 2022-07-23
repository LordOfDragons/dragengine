/* 
 * Drag[en]gine Animator Module
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

#include "dearTaskApplyRules.h"
#include "../dearAnimatorInstance.h"
#include "../deDEAnimator.h"
#include "../component/dearComponent.h"
#include "../dearBoneState.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>



// Class dearTaskApplyRules
/////////////////////////////

// Constructor, destructor
////////////////////////////

dearTaskApplyRules::dearTaskApplyRules( dearAnimatorInstance &instance ) :
deParallelTask( &instance.GetModule() ),
pInstance( instance ),
pDropped( false ){
}

dearTaskApplyRules::~dearTaskApplyRules(){
}



// Subclass Responsibility
////////////////////////////

void dearTaskApplyRules::Drop(){
	pDropped = true;
}

void dearTaskApplyRules::Run(){
// 	pInstance.GetModule().LogInfoFormat( "TaskApplyRules.Run(%p): ApplyRules", &pInstance );
	pInstance.ApplyRules();
// 	pInstance.GetModule().LogInfoFormat( "TaskApplyRules.Run(%p): ApplyStateToArComponent", &pInstance );
	pInstance.ApplyStateToArComponent();
// 	pInstance.GetModule().LogInfoFormat( "TaskApplyRules.Run(%p): Exit", &pInstance );
	
// 	dearComponent * const component = pInstance.GetComponent();
// 	if( component ){
// 		component->PrepareBones();
// 	}
}

void dearTaskApplyRules::Finished(){
	if( pDropped ){
		return; // pInstance became invalid
	}
	
	// PROBLEM
	// 
	// task is marked finished after Run() method. this means the next task can start running
	// after the Run() method exists but before the Finished() method is called. Using
	// not-marked finished is not possible since this dead-locks.
	// 
	// this component update here is thus not guaranteed to run between a Finished() and the
	// next task Run() call. this can result in a new state being calculated but not stored
	// to the component yet. if a new task is run which notices no previous task to exist it
	// is possible the old component state is captured before the new one is written inside
	// Finished().
	// 
	// GetDependedOnBy() can not be used because Finished() will be called after clearing
	// both the 'depends on' and 'depended on by' list
	if( ! IsCancelled() ){
		dearComponent * const component = pInstance.GetComponent();
		if( component && component->GetComponent().GetAnimatorTask() == this ){
			try{
// 				pInstance.GetModule().LogInfoFormat( "TaskApplyRules.Finished(%p): UpdateComponent", &pInstance );
				component->UpdateComponent();
// 				component->UpdateComponentPrepareBones();
				
			}catch( const deException & ){
				pInstance.StopTaskApplyRules();
				throw;
			}
		}
	}
	
// 	pInstance.GetModule().LogInfoFormat( "TaskApplyRules.Finished(%p): StopTaskApplyRules", &pInstance );
	pInstance.StopTaskApplyRules();
}



// Debugging
//////////////

decString dearTaskApplyRules::GetDebugName() const{
	return "DEAnimator:ApplyRules";
}

decString dearTaskApplyRules::GetDebugDetails() const{
	return "";
}
