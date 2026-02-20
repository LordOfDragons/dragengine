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

#include "deGraphicNull.h"

#include <dragengine/common/exceptions.h>



// Export definition
//////////////////////

#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *NullGraphicCreateModule(deLoadableModule *loadableModule);
#ifdef  __cplusplus
}
#endif
#endif


// Entry Function
///////////////////

deBaseModule *NullGraphicCreateModule(deLoadableModule *loadableModule){
	deBaseModule *module = nullptr;
	
	try{
		module = new deGraphicNull(*loadableModule);
		
	}catch(const deException &){
		return nullptr;
	}
	
	return module;
}



// Class deGraphicNull
////////////////////////

// Constructor, destructor
////////////////////////////

deGraphicNull::deGraphicNull(deLoadableModule &loadableModule) :
deBaseGraphicModule(loadableModule){
}

deGraphicNull::~deGraphicNull(){
}



// Management
///////////////

bool deGraphicNull::Init(deRenderWindow *renderWindow){
	return true;
}

void deGraphicNull::CleanUp(){
}

void deGraphicNull::SetRenderWindow(deRenderWindow *renderWindow){
}

#ifdef OS_ANDROID
void deGraphicNull::InitAppWindow(){
}

void deGraphicNull::TerminateAppWindow(){
}
#endif



void deGraphicNull::RenderWindows(){
}

void deGraphicNull::InputOverlayCanvasChanged(){
}


deBaseGraphicCanvas *deGraphicNull::CreateCanvas(deCanvas*){
	return nullptr;
}

deBaseGraphicCaptureCanvas *deGraphicNull::CreateCaptureCanvas(deCaptureCanvas*){
	return nullptr;
}

deBaseGraphicDynamicSkin *deGraphicNull::CreateDynamicSkin(deDynamicSkin*){
	return nullptr;
}

deBaseGraphicEnvMapProbe *deGraphicNull::CreateEnvMapProbe(deEnvMapProbe*){
	return nullptr;
}

deBaseGraphicOcclusionMesh *deGraphicNull::CreateOcclusionMesh(deOcclusionMesh*){
	return nullptr;
}

deBaseGraphicModel *deGraphicNull::CreateModel(deModel*){
	return nullptr;
}

deBaseGraphicSkin *deGraphicNull::CreateSkin(deSkin*){
	return nullptr;
}

deBaseGraphicImage *deGraphicNull::CreateImage(deImage*){
	return nullptr;
}

deBaseGraphicComponent *deGraphicNull::CreateComponent(deComponent*){
	return nullptr;
}

deBaseGraphicLight *deGraphicNull::CreateLight(deLight*){
	return nullptr;
}

deBaseGraphicFont *deGraphicNull::CreateFont(deFont*){
	return nullptr;
}

deBaseGraphicSky *deGraphicNull::CreateSky(deSky*){
	return nullptr;
}

deBaseGraphicSkyInstance *deGraphicNull::CreateSkyInstance(deSkyInstance*){
	return nullptr;
}

deBaseGraphicWorld *deGraphicNull::CreateWorld(deWorld*){
	return nullptr;
}

deBaseGraphicEffect *deGraphicNull::CreateEffect(deEffect*){
	return nullptr;
}

deBaseGraphicDebugDrawer *deGraphicNull::CreateDebugDrawer(deDebugDrawer*){
	return nullptr;
}

deBaseGraphicRenderWindow *deGraphicNull::CreateRenderWindow(deRenderWindow*){
	return nullptr;
}

deBaseGraphicBillboard *deGraphicNull::CreateBillboard(deBillboard*){
	return nullptr;
}

deBaseGraphicDecal *deGraphicNull::CreateDecal(deDecal*){
	return nullptr;
}

deBaseGraphicCamera *deGraphicNull::CreateCamera(deCamera*){
	return nullptr;
}

deBaseGraphicLumimeter *deGraphicNull::CreateLumimeter(deLumimeter*){
	return nullptr;
}

deBaseGraphicHeightTerrain *deGraphicNull::CreateHeightTerrain(deHeightTerrain*){
	return nullptr;
}

deBaseGraphicPropField *deGraphicNull::CreatePropField(dePropField*){
	return nullptr;
}

deBaseGraphicParticleEmitter *deGraphicNull::CreateParticleEmitter(deParticleEmitter*){
	return nullptr;
}

deBaseGraphicParticleEmitterInstance *deGraphicNull::CreateParticleEmitterInstance(deParticleEmitterInstance*){
	return nullptr;
}

deBaseGraphicSmokeEmitter *deGraphicNull::CreateSmokeEmitter(deSmokeEmitter*){
	return nullptr;
}

deBaseGraphicVideo *deGraphicNull::CreateVideo(deVideo*){
	return nullptr;
}

deBaseGraphicVideoPlayer *deGraphicNull::CreateVideoPlayer(deVideoPlayer*){
	return nullptr;
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class degnModuleInternal : public deInternalModule{
public:
	using Ref = deTObjectReference<degnModuleInternal>;
	
	degnModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("NullGraphic");
		SetDescription("Renders nothing at all. Null modules are useful \
for testing purpose or servers without a graphic system.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtGraphic);
		SetDirectoryName("null");
		SetPriority(0);
		SetIsFallback(true);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(NullGraphicCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deTObjectReference<deInternalModule> degnRegisterInternalModule(deModuleSystem *system){
	return degnModuleInternal::Ref::New(system);
}
#endif
