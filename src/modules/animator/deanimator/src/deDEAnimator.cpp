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
#include <string.h>

#include "dearAnimator.h"
#include "dearAnimatorInstance.h"
#include "deDEAnimator.h"
#include "animation/dearAnimation.h"
#include "component/dearComponent.h"

#include <dragengine/systems/modules/deModuleParameter.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/deEngine.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *DEAnimatorCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// Entry function
///////////////////

deBaseModule *DEAnimatorCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deDEAnimator( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Class deDEAnimator
///////////////////////

// Constructor, destructor
////////////////////////////

deDEAnimator::deDEAnimator( deLoadableModule &loadableModule ) :
deBaseAnimatorModule( loadableModule ){
}

deDEAnimator::~deDEAnimator(){
}



// Module Management
//////////////////////

bool deDEAnimator::Init(){
	return true;
}

void deDEAnimator::CleanUp(){
}



// Management
///////////////

deBaseAnimatorAnimator *deDEAnimator::CreateAnimator( deAnimator *animator ){
	return new dearAnimator( *this, *animator );
}

deBaseAnimatorAnimatorInstance *deDEAnimator::CreateAnimatorInstance( deAnimatorInstance *instance ){
	return new dearAnimatorInstance( *this, *instance );
}

deBaseAnimatorAnimation *deDEAnimator::CreateAnimation( deAnimation *animation ){
	return new dearAnimation( this, animation );
}

deBaseAnimatorComponent *deDEAnimator::CreateComponent( deComponent *component ){
	return new dearComponent( *this, *component );
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class dearModuleInternal : public deInternalModule{
public:
	dearModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("DEAnimator");
		SetDescription("Animates components using animator rules.");
		SetAuthor("Plüss Roland (roland@rptd.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtAnimator);
		SetDirectoryName("deanimator");
		SetPriority(1);
	}
	
	void CreateModule() override{
		SetModule(DEAnimatorCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *dearRegisterInternalModule(deModuleSystem *system){
	return new dearModuleInternal(system);
}
#endif
