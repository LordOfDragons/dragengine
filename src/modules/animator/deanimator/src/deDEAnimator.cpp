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
