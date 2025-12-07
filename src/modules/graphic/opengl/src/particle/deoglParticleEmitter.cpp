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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "deoglParticleEmitter.h"
#include "deoglParticleEmitterType.h"
#include "deoglRParticleEmitter.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterOffsets.h>
#include <dragengine/resources/particle/deParticleEmitterParameter.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>



// Class deoglParticleEmitter
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglParticleEmitter::deoglParticleEmitter(deGraphicOpenGl &ogl, const deParticleEmitter &emitter) :
pOgl(ogl),
pParticleEmitter(emitter),
pREmitter(NULL),
pTypes(NULL),
pTypeCount(0),
pDirtyTypes(true)
{
	try{
		pREmitter = new deoglRParticleEmitter(ogl.GetRenderThread());
		TypeCountChanged();
		
	}catch(const deException &){
		pCleanUp();
	}
}

deoglParticleEmitter::~deoglParticleEmitter(){
	pCleanUp();
}



// Management
///////////////

void deoglParticleEmitter::SyncToRender(){
	int i;
	
	if(pDirtyTypes){
		pREmitter->RemoveAllTypes();
		for(i=0; i<pTypeCount; i++){
			pREmitter->AddType(pTypes[i]->GetRType());
		}
		pDirtyTypes = false;
	}
	
	for(i=0; i<pTypeCount; i++){
		pTypes[i]->SyncToRender();
	}
}



deoglParticleEmitterType &deoglParticleEmitter::GetTypeAt(int index) const{
	if(index < 0 || index >= pTypeCount){
		DETHROW(deeInvalidParam);
	}
	return *pTypes[index];
}



// Notifications
//////////////////

void deoglParticleEmitter::ControllerCountChanged(){
}

void deoglParticleEmitter::ControllerChanged(int controller){
}

void deoglParticleEmitter::TypeCountChanged(){
	const int typeCount = pParticleEmitter.GetTypeCount();
	
	if(pTypes){
		while(pTypeCount > 0){
			pTypeCount--;
			delete pTypes[pTypeCount];
		}
		delete [] pTypes;
		pTypes = NULL;
	}
	pTypeCount = 0;
	
	if(typeCount > 0){
		pTypes = new deoglParticleEmitterType*[typeCount];
		for(pTypeCount=0; pTypeCount<typeCount; pTypeCount++){
			pTypes[pTypeCount] = new deoglParticleEmitterType(*this, pParticleEmitter.GetTypeAt(pTypeCount));
		}
	}
	
	pDirtyTypes = true;
}

void deoglParticleEmitter::TypeChanged(int type){
	pTypes[type]->TypeChanged();
}



// Private Functions
//////////////////////

void deoglParticleEmitter::pCleanUp(){
	if(pTypes){
		while(pTypeCount > 0){
			pTypeCount--;
			delete pTypes[pTypeCount];
		}
		delete [] pTypes;
	}
	
	// types holds a reference to pREmitter. do not remove it earlier
		pREmitter = NULL;
	}
}
