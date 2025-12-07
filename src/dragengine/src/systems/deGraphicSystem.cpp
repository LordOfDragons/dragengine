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

#include "deGraphicSystem.h"
#include "deModuleSystem.h"
#include "deScriptingSystem.h"
#include "modules/deLoadableModule.h"
#include "modules/graphic/deBaseGraphicSkin.h"
#include "modules/graphic/deBaseGraphicModel.h"
#include "modules/graphic/deBaseGraphicModule.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../parallel/deParallelProcessing.h"
#include "../resources/billboard/deBillboard.h"
#include "../resources/canvas/deCanvas.h"
#include "../resources/canvas/deCanvasView.h"
#include "../resources/canvas/deCanvasManager.h"
#include "../resources/canvas/capture/deCaptureCanvas.h"
#include "../resources/component/deComponent.h"
#include "../resources/debug/deDebugDrawer.h"
#include "../resources/decal/deDecal.h"
#include "../resources/effect/deEffect.h"
#include "../resources/font/deFont.h"
#include "../resources/image/deImage.h"
#include "../resources/loader/deResourceLoader.h"
#include "../resources/model/deModel.h"
#include "../resources/occlusionmesh/deOcclusionMesh.h"
#include "../resources/particle/deParticleEmitter.h"
#include "../resources/particle/deParticleEmitterInstance.h"
#include "../resources/probe/deEnvMapProbe.h"
#include "../resources/propfield/dePropField.h"
#include "../resources/rendering/deRenderWindow.h"
#include "../resources/rendering/deRenderWindowManager.h"
#include "../resources/sensor/deLumimeter.h"
#include "../resources/skin/deSkin.h"
#include "../resources/skin/dynamic/deDynamicSkin.h"
#include "../resources/sky/deSky.h"
#include "../resources/sky/deSkyInstance.h"
#include "../resources/smoke/deSmokeEmitter.h"
#include "../resources/terrain/heightmap/deHeightTerrain.h"
#include "../resources/video/deVideo.h"
#include "../resources/video/deVideoPlayer.h"
#include "../resources/camera/deCamera.h"
#include "../resources/light/deLight.h"
#include "../resources/world/deWorld.h"



extern const int *vResourcePeerCreationOrder;



// Class deGraphicSystem
//////////////////////////

// Constructor, destructor
////////////////////////////

deGraphicSystem::deGraphicSystem( deEngine *engine ) :
deBaseSystem( engine, "Graphic", deModuleSystem::emtGraphic ),
pActiveModule( NULL ){
}

deGraphicSystem::~deGraphicSystem(){
}



// Overloadables
//////////////////

void deGraphicSystem::SetActiveModule( deLoadableModule *module ){
	deBaseSystem::SetActiveModule( module );
	pActiveModule = ( deBaseGraphicModule* )module->GetModule();
}

void deGraphicSystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
	
	pRenderWindow = NULL;
	pInputOverlayCanvas = NULL;
	pDebugOverlayCanvas = NULL;
	pActiveModule = NULL;
}

void deGraphicSystem::PostStart(){
	deEngine &engine = *GetEngine();
	int i;
	
	// there has to be a render window to start
	if( ! pRenderWindow ){
		pRenderWindow.TakeOver( engine.GetRenderWindowManager()->CreateRenderWindow() );
		pRenderWindow->SetSize( 800, 600 );
		pRenderWindow->SetTitle( "Drag[en]gine Game" );
	}
	
	// init module
	if( ! pActiveModule->Init( pRenderWindow ) ){
		DETHROW( deeInvalidAction );
	}
	
	// load graphic system related stuff in the right order
	for( i=0; i<engine.GetResourceManagerCount(); i++ ){
		engine.GetResourceManagerAt( vResourcePeerCreationOrder[ i ] )->SystemGraphicLoad();
	}
}

void deGraphicSystem::PreStop(){
	deEngine &engine = *GetEngine();
	int i;
	
	// remove all parallel tasks if present
	engine.GetParallelProcessing().FinishAndRemoveTasksOwnedBy( pActiveModule );
	
	// unload graphic system related stuff in the right order
	for( i=engine.GetResourceManagerCount()-1; i>=0; i-- ){
		engine.GetResourceManagerAt( vResourcePeerCreationOrder[ i ] )->SystemGraphicUnload();
	}
	
	// shutdown current module
	pActiveModule->CleanUp();
}



