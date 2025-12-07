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

#include "deoglLightShader.h"
#include "deoglLightShaderConfig.h"
#include "deoglLightShaderManager.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderUnitSourceCode.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/threading/deMutexGuard.h>


// Unit Source Code Path
////////777///////////////

static const char *vUnitSourceCodePath[deoglLightShaderManager::EUSCP_COUNT] = {
	"V DefRen Light", //<! euscpVertexLight
	"V DefRen Light Particle", //<! euscpVertexParticle
	
	"G DefRen Light Particle", //<! euscpGeometryParticle
	"G DefRen Light Stereo", //<! euscpGeometryStereo
	
	"F DefRen Light" //<! euscpFragmentLight
};


// Class deoglLightShaderManager::cPrepareShader
//////////////////////////////////////////////////

deoglLightShaderManager::cPrepareShader::cPrepareShader(
	deoglLightShaderManager &manager, const deoglLightShader::Ref &shader) :
pManager(manager),
pShader(shader){
}

void deoglLightShaderManager::cPrepareShader::AddListener(cGetShaderListener *listener){
	DEASSERT_NOTNULL(listener)
	pListeners.Add(listener);
}

void deoglLightShaderManager::cPrepareShader::PrepareShaderFinished(deoglLightShader &shader){
	{
	const deMutexGuard guard(pManager.pMutex);
	const int index = pManager.pPrepareShaders.IndexOf(this);
	if(index != -1){
		pManager.pPrepareShaders.RemoveFrom(index);
	}
	
	deoglLightShader *useShader = pManager.pGetShaderWith(pShader->GetConfig());
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


// Class deoglLightShaderManager
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightShaderManager::deoglLightShaderManager(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pMaintananceInterval(0){
}

deoglLightShaderManager::~deoglLightShaderManager(){
	const int shaderCount = pShaderList.GetCount();
	decString text;
	int i;
	
	for(i=0; i<shaderCount; i++){
		const deoglLightShader &shader = *((deoglLightShader*)pShaderList.GetAt(i));
		
		if(shader.GetRefCount() != 1){
			shader.GetConfig().DebugGetConfigString(text);
			pRenderThread.GetLogger().LogWarnFormat("LightShaderManager CleanUp: Shader with refcount %i. Config=%s",
				shader.GetRefCount(), text.GetString());
		}
	}
}



// Management
///////////////

const char *deoglLightShaderManager::GetUnitSourceCodePath(int unitSourceCodePath) const{
	if(unitSourceCodePath < 0 || unitSourceCodePath >= EUSCP_COUNT){
		DETHROW(deeInvalidParam);
	}
	
	return vUnitSourceCodePath[unitSourceCodePath];
}


deoglLightShader *deoglLightShaderManager::GetShaderWith(deoglLightShaderConfig &configuration){
	const deMutexGuard guard(pMutex);
	configuration.UpdateKey();
	
	deoglLightShader *foundShader = pGetShaderWith(configuration);
	if(foundShader){
		return foundShader;
	}
	
	const deoglLightShader::Ref shader(deoglLightShader::Ref::New(
		new deoglLightShader(pRenderThread, configuration)));
	shader->PrepareShader(nullptr);
	pShaderList.Add(shader);
	return shader;
}

void deoglLightShaderManager::GetShaderWithAsync(deoglLightShaderConfig &configuration,
cGetShaderListener *listener){
	DEASSERT_NOTNULL(listener)
	
	cPrepareShader *preparing = nullptr;
	
	{
	const deMutexGuard guard(pMutex);
	configuration.UpdateKey();
	
	deoglLightShader * const foundShader = pGetShaderWith(configuration);
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
		preparing = new cPrepareShader(*this, deoglLightShader::Ref::New(
			new deoglLightShader(pRenderThread, configuration)));
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



void deoglLightShaderManager::Maintanance(){
	// currently no maintanance done... all greedy
	
	if(pMaintananceInterval == 0){
		deoglLightShader *shader = NULL;
		int i;
		
		for(i=pShaderList.GetCount()-1; i>=0; i--){
			shader = (deoglLightShader*)pShaderList.GetAt(i);
			
			if(shader->GetRefCount() == 1){
				pShaderList.Remove(shader);
			}
		}
		
		pMaintananceInterval = 10; // maintenance every 10 frames
	}
}


// Private Functions
//////////////////////

deoglLightShader *deoglLightShaderManager::pGetShaderWith(
const deoglLightShaderConfig &configuration) const{
	const int count = pShaderList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglLightShader * const shader = (deoglLightShader*)pShaderList.GetAt(i);
		if(shader->GetConfig() == configuration){
			return shader;
		}
	}
	
	return nullptr;
}

deoglLightShaderManager::cPrepareShader *deoglLightShaderManager::pGetPrepareWith(
const deoglLightShaderConfig &configuration) const{
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
