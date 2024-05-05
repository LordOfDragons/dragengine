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

#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *NullGraphicCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif



// Entry Function
///////////////////

deBaseModule *NullGraphicCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deGraphicNull( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Class deGraphicNull
////////////////////////

// Constructor, destructor
////////////////////////////

deGraphicNull::deGraphicNull( deLoadableModule &loadableModule ) :
deBaseGraphicModule( loadableModule ){
}

deGraphicNull::~deGraphicNull(){
}



// Management
///////////////

bool deGraphicNull::Init( deRenderWindow *renderWindow ){
	return true;
}

void deGraphicNull::CleanUp(){
}

void deGraphicNull::SetRenderWindow( deRenderWindow *renderWindow ){
}

#ifdef ANDROID
void deGraphicNull::InitAppWindow(){
}

void deGraphicNull::TerminateAppWindow(){
}
#endif



void deGraphicNull::RenderWindows(){
}

void deGraphicNull::InputOverlayCanvasChanged(){
}


deBaseGraphicCanvas *deGraphicNull::CreateCanvas( deCanvas* ){
	return NULL;
}

deBaseGraphicCaptureCanvas *deGraphicNull::CreateCaptureCanvas( deCaptureCanvas* ){
	return NULL;
}

deBaseGraphicDynamicSkin *deGraphicNull::CreateDynamicSkin( deDynamicSkin* ){
	return NULL;
}

deBaseGraphicEnvMapProbe *deGraphicNull::CreateEnvMapProbe( deEnvMapProbe* ){
	return NULL;
}

deBaseGraphicOcclusionMesh *deGraphicNull::CreateOcclusionMesh( deOcclusionMesh* ){
	return NULL;
}

deBaseGraphicModel *deGraphicNull::CreateModel( deModel* ){
	return NULL;
}

deBaseGraphicSkin *deGraphicNull::CreateSkin( deSkin* ){
	return NULL;
}

deBaseGraphicImage *deGraphicNull::CreateImage( deImage* ){
	return NULL;
}

deBaseGraphicComponent *deGraphicNull::CreateComponent( deComponent* ){
	return NULL;
}

deBaseGraphicLight *deGraphicNull::CreateLight( deLight* ){
	return NULL;
}

deBaseGraphicFont *deGraphicNull::CreateFont( deFont* ){
	return NULL;
}

deBaseGraphicSky *deGraphicNull::CreateSky( deSky* ){
	return NULL;
}

deBaseGraphicSkyInstance *deGraphicNull::CreateSkyInstance( deSkyInstance* ){
	return NULL;
}

deBaseGraphicWorld *deGraphicNull::CreateWorld( deWorld* ){
	return NULL;
}

deBaseGraphicEffect *deGraphicNull::CreateEffect( deEffect* ){
	return NULL;
}

deBaseGraphicDebugDrawer *deGraphicNull::CreateDebugDrawer( deDebugDrawer* ){
	return NULL;
}

deBaseGraphicRenderWindow *deGraphicNull::CreateRenderWindow( deRenderWindow* ){
	return NULL;
}

deBaseGraphicBillboard *deGraphicNull::CreateBillboard( deBillboard* ){
	return NULL;
}

deBaseGraphicDecal *deGraphicNull::CreateDecal( deDecal* ){
	return NULL;
}

deBaseGraphicCamera *deGraphicNull::CreateCamera( deCamera* ){
	return NULL;
}

deBaseGraphicLumimeter *deGraphicNull::CreateLumimeter( deLumimeter* ){
	return NULL;
}

deBaseGraphicHeightTerrain *deGraphicNull::CreateHeightTerrain( deHeightTerrain* ){
	return NULL;
}

deBaseGraphicPropField *deGraphicNull::CreatePropField( dePropField* ){
	return NULL;
}

deBaseGraphicParticleEmitter *deGraphicNull::CreateParticleEmitter( deParticleEmitter* ){
	return NULL;
}

deBaseGraphicParticleEmitterInstance *deGraphicNull::CreateParticleEmitterInstance( deParticleEmitterInstance* ){
	return NULL;
}

deBaseGraphicSmokeEmitter *deGraphicNull::CreateSmokeEmitter( deSmokeEmitter* ){
	return NULL;
}

deBaseGraphicVideo *deGraphicNull::CreateVideo( deVideo* ){
	return NULL;
}

deBaseGraphicVideoPlayer *deGraphicNull::CreateVideoPlayer( deVideoPlayer* ){
	return NULL;
}
