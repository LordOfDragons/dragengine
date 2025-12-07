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

#include "deoglTexUnitConfig.h"
#include "deoglTexUnitsConfig.h"
#include "deoglTexUnitsConfigList.h"
#include "../deoglTextureStageManager.h"
#include "../cubemap/deoglCubeMap.h"
#include "../texture2d/deoglTexture.h"
#include "../../gi/deoglGI.h"
#include "../../gi/deoglGIMaterials.h"
#include "../../rendering/defren/deoglDeferredRendering.h"
#include "../../rendering/plan/deoglRenderPlan.h"
#include "../../rendering/deoglRenderReflection.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../envmap/deoglEnvironmentMap.h"
#include "../../rendering/task/shared/deoglRenderTaskSharedPool.h"
#include "../../rendering/task/shared/deoglRenderTaskSharedTexture.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTDefaultTextures.h"
#include "../../renderthread/deoglRTUniqueKey.h"

#include <dragengine/common/exceptions.h>



// Class deoglTexUnitsConfig
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglTexUnitsConfig::deoglTexUnitsConfig(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pUnits(NULL),
pUnitCount(0),
pParamBlock(NULL),
pMaterialIndex(-1),
pMaterialUsageCount(0),
pUsageCount(1),
pUnitsHashCode(0),
pUniqueKey(renderThread.GetUniqueKey().Get()),
pRTSTexture(NULL),
pLLPrev(NULL),
pLLNext(NULL){
}

deoglTexUnitsConfig::~deoglTexUnitsConfig(){
	if(pRTSTexture){
		pRenderThread.GetRenderTaskSharedPool().ReturnTexture(pRTSTexture);
	}
	
	if(pUnits){
		delete [] pUnits;
	}
	
	pRenderThread.GetUniqueKey().Return(pUniqueKey);
	
}



// Management
///////////////

void deoglTexUnitsConfig::SetUnitCount(int count){
	if(pUnits){
		delete [] pUnits;
		pUnits = NULL;
		pUnitCount = 0;
	}
	
	if(count > 0){
		pUnits = new deoglTexUnitConfig[count];
		pUnitCount = count;
	}
}

deoglTexUnitConfig &deoglTexUnitsConfig::GetUnitAt(int index) const{
	if(index < 0 || index >= pUnitCount){
		DETHROW(deeInvalidParam);
	}
	
	return pUnits[index];
}

void deoglTexUnitsConfig::SetUnits(const deoglTexUnitConfig *units, int unitCount){
	if(unitCount < 0 || (unitCount > 0 && ! units)){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	SetUnitCount(unitCount);
	for(i=0; i<unitCount; i++){
		pUnits[i].SetFrom(units[i]);
	}
}

void deoglTexUnitsConfig::SetParameterBlock(deoglShaderParameterBlock *paramBlock){
	pParamBlock = paramBlock;
}



void deoglTexUnitsConfig::Apply() const{
	//deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	//tsmgr.DisableStagesAbove( pUnitCount - 1 );
	
	int i;
	for(i=0; i<pUnitCount; i++){
		pUnits[i].Apply(pRenderThread, i);
	}
}



void deoglTexUnitsConfig::SetMaterialIndex(int index){
	pMaterialIndex = index;
}

void deoglTexUnitsConfig::AddMaterialUsage(){
	pMaterialUsageCount++;
}

void deoglTexUnitsConfig::RemoveMaterialUsage(){
	if(pMaterialUsageCount == 0){
		DETHROW(deeInvalidParam);
	}
	pMaterialUsageCount--;
}



bool deoglTexUnitsConfig::Equals(const deoglTexUnitsConfig &tuc) const{
	if(pUnitsHashCode != tuc.pUnitsHashCode || pUnitCount != tuc.pUnitCount){
		return false;
	}
	
	int i;
	
	for(i=0; i<pUnitCount; i++){
		if(! pUnits[i].Equals(tuc.pUnits[i])){
			return false;
		}
	}
	
	return true;
}

bool deoglTexUnitsConfig::Equals(const deoglTexUnitConfig *units, int unitCount,
deoglShaderParameterBlock *paramBlock) const{
	if(unitCount < 0 || (unitCount > 0 && ! units)){
		DETHROW(deeInvalidParam);
	}
	
	if(pUnitCount != unitCount || pParamBlock != paramBlock){
		return false;
	}
	
	int i;
	for(i=0; i<pUnitCount; i++){
		if(! pUnits[i].Equals(units[i])){
			return false;
		}
	}
	
	return true;
}



void deoglTexUnitsConfig::AddUsage(){
	pUsageCount++;
}

void deoglTexUnitsConfig::RemoveUsage(){
	pUsageCount--;
	
	if(pUsageCount > 0){
		return;
	}
	
	if(pRTSTexture){
		pRenderThread.GetRenderTaskSharedPool().ReturnTexture(pRTSTexture);
		pRTSTexture = NULL;
	}
	
	pRenderThread.GetShader().GetTexUnitsConfigList().Remove(this);
}

void deoglTexUnitsConfig::CalcUnitsHashCode(){
	pUnitsHashCode = CalcUnitsHashCodeForUnits(pUnits, pUnitCount);
}

unsigned int deoglTexUnitsConfig::CalcUnitsHashCodeForUnits(const deoglTexUnitConfig *units, int unitCount){
	if(unitCount < 0 || (unitCount > 0 && ! units)){
		DETHROW(deeInvalidParam);
	}
	
	// for the hash function the opengl texture names of each unit are simply added.
	// this works since opengl names are unique for each texture no matter what type
	// the texture is of. the filtering and wrapping parameters are not very useful
	// for the hash value as is the unit number. textures tend to be used for the
	// same units in different configurations hence the order of the units is not
	// important. this gives a fast hash value that does the trick good enough
	unsigned int hashCode = 0;
	int i;
	
	for(i=0; i<unitCount; i++){
		if(units[i].GetTexture()){
			hashCode += (unsigned int)deoglTexUnitConfig::EST_BASE_GLNAME
				+ (unsigned int)units[i].GetTexture()->GetTexture();
			
		}else if(units[i].GetCubeMap()){
			hashCode += (unsigned int)deoglTexUnitConfig::EST_BASE_GLNAME
				+ (unsigned int)units[i].GetCubeMap()->GetTexture();
			
		}else if(units[i].GetTBO()){
			hashCode += (unsigned int)deoglTexUnitConfig::EST_BASE_GLNAME
				+ (unsigned int)units[i].GetTBO();
			
		}else{
			hashCode += (unsigned int)units[i].GetSpecial();
		}
	}
	
	return hashCode;
	
	/*
	unsigned int temp;
	int i;
	
	for(i=0; i<unitCount; i++){
		temp = hashCode >> 24;
		hashCode = (hashCode << 8) + temp;
		
		if(units[i].GetTexture()){
			hashCode ^= (unsigned int)units[i].GetTexture()->GetTexture();
			
		}else if(units[i].GetCubeMap()){
			hashCode ^= (unsigned int)units[i].GetCubeMap()->GetTexture();
			
		}else{
			hashCode = ~hashCode;
		}
	}
	*/
}



void deoglTexUnitsConfig::EnsureRTSTexture(){
	if(pRTSTexture){
		return;
	}
	
	pRTSTexture = pRenderThread.GetRenderTaskSharedPool().GetTexture();
	pRTSTexture->SetTUC(this);
}



void deoglTexUnitsConfig::SetLLPrev(deoglTexUnitsConfig *entry){
	pLLPrev = entry;
}

void deoglTexUnitsConfig::SetLLNext(deoglTexUnitsConfig *entry){
	pLLNext = entry;
}
