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

#include "deParallelTask.h"
#include "deParallelThread.h"
#include "deParallelProcessing.h"
#include "../common/exceptions.h"



// Class deParallelTask
/////////////////////////

// Constructor, destructor
////////////////////////////

deParallelTask::deParallelTask(deBaseModule *owner) :
pOwner(owner),
pCancel(false),
pFinished(false),
pMarkFinishedAfterRun(true),
pEmptyRun(false),
pLowPriority(false){
}

deParallelTask::~deParallelTask(){
}



// Management
///////////////

void deParallelTask::SetMarkFinishedAfterRun(bool markFinishedAfterRun){
	pMarkFinishedAfterRun = markFinishedAfterRun;
}

void deParallelTask::SetEmptyRun(bool emptyRun){
	pEmptyRun = emptyRun;
}

void deParallelTask::SetLowPriority(bool lowPriority){
	pLowPriority = lowPriority;
}

void deParallelTask::Cancel(deParallelProcessing &parallel){
	const deMutexGuard lock(parallel.GetTaskDependencyMutex());
	UnprotectedCancel();
}

void deParallelTask::SetFinished(){
	pFinished = true;
}

void deParallelTask::AddDependsOn(deParallelTask *task){
	DEASSERT_NOTNULL(task)
	DEASSERT_FALSE(task == this)
	
	pDependsOn.Add(task);
	task->GetDependedOnBy().Add(this);
	
// 	VerifyDependsOn();
// 	task->VerifyDependsOn();
}

void deParallelTask::RemoveDependsOn(deParallelTask *task){
	DEASSERT_NOTNULL(task)
	DEASSERT_FALSE(task == this)
	
	task->GetDependedOnBy().Remove(this);
	pDependsOn.Remove(task);
	
// 	VerifyDependsOn();
}

void deParallelTask::RemoveAllDependsOn(){
	if(pDependsOn.IsEmpty()){
		return;
	}
	
	const deParallelTask::Ref guard(this);
	
	while(pDependsOn.IsNotEmpty()){
		deParallelTask * const task = pDependsOn.First();
		task->GetDependedOnBy().Remove(this);
		pDependsOn.Remove(task);
		
// 		VerifyDependsOn();
	}
}

void deParallelTask::RemoveFromAllDependedOnTasks(){
	if(pDependedOnBy.GetCount() == 0){
		return;
	}
	
	const deParallelTask::Ref guard(this);
	
	while(pDependedOnBy.GetCount() > 0){
		pDependedOnBy.GetAt(0)->RemoveDependsOn(this);
		
// 		VerifyDependsOn();
	}
}

bool deParallelTask::CanRun(deParallelProcessing &parallel) const{
	if(IsCancelled()){
		return false;
	}
	
	const deMutexGuard lock(parallel.GetTaskDependencyMutex());
	return pDependsOn.AllMatching([](const deParallelTask *task){
		return task->GetFinished();
	});
}

void deParallelTask::Reset(){
	pFinished = false;
	pCancel = false;
}



// Subclass Responsibility
////////////////////////////

void deParallelTask::Cancelled(){
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
	for(i=0; i<count; i++){
		if(!pDependsOn.GetAt(i)->GetDependedOnBy().Has(this)){
			DETHROW(deeInvalidParam);
		}
	}
	
	count = pDependedOnBy.GetCount();
	for(i=0; i<count; i++){
		if(!pDependedOnBy.GetAt(i)->DoesDependOn(this)){
			DETHROW(deeInvalidParam);
		}
	}
}
*/


void deParallelTask::UnprotectedCancel(){
	if(pCancel || pFinished){
		return;
	}
	
	pCancel = true;
	
	pDependedOnBy.Visit([](deParallelTask *task){
		task->UnprotectedCancel();
	});
	
	Cancelled();
}
