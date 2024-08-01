#include "deMsgdkAsyncTask.h"


// Class deMsgdkAsyncTask::Invalidator
////////////////////////////////////////

deMsgdkAsyncTask::Invalidator::Invalidator() : pValid(true)
{
}

void deMsgdkAsyncTask::Invalidator::Invalidate()
{
	pValid = false;
}


// Class deMsgdkAsyncTask
///////////////////////////

// Constructor, destructor
////////////////////////////

deMsgdkAsyncTask::deMsgdkAsyncTask(const Invalidator::Ref &invalidator) :
pInvalidator(invalidator)
{
	ResetAsyncBlock();
}

deMsgdkAsyncTask::~deMsgdkAsyncTask()
{
}


// Protected Functions
////////////////////////

void deMsgdkAsyncTask::ResetAsyncBlock()
{
	pAsyncBlock = {};
	pAsyncBlock.queue = nullptr;
    pAsyncBlock.context = this;
	pAsyncBlock.callback = [](XAsyncBlock* ab){
		deMsgdkAsyncTask * const myself = (deMsgdkAsyncTask*)(ab->context);
		if(myself->pInvalidator->IsValid()){
			myself->OnFinished();
		}
		myself->FreeReference();
    };
}
