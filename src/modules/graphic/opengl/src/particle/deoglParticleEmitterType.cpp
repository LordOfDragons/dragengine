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
#include "deoglRParticleEmitterType.h"
#include "../deGraphicOpenGl.h"
#include "../skin/deoglSkin.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/resources/particle/deParticleEmitterOffsets.h>
#include <dragengine/resources/particle/deParticleEmitterParameter.h>
#include <dragengine/resources/skin/deSkin.h>



// Class deoglParticleEmitterType
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglParticleEmitterType::deoglParticleEmitterType(
deoglParticleEmitter &emitter, const deParticleEmitterType &type) :
pType(type),
pRType(NULL),
pDirtyType(true){
	pRType = new deoglRParticleEmitterType(*emitter.GetREmitter());
}

deoglParticleEmitterType::~deoglParticleEmitterType(){
	if(pRType){
		pRType->FreeReference();
	}
}



// Management
///////////////

void deoglParticleEmitterType::SyncToRender(){
	if(pDirtyType){
		if(pType.GetSkin()){
			pRType->SetSkin(((deoglSkin*)pType.GetSkin()->GetPeerGraphic())->GetRSkin());
			
		}else{
			pRType->SetSkin(NULL);
		}
		
		pRType->SetSimulationType(pType.GetSimulationType());
		
		pRType->CheckEmitLight(pType);
		pRType->UpdateParameterSamples(pType);
		pRType->DropPipelines();
		
		pDirtyType = false;
	}
}



void deoglParticleEmitterType::TypeChanged(){
	pDirtyType = true;
}
