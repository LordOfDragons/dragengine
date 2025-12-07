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

void deParallelTask::Cancel(){
	if(pCancel || pFinished){
		return;
	}
	
	pCancel = true;
	
	const int count = pDependedOnBy.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deParallelTask*)pDependedOnBy.GetAt(i))->Cancel();
	}
	
	Cancelled();
}

void deParallelTask::SetFinished(){
	pFinished = true;
}

int deParallelTask::GetDependsOnCount() const{
	return pDependsOn.GetCount();
}

deParallelTask *deParallelTask::GetDependsOnAt(int index) const{
	return (deParallelTask*)pDependsOn.GetAt(index);
}

bool deParallelTask::DoesDependOn(deParallelTask *task) const{
	return pDependsOn.Has(task);
}

void deParallelTask::AddDependsOn(deParallelTask *task){
	if(! task || task == this){
		DETHROW(deeInvalidParam);
	}
	if(task->DoesDependOn(this)){
		DETHROW(deeInvalidParam);
	}
	
	pDependsOn.Add(task);
	task->GetDependedOnBy().Add(this);
	
// 	VerifyDependsOn();
// 	task->VerifyDependsOn();
}

void deParallelTask::RemoveDependsOn(deParallelTask *task){
	if(! task || task == this){
		DETHROW(deeInvalidParam);
	}
	
	task->GetDependedOnBy().Remove(this);
	pDependsOn.Remove(task);
	
// 	VerifyDependsOn();
}

void deParallelTask::RemoveAllDependsOn(){
	if(pDependsOn.GetCount() == 0){
		return;
	}
	
	const deParallelTask::Ref guard(this);
	
	while(pDependsOn.GetCount() > 0){
		deParallelTask * const task = (deParallelTask*)pDependsOn.GetAt(0);
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
		((deParallelTask*)pDependedOnBy.GetAt(0))->RemoveDependsOn(this);
		
// 		VerifyDependsOn();
	}
}

bool deParallelTask::CanRun() const{
	if(IsCancelled()){
		return false;
	}
	
	const int count = pDependsOn.GetCount();
	int i;
	for(i=0; i<count; i++){
		if(! ((deParallelTask*)pDependsOn.GetAt(i))->GetFinished()){
			return false;
		}
	}
	
	return true;
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
		if(! ((deParallelTask*)pDependsOn.GetAt(i))->GetDependedOnBy().Has(this)){
			DETHROW(deeInvalidParam);
		}
	}
	
	count = pDependedOnBy.GetCount();
	for(i=0; i<count; i++){
		if(! ((deParallelTask*)pDependedOnBy.GetAt(i))->DoesDependOn(this)){
			DETHROW(deeInvalidParam);
		}
	}
}
*/
