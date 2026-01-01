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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdeParticleEmitter.h"
#include "../gdeGameDefinition.h"

#include <dragengine/common/exceptions.h>



// Class gdeParticleEmitter
/////////////////////////////

// Constructor, destructor
////////////////////////////

gdeParticleEmitter::gdeParticleEmitter() :
pGameDefinition(nullptr){
}

gdeParticleEmitter::gdeParticleEmitter(const char *path, const char *name) :
pGameDefinition(nullptr),
pPath(path),
pName(name){
}

gdeParticleEmitter::gdeParticleEmitter(const gdeParticleEmitter &particleEmitter) :
pGameDefinition(nullptr),

pPath(particleEmitter.pPath),
pName(particleEmitter.pName),
pDescription(particleEmitter.pDescription),
pCategory(particleEmitter.pCategory){
}

gdeParticleEmitter::~gdeParticleEmitter(){
	pCleanUp();
}



// Management
///////////////

void gdeParticleEmitter::SetGameDefinition(gdeGameDefinition *gamedef){
	pGameDefinition = gamedef;
}



void gdeParticleEmitter::SetPath(const char *path){
	if(pPath == path){
		return;
	}
	
	pPath = path;
	
	if(pGameDefinition){
		pGameDefinition->NotifyParticleEmitterChanged(this);
	}
}

void gdeParticleEmitter::SetName(const char *name){
	if(pName == name){
		return;
	}
	
	pName = name;
	
	if(pGameDefinition){
		pGameDefinition->NotifyParticleEmitterNameChanged(this);
	}
}

void gdeParticleEmitter::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	
	if(pGameDefinition){
		pGameDefinition->NotifyParticleEmitterChanged(this);
	}
}

void gdeParticleEmitter::SetCategory(const char *category){
	if(pCategory == category){
		return;
	}
	
	pCategory = category;
	
	if(pGameDefinition){
		pGameDefinition->NotifyParticleEmitterChanged(this);
	}
}



// Private functions
//////////////////////

void gdeParticleEmitter::pCleanUp(){
	SetGameDefinition(nullptr);
}
