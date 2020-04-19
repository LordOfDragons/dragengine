/* 
 * Drag[en]gine Game Engine
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

#include "deParallelTask.h"
#include "deParallelTaskReference.h"
#include "deParallelThread.h"
#include "../common/exceptions.h"
#include "../threading/deThreadSafeObjectReference.h"



// Class deParallelTask
/////////////////////////

// Constructor, destructor
////////////////////////////

deParallelTask::deParallelTask( deBaseModule *owner ) :
pOwner( owner ),
pCancel( false ),
pFinished( false ),
pMarkFinishedAfterRun( true ),
pEmptyRun( false ),
pLowPriority( false ){
}

deParallelTask::~deParallelTask(){
}



// Management
///////////////

void deParallelTask::SetMarkFinishedAfterRun( bool markFinishedAfterRun ){
	pMarkFinishedAfterRun = markFinishedAfterRun;
}

void deParallelTask::SetEmptyRun( bool emptyRun ){
	pEmptyRun = emptyRun;
}

void deParallelTask::SetLowPriority( bool lowPriority ){
	pLowPriority = lowPriority;
}

void deParallelTask::Cancel(){
	if( pCancel || pFinished ){
		return;
	}
	
	pCancel = true;
	
	const int count = pDependedOnBy.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deParallelTask* )pDependedOnBy.GetAt( i ) )->Cancel();
	}
}

void deParallelTask::SetFinished(){
	pFinished = true;
}

int deParallelTask::GetDependsOnCount() const{
	return pDependsOn.GetCount();
}

deParallelTask *deParallelTask::GetDependsOnAt( int index ) const{
	return ( deParallelTask* )pDependsOn.GetAt( index );
}

bool deParallelTask::DoesDependOn( deParallelTask *task ) const{
	return pDependsOn.Has( task );
}

void deParallelTask::AddDependsOn( deParallelTask *task ){
	if( ! task || task == this ){
		DETHROW( deeInvalidParam );
	}
	if( task->DoesDependOn( this ) ){
		DETHROW( deeInvalidParam );
	}
	
	pDependsOn.Add( task );
	task->GetDependedOnBy().Add( this );
	
// 	VerifyDependsOn();
// 	task->VerifyDependsOn();
}

void deParallelTask::RemoveDependsOn( deParallelTask *task ){
	if( ! task || task == this ){
		DETHROW( deeInvalidParam );
	}
	
	task->GetDependedOnBy().Remove( this );
	pDependsOn.Remove( task );
	
// 	VerifyDependsOn();
}

void deParallelTask::RemoveAllDependsOn(){
	if( pDependsOn.GetCount() == 0 ){
		return;
	}
	
	deThreadSafeObjectReference guard( this );
	
	while( pDependsOn.GetCount() > 0 ){
		deParallelTask * const task = ( deParallelTask* )pDependsOn.GetAt( 0 );
		task->GetDependedOnBy().Remove( this );
		pDependsOn.Remove( task );
		
// 		VerifyDependsOn();
	}
}

void deParallelTask::RemoveFromAllDependedOnTasks(){
	if( pDependedOnBy.GetCount() == 0 ){
		return;
	}
	
	deThreadSafeObjectReference guard( this );
	
	while( pDependedOnBy.GetCount() > 0 ){
		( ( deParallelTask* )pDependedOnBy.GetAt( 0 ) )->RemoveDependsOn( this );
		
// 		VerifyDependsOn();
	}
}

bool deParallelTask::CanRun() const{
	if( IsCancelled() ){
		return false;
	}
	
	const int count = pDependsOn.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		if( ! ( ( deParallelTask* )pDependsOn.GetAt( i ) )->GetFinished() ){
			return false;
		}
	}
	
	return true;
}

void deParallelTask::Reset(){
	pFinished = false;
	pCancel = false;
}



// Debugging
//////////////

decString deParallelTask::GetDebugName() const{
	return "Task";
}

decString deParallelTask::GetDebugDetails() const{
	return "";
}

/*
void deParallelTask::VerifyDependsOn(){
	int i, count = pDependsOn.GetCount();
	for( i=0; i<count; i++ ){
		if( ! ((deParallelTask*)pDependsOn.GetAt( i ))->GetDependedOnBy().Has(this) ){
			DETHROW( deeInvalidParam );
		}
	}
	
	count = pDependedOnBy.GetCount();
	for( i=0; i<count; i++ ){
		if( ! ((deParallelTask*)pDependedOnBy.GetAt( i ))->DoesDependOn(this) ){
			DETHROW( deeInvalidParam );
		}
	}
}
*/
