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
