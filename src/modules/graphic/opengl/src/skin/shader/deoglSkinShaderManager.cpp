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

#include "deoglSkinShaderConfig.h"
#include "deoglSkinShaderManager.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderUnitSourceCode.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>


// Unit Source Code Path
//////////////////////////

static const char *vUnitSourceCodePath[deoglSkinShaderManager::UnitSourceCodePathCount] = {
	"V DefRen Skin Geometry", // euscpVertexGeometry
	"V DefRen Skin Depth", // euscpVertexDepth
	"V DefRen Skin Particle", // euscpVertexParticle
	"V DefRen GI Material Map", // euscpGIMaterialMap
	
	"G DefRen Skin Geometry", // euscpGeometryGeometry
	"G DefRen Skin Depth", // euscpGeometryDepth
	"G DefRen Skin Particle", // euscpGeometryParticle
	"G DefRen Skin Particle Ribbon", // euscpGeometryParticleRibbon
	
	"TC DefRen Geometry Depth", // euscpTessControlGeometry
	"TC DefRen Skin Depth", // euscpTessControlDepth
	
	"TE DefRen Skin Geometry", // euscpTessEvalGeometry
	"TE DefRen Skin Depth", // euscpTessEvalDepth
	
	"F DefRen Skin Geometry", // euscpFragmentGeometry
	"F DefRen Skin Depth", // euscpFragmentDepth
	"F DefRen GI Material Map" // euscpGIMaterialMap
};


// Class deoglSkinShaderManager::cPrepareShader
/////////////////////////////////////////////////

deoglSkinShaderManager::cPrepareShader::cPrepareShader(
	deoglSkinShaderManager &manager, const deoglSkinShader::Ref &shader) :
pManager(manager),
pShader(shader){
}

void deoglSkinShaderManager::cPrepareShader::AddListener(cGetShaderListener *listener){
	DEASSERT_NOTNULL(listener)
	pListeners.Add(listener);
}

void deoglSkinShaderManager::cPrepareShader::PrepareShaderFinished(deoglSkinShader &shader){
	{
	const deMutexGuard guard(pManager.pMutex);
	const int index = pManager.pPrepareShaders.IndexOf(this);
	if(index != -1){
		pManager.pPrepareShaders.RemoveFrom(index);
	}
	
	deoglSkinShader *useShader = pManager.pGetShaderWith(pShader->GetConfig());
	if(!useShader && shader.GetShader() && shader.GetShader()->GetCompiled()){
		useShader = pShader;
		pManager.pShaderList.Add(pShader);
	}
	pShader = nullptr;
	
	const int count = pListeners.GetCount();
	int i;
	for(i=0; i<count; i++){
		cGetShaderListener * const listener = (cGetShaderListener*)pListeners.GetAt(i);
		try{
			listener->GetShaderFinished(useShader);
			
		}catch(const deException &e){
			pManager.pRenderThread.GetLogger().LogException(e);
		}
		delete listener;
	}
	pListeners.RemoveAll();
	}
}


// Class deoglSkinShaderManager
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinShaderManager::deoglSkinShaderManager(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pMaintananceInterval(0){
}

deoglSkinShaderManager::~deoglSkinShaderManager(){
}



// Management
///////////////

const char *deoglSkinShaderManager::GetUnitSourceCodePath(eUnitSourceCodePath unitSourceCodePath) const{
	return vUnitSourceCodePath[unitSourceCodePath];
}



int deoglSkinShaderManager::GetShaderCount(){
	const deMutexGuard guard(pMutex);
	return pShaderList.GetCount();
}

const deoglSkinShader &deoglSkinShaderManager::GetShaderAt(int index){
	const deMutexGuard guard(pMutex);
	return *(const deoglSkinShader*)pShaderList.GetAt(index);
}

deoglSkinShader *deoglSkinShaderManager::GetShaderWith(deoglSkinShaderConfig &configuration){
	const deMutexGuard guard(pMutex);
	configuration.UpdateKey();
	
	deoglSkinShader *foundShader = pGetShaderWith(configuration);
	if(foundShader){
		return foundShader;
	}
	
	const deoglSkinShader::Ref shader(deoglSkinShader::Ref::New(pRenderThread, configuration));
	shader->PrepareShader(nullptr);
	pShaderList.Add(shader);
	return shader;
}

void deoglSkinShaderManager::GetShaderWithAsync(deoglSkinShaderConfig &configuration,
cGetShaderListener *listener){
	DEASSERT_NOTNULL(listener)
	
	cPrepareShader *preparing = nullptr;
	
	{
	const deMutexGuard guard(pMutex);
	configuration.UpdateKey();
	
	deoglSkinShader * const foundShader = pGetShaderWith(configuration);
	if(foundShader){
		try{
			listener->GetShaderFinished(foundShader);
			
		}catch(const deException &e){
			pRenderThread.GetLogger().LogException(e);
		}
		delete listener;
		return;
	}
	
	preparing = pGetPrepareWith(configuration);
	if(preparing){
		preparing->AddListener(listener);
		return;
	}
	
	try{
		preparing = new cPrepareShader(*this,
			deoglSkinShader::Ref::New(pRenderThread, configuration));
		preparing->AddListener(listener);
		
	}catch(const deException &e){
		pRenderThread.GetLogger().LogException(e);
		if(preparing){
			delete preparing;
		}
		delete listener;
		throw;
	}
	pPrepareShaders.Add(preparing);
	}
	
	// has to be outside mutex lock since call can immediately call listener which acquires lock
	preparing->GetShader()->PrepareShader(preparing);
}


// Private Functions
//////////////////////

deoglSkinShader *deoglSkinShaderManager::pGetShaderWith(
const deoglSkinShaderConfig &configuration) const{
	const int count = pShaderList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglSkinShader * const shader = (deoglSkinShader*)pShaderList.GetAt(i);
		if(shader->GetConfig() == configuration){
			return shader;
		}
	}
	
	return nullptr;
}

deoglSkinShaderManager::cPrepareShader *deoglSkinShaderManager::pGetPrepareWith(
const deoglSkinShaderConfig &configuration) const{
	const int count = pPrepareShaders.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		cPrepareShader * const preparing = (cPrepareShader*)pPrepareShaders.GetAt(i);
		if(preparing->GetShader()->GetConfig() == configuration){
			return preparing;
		}
	}
	
	return nullptr;
}