// Management
///////////////

void deGraphicSystem::SetRenderWindow( deRenderWindow *renderWindow ){
	if( renderWindow == pRenderWindow ){
		return;
	}
	
	pRenderWindow = renderWindow;
	
	if( renderWindow && renderWindow->GetNotifyScriptOnResize() ){
		GetEngine()->GetScriptingSystem()->OnResizeRenderWindow();
	}
}

void deGraphicSystem::CreateAndSetRenderWindow( int width, int height, bool fullScreen,
const char *title, deImage *icon ){
	deRenderWindow::Ref renderWindow(deRenderWindow::Ref::New(
		 GetEngine()->GetRenderWindowManager()->CreateRenderWindow() ));
	renderWindow->SetSize( width, height );
	renderWindow->SetTitle( title );
	renderWindow->SetFullScreen( fullScreen );
	renderWindow->SetNotifyScriptOnResize( true );
	renderWindow->SetIcon( icon );
	SetRenderWindow( renderWindow );
}

#ifdef OS_BEOS
void deGraphicSystem::CreateAndSetHostedRenderWindow( int width, int height,
bool fullScreen, const char *title, deImage *icon, BWindow *hostWindow )
#define IMPLEMENT_CreateAndSetHostedRenderWindow 1

#elif defined OS_ANDROID
void deGraphicSystem::CreateAndSetHostedRenderWindow( int width, int height,
bool fullScreen, const char *title, deImage *icon, void *hostWindow )
#define IMPLEMENT_CreateAndSetHostedRenderWindow 1

#elif defined OS_WEBWASM
void deGraphicSystem::CreateAndSetHostedRenderWindow(int width, int height,
bool fullScreen, const char *title, deImage *icon, void *hostWindow)
#define IMPLEMENT_CreateAndSetHostedRenderWindow 1

#elif defined OS_UNIX_X11
void deGraphicSystem::CreateAndSetHostedRenderWindow( int width, int height,
bool fullScreen, const char *title, deImage *icon, Window hostWindow )
#define IMPLEMENT_CreateAndSetHostedRenderWindow 1

#elif defined OS_W32
void deGraphicSystem::CreateAndSetHostedRenderWindow( int width, int height,
bool fullScreen, const char *title, deImage *icon, HWND hostWindow )
#define IMPLEMENT_CreateAndSetHostedRenderWindow 1
#endif

#ifdef IMPLEMENT_CreateAndSetHostedRenderWindow
{
	deRenderWindow::Ref renderWindow(deRenderWindow::Ref::New(
		 GetEngine()->GetRenderWindowManager()->CreateRenderWindowInside( hostWindow ) ));
	renderWindow->SetSize( width, height );
	renderWindow->SetTitle( title );
	renderWindow->SetFullScreen( fullScreen );
	renderWindow->SetNotifyScriptOnResize( true );
	renderWindow->SetIcon( icon );
	SetRenderWindow( renderWindow );
}
#endif



void deGraphicSystem::SetInputOverlayCanvas( deCanvasView *view ){
	pInputOverlayCanvas = view;
}



#ifdef OS_ANDROID
void deGraphicSystem::InitAppWindow(){
	if( ! GetIsRunning() ){
		return;
	}
	pActiveModule->InitAppWindow();
}

void deGraphicSystem::TerminateAppWindow(){
	if( ! GetIsRunning() ){
		return;
	}
	pActiveModule->TerminateAppWindow();
}
#endif



// Runtime Functions
//////////////////////

void deGraphicSystem::RenderWindows(){
	if( GetIsRunning() ){
		pActiveModule->RenderWindows();
	}
}

void deGraphicSystem::CreateGlobalResources(){
	if( ! pDebugOverlayCanvas ){
		pDebugOverlayCanvas.TakeOver( GetEngine()->GetCanvasManager()->CreateCanvasView() );
	}
}



void deGraphicSystem::LoadBillboard( deBillboard *billboard ){
	DEASSERT_NOTNULL( billboard )
	if( GetIsRunning() && ! billboard->GetPeerGraphic() ){
		billboard->SetPeerGraphic( pActiveModule->CreateBillboard( billboard ) );
	}
}

