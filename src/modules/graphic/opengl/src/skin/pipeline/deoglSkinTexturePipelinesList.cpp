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

#include "deoglSkinTexturePipelinesList.h"
#include "deoglSkinTexturePipelines.h"
#include "deoglSTPipelinesBillboard.h"
#include "deoglSTPipelinesComponent.h"
#include "deoglSTPipelinesDecal.h"
#include "deoglSTPipelinesOutline.h"
#include "heightmap/deoglSTPipelinesHeightMap1.h"
#include "heightmap/deoglSTPipelinesHeightMap2.h"
#include "particle/deoglSTPipelinesParticle.h"
#include "particle/deoglSTPipelinesParticleBeam.h"
#include "particle/deoglSTPipelinesParticleRibbon.h"
#include "propfield/deoglSTPipelinesPropField.h"
#include "propfield/deoglSTPipelinesPropFieldImposter.h"
#include "../../shaders/deoglBatchedShaderLoading.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>



// Class deoglSkinTexturePipelinesList
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinTexturePipelinesList::deoglSkinTexturePipelinesList(const deoglSkinTexture &texture){
	pPipelines[eptComponent].TakeOver(new deoglSTPipelinesComponent(texture));
	pPipelines[eptBillboard].TakeOver(new deoglSTPipelinesBillboard(texture));
	pPipelines[eptDecal].TakeOver(new deoglSTPipelinesDecal(texture));
	pPipelines[eptPropField].TakeOver(new deoglSTPipelinesPropField(texture));
	pPipelines[eptPropFieldImposter].TakeOver(new deoglSTPipelinesPropFieldImposter(texture));
	pPipelines[eptHeightMap1].TakeOver(new deoglSTPipelinesHeightMap1(texture));
	pPipelines[eptHeightMap2].TakeOver(new deoglSTPipelinesHeightMap2(texture));
	pPipelines[eptParticle].TakeOver(new deoglSTPipelinesParticle(texture));
	pPipelines[eptParticleRibbon].TakeOver(new deoglSTPipelinesParticleRibbon(texture));
	pPipelines[eptParticleBeam].TakeOver(new deoglSTPipelinesParticleBeam(texture));
	pPipelines[eptOutline].TakeOver(new deoglSTPipelinesOutline(texture));
}

deoglSkinTexturePipelinesList::~deoglSkinTexturePipelinesList(){
}



// Management
///////////////

deoglSkinTexturePipelines &deoglSkinTexturePipelinesList::GetAt(ePipelineTypes type) const{
	return pPipelines[type];
}

void deoglSkinTexturePipelinesList::Prepare(deoglBatchedShaderLoading &batched){
	int i;
	for(i=0; i<PipelineTypesCount; i++){
		pPipelines[i]->Prepare(batched);
		if(batched.TimedOut()){
			return;
		}
	}
}
