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

#include "igdeGDParticleEmitter.h"
#include "igdeGDParticleEmitterManager.h"
#include "../igdeGDCategory.h"
#include "../igdeGameDefinition.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDParticleEmitterManager
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDParticleEmitterManager::igdeGDParticleEmitterManager() :
pDefaultPath("default")
{
	pCategories.TakeOver(new igdeGDCategory("Particle Emitters"));
}

igdeGDParticleEmitterManager::~igdeGDParticleEmitterManager(){
}



// Management
///////////////

void igdeGDParticleEmitterManager::AddEmitter(igdeGDParticleEmitter *emitter){
	if(!emitter){
		DETHROW(deeInvalidParam);
	}
	pEmitterList.Add(emitter);
}

void igdeGDParticleEmitterManager::RemoveEmitter(igdeGDParticleEmitter *emitter){
	pEmitterList.Remove(emitter);
}

void igdeGDParticleEmitterManager::RemoveAllEmitters(){
	pEmitterList.RemoveAll();
}

void igdeGDParticleEmitterManager::SetDefaultPath(const char *path){
	pDefaultPath = path;
}



void igdeGDParticleEmitterManager::UpdateWith(const igdeGDParticleEmitterManager &particleEmitterManager){
	const int count = particleEmitterManager.GetEmitterList().GetCount();
	igdeGDParticleEmitter::Ref emitter = NULL;
	igdeGDParticleEmitter *emitterCheck;
	int i;
	
	try{
		for(i=0; i<count; i++){
			emitter.TakeOver(new igdeGDParticleEmitter(*particleEmitterManager.GetEmitterList().GetAt(i)));
			emitterCheck = pEmitterList.GetWithPath(emitter->GetPath().GetString());
			if(emitterCheck){
				RemoveEmitter(emitterCheck);
			}
			AddEmitter(emitter);
			emitter = NULL;
		}
		
	}catch(const deException &){
		throw;
	}
	
	pCategories->UpdateWith(particleEmitterManager.pCategories);
	
	pDefaultPath = particleEmitterManager.pDefaultPath;
}