void deGraphicSystem::LoadCanvas( deCanvas *canvas ){
	DEASSERT_NOTNULL( canvas )
	if( GetIsRunning() && ! canvas->GetPeerGraphic() ){
		canvas->SetPeerGraphic( pActiveModule->CreateCanvas( canvas ) );
	}
}

void deGraphicSystem::LoadCamera( deCamera *camera ){
	DEASSERT_NOTNULL( camera )
	if( GetIsRunning() && ! camera->GetPeerGraphic() ){
		camera->SetPeerGraphic( pActiveModule->CreateCamera( camera ) );
	}
}

void deGraphicSystem::LoadCaptureCanvas( deCaptureCanvas *captureCanvas ){
	DEASSERT_NOTNULL( captureCanvas )
	if( GetIsRunning() && ! captureCanvas->GetPeerGraphic() ){
		captureCanvas->SetPeerGraphic( pActiveModule->CreateCaptureCanvas( captureCanvas ) );
	}
}

void deGraphicSystem::LoadComponent( deComponent *component ){
	DEASSERT_NOTNULL( component )
	if( GetIsRunning() && ! component->GetPeerGraphic() ){
		component->SetPeerGraphic( pActiveModule->CreateComponent( component ) );
	}
}

void deGraphicSystem::LoadDebugDrawer( deDebugDrawer *debugDrawer ){
	DEASSERT_NOTNULL( debugDrawer )
	if( GetIsRunning() && ! debugDrawer->GetPeerGraphic() ){
		debugDrawer->SetPeerGraphic( pActiveModule->CreateDebugDrawer( debugDrawer ) );
	}
}

void deGraphicSystem::LoadDecal( deDecal *decal ){
	DEASSERT_NOTNULL( decal )
	if( GetIsRunning() && ! decal->GetPeerGraphic() ){
		decal->SetPeerGraphic( pActiveModule->CreateDecal( decal ) );
	}
}

void deGraphicSystem::LoadDynamicSkin( deDynamicSkin *dynamicSkin ){
	DEASSERT_NOTNULL( dynamicSkin )
	if( GetIsRunning() && ! dynamicSkin->GetPeerGraphic() ){
		dynamicSkin->SetPeerGraphic( pActiveModule->CreateDynamicSkin( dynamicSkin ) );
	}
}

void deGraphicSystem::LoadEffect( deEffect *effect ){
	DEASSERT_NOTNULL( effect )
	if( GetIsRunning() && ! effect->GetPeerGraphic() ){
		effect->SetPeerGraphic( pActiveModule->CreateEffect( effect ) );
	}
}

void deGraphicSystem::LoadEnvMapProbe( deEnvMapProbe *envMapProbe ){
	DEASSERT_NOTNULL( envMapProbe )
	if( GetIsRunning() && ! envMapProbe->GetPeerGraphic() ){
		envMapProbe->SetPeerGraphic( pActiveModule->CreateEnvMapProbe( envMapProbe ) );
	}
}

void deGraphicSystem::LoadFont( deFont *font ){
	DEASSERT_NOTNULL( font )
	if( GetIsRunning() && ! font->GetPeerGraphic() ){
		font->SetPeerGraphic( pActiveModule->CreateFont( font ) );
	}
}

void deGraphicSystem::LoadHeightTerrain( deHeightTerrain *heightTerrain ){
	DEASSERT_NOTNULL( heightTerrain )
	if( GetIsRunning() && ! heightTerrain->GetPeerGraphic() ){
		heightTerrain->SetPeerGraphic( pActiveModule->CreateHeightTerrain( heightTerrain ) );
	}
}

void deGraphicSystem::LoadImage( deImage *image ){
	DEASSERT_NOTNULL( image )
	if( GetIsRunning() && ! image->GetPeerGraphic() ){
		image->SetPeerGraphic( pActiveModule->CreateImage( image ) );
	}
}

void deGraphicSystem::LoadLight( deLight *light ){
	DEASSERT_NOTNULL( light )
	if( GetIsRunning() && ! light->GetPeerGraphic() ){
		light->SetPeerGraphic( pActiveModule->CreateLight( light ) );
	}
}

void deGraphicSystem::LoadLumimeter( deLumimeter *lumimeter ){
	DEASSERT_NOTNULL( lumimeter )
	if( GetIsRunning() && ! lumimeter->GetPeerGraphic() ){
		lumimeter->SetPeerGraphic( pActiveModule->CreateLumimeter( lumimeter ) );
	}
}

void deGraphicSystem::LoadModel( deModel *model ){
	DEASSERT_NOTNULL( model )
	if( GetIsRunning() && ! model->GetPeerGraphic() ){
		model->SetPeerGraphic( pActiveModule->CreateModel( model ) );
	}
}

void deGraphicSystem::LoadOcclusionMesh( deOcclusionMesh *occlusionMesh ){
	DEASSERT_NOTNULL( occlusionMesh )
	if( GetIsRunning() && ! occlusionMesh->GetPeerGraphic() ){
		occlusionMesh->SetPeerGraphic( pActiveModule->CreateOcclusionMesh( occlusionMesh ) );
	}
}

void deGraphicSystem::LoadParticleEmitter( deParticleEmitter *emitter ){
	DEASSERT_NOTNULL( emitter )
	if( GetIsRunning() && ! emitter->GetPeerGraphic() ){
		emitter->SetPeerGraphic( pActiveModule->CreateParticleEmitter( emitter ) );
	}
}

void deGraphicSystem::LoadParticleEmitterInstance( deParticleEmitterInstance *instance ){
	DEASSERT_NOTNULL( instance )
	if( GetIsRunning() && ! instance->GetPeerGraphic() ){
		instance->SetPeerGraphic( pActiveModule->CreateParticleEmitterInstance( instance ) );
	}
}

void deGraphicSystem::LoadPropField( dePropField *propField ){
	DEASSERT_NOTNULL( propField )
	if( GetIsRunning() && ! propField->GetPeerGraphic() ){
		propField->SetPeerGraphic( pActiveModule->CreatePropField( propField ) );
	}
}

void deGraphicSystem::LoadRenderWindow( deRenderWindow *renderWindow ){
	DEASSERT_NOTNULL( renderWindow )
	if( GetIsRunning() && ! renderWindow->GetPeerGraphic() ){
		renderWindow->SetPeerGraphic( pActiveModule->CreateRenderWindow( renderWindow ) );
	}
}

void deGraphicSystem::LoadSkin( deSkin *skin ){
	DEASSERT_NOTNULL( skin )
	if( GetIsRunning() && ! skin->GetPeerGraphic() ){
		skin->SetPeerGraphic( pActiveModule->CreateSkin( skin ) );
	}
}

void deGraphicSystem::LoadSky( deSky *sky ){
	DEASSERT_NOTNULL( sky )
	if( GetIsRunning() && ! sky->GetPeerGraphic() ){
		sky->SetPeerGraphic( pActiveModule->CreateSky( sky ) );
	}
}

void deGraphicSystem::LoadSkyInstance( deSkyInstance *instance ){
	DEASSERT_NOTNULL( instance )
	if( GetIsRunning() && ! instance->GetPeerGraphic() ){
		instance->SetPeerGraphic( pActiveModule->CreateSkyInstance( instance ) );
	}
}

void deGraphicSystem::LoadSmokeEmitter( deSmokeEmitter *smokeEmitter ){
	DEASSERT_NOTNULL( smokeEmitter )
	if( GetIsRunning() && ! smokeEmitter->GetPeerGraphic() ){
		smokeEmitter->SetPeerGraphic( pActiveModule->CreateSmokeEmitter( smokeEmitter ) );
	}
}

void deGraphicSystem::LoadVideo( deVideo *video ){
	DEASSERT_NOTNULL( video )
	if( GetIsRunning() && ! video->GetPeerGraphic() ){
		video->SetPeerGraphic( pActiveModule->CreateVideo( video ) );
	}
}

void deGraphicSystem::LoadVideoPlayer( deVideoPlayer *videoPlayer ){
	DEASSERT_NOTNULL( videoPlayer )
	if( GetIsRunning() && ! videoPlayer->GetPeerGraphic() ){
		videoPlayer->SetPeerGraphic( pActiveModule->CreateVideoPlayer( videoPlayer ) );
	}
}

void deGraphicSystem::LoadWorld( deWorld *world ){
	DEASSERT_NOTNULL( world )
	if( GetIsRunning() && ! world->GetPeerGraphic() ){
		world->SetPeerGraphic( pActiveModule->CreateWorld( world ) );
	}
}
