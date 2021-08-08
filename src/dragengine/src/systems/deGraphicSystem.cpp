/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	deRenderWindowReference renderWindow;
	renderWindow.TakeOver( GetEngine()->GetRenderWindowManager()->CreateRenderWindow() );
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
#endif
#ifdef ANDROID
void deGraphicSystem::CreateAndSetHostedRenderWindow( int width, int height,
bool fullScreen, const char *title, deImage *icon, void *hostWindow )
#define IMPLEMENT_CreateAndSetHostedRenderWindow 1
#endif
#if defined OS_UNIX && defined HAS_LIB_X11
void deGraphicSystem::CreateAndSetHostedRenderWindow( int width, int height,
bool fullScreen, const char *title, deImage *icon, Window hostWindow )
#define IMPLEMENT_CreateAndSetHostedRenderWindow 1
#endif
#ifdef OS_W32
void deGraphicSystem::CreateAndSetHostedRenderWindow( int width, int height,
bool fullScreen, const char *title, deImage *icon, HWND hostWindow )
#define IMPLEMENT_CreateAndSetHostedRenderWindow 1
#endif
#ifdef IMPLEMENT_CreateAndSetHostedRenderWindow
{
	deRenderWindowReference renderWindow;
	renderWindow.TakeOver( GetEngine()->GetRenderWindowManager()->CreateRenderWindowInside( hostWindow ) );
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



#ifdef ANDROID
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
	if( GetIsRunning() ){
		if( billboard->GetPeerGraphic() ) DETHROW( deeInvalidParam );
		billboard->SetPeerGraphic( pActiveModule->CreateBillboard( billboard ) );
		if( ! billboard->GetPeerGraphic() ) DETHROW( deeOutOfMemory );
	}
}

void deGraphicSystem::LoadCanvas( deCanvas *canvas ){
	if( ! GetIsRunning() ){
		return;
	}
	
	if( canvas->GetPeerGraphic() ){
		DETHROW( deeInvalidParam );
	}
	canvas->SetPeerGraphic( pActiveModule->CreateCanvas( canvas ) );
}

void deGraphicSystem::LoadCamera( deCamera *camera ){
	if( ! GetIsRunning() ) return;
	if( camera->GetPeerGraphic() ) DETHROW( deeInvalidParam );
	camera->SetPeerGraphic( pActiveModule->CreateCamera( camera ) );
	if( ! camera->GetPeerGraphic() ) DETHROW( deeOutOfMemory );
}

void deGraphicSystem::LoadCaptureCanvas( deCaptureCanvas *captureCanvas ){
	if( ! GetIsRunning() ){
		return;
	}
	
	if( captureCanvas->GetPeerGraphic() ){
		DETHROW( deeInvalidParam );
	}
	captureCanvas->SetPeerGraphic( pActiveModule->CreateCaptureCanvas( captureCanvas ) );
}

void deGraphicSystem::LoadComponent(deComponent *comp){
	if( ! GetIsRunning() ) return;
	if(comp->GetPeerGraphic()) DETHROW(deeInvalidParam);
	comp->SetPeerGraphic(pActiveModule->CreateComponent(comp));
	if(!comp->GetPeerGraphic()) DETHROW(deeOutOfMemory);
}

void deGraphicSystem::LoadDebugDrawer( deDebugDrawer *effect ){
	if( ! GetIsRunning() ) return;
	if( effect->GetPeerGraphic() ) DETHROW( deeInvalidParam );
	effect->SetPeerGraphic( pActiveModule->CreateDebugDrawer( effect ) );
	if( ! effect->GetPeerGraphic() ) DETHROW( deeOutOfMemory );
}

void deGraphicSystem::LoadDecal( deDecal *decal ){
	if( GetIsRunning() ){
		if( decal->GetPeerGraphic() ){
			DETHROW( deeInvalidParam );
		}
		
		decal->SetPeerGraphic( pActiveModule->CreateDecal( decal ) );
		if( ! decal->GetPeerGraphic() ){
			DETHROW( deeOutOfMemory );
		}
	}
}

void deGraphicSystem::LoadDynamicSkin( deDynamicSkin *dynamicSkin ){
	if( ! GetIsRunning() ){
		return;
	}
	
	if( dynamicSkin->GetPeerGraphic() ){
		DETHROW( deeInvalidParam );
	}
	dynamicSkin->SetPeerGraphic( pActiveModule->CreateDynamicSkin( dynamicSkin ) );
}

void deGraphicSystem::LoadEffect( deEffect *effect ){
	if( ! GetIsRunning() ) return;
	if( effect->GetPeerGraphic() ) DETHROW( deeInvalidParam );
	effect->SetPeerGraphic( pActiveModule->CreateEffect( effect ) );
	if( ! effect->GetPeerGraphic() ) DETHROW( deeOutOfMemory );
}

void deGraphicSystem::LoadEnvMapProbe( deEnvMapProbe *envMapProbe ){
	if( GetIsRunning() ){
		if( envMapProbe->GetPeerGraphic() ){
			DETHROW( deeInvalidParam );
		}
		
		envMapProbe->SetPeerGraphic( pActiveModule->CreateEnvMapProbe( envMapProbe ) );
		if( ! envMapProbe->GetPeerGraphic() ){
			DETHROW( deeOutOfMemory );
		}
	}
}

void deGraphicSystem::LoadFont(deFont *font){
	if( ! GetIsRunning() ){
		return;
	}
	
	if( font->GetPeerGraphic() ){
		DETHROW( deeInvalidParam );
	}
	
	font->SetPeerGraphic( pActiveModule->CreateFont( font ) );
	
	if( ! font->GetPeerGraphic() ){
		DETHROW( deeOutOfMemory );
	}
}

void deGraphicSystem::LoadHeightTerrain( deHeightTerrain *heightTerrain ){
	if( GetIsRunning() ){
		if( heightTerrain->GetPeerGraphic() ) DETHROW( deeInvalidParam );
		
		heightTerrain->SetPeerGraphic( pActiveModule->CreateHeightTerrain( heightTerrain ) );
		if( ! heightTerrain->GetPeerGraphic() ) DETHROW( deeOutOfMemory );
	}
}

void deGraphicSystem::LoadImage( deImage *image ){
	if( ! image ){
		DETHROW( deeInvalidParam );
	}
	
	if( GetIsRunning() && ! image->GetPeerGraphic() ){
		image->SetPeerGraphic( pActiveModule->CreateImage( image ) );
	}
}

void deGraphicSystem::LoadLight(deLight *light){
	if( ! GetIsRunning() ) return;
	if(light->GetPeerGraphic()) DETHROW(deeInvalidParam);
	light->SetPeerGraphic(pActiveModule->CreateLight(light));
	if(!light->GetPeerGraphic()) DETHROW(deeOutOfMemory);
}

void deGraphicSystem::LoadLumimeter( deLumimeter *lumimeter ){
	if( ! GetIsRunning() ) return;
	if( lumimeter->GetPeerGraphic() ) DETHROW( deeInvalidParam );
	lumimeter->SetPeerGraphic( pActiveModule->CreateLumimeter( lumimeter ) );
	if( ! lumimeter->GetPeerGraphic() ) DETHROW( deeOutOfMemory );
}

void deGraphicSystem::LoadModel( deModel *model ){
	if( ! model ){
		DETHROW( deeInvalidParam );
	}
	
	if( GetIsRunning() && ! model->GetPeerGraphic() ){
		model->SetPeerGraphic( pActiveModule->CreateModel( model ) );
	}
}

void deGraphicSystem::LoadOcclusionMesh( deOcclusionMesh *occmesh ){
	if( ! occmesh ){
		DETHROW( deeInvalidParam );
	}
	
	if( GetIsRunning() && ! occmesh->GetPeerGraphic() ){
		occmesh->SetPeerGraphic( pActiveModule->CreateOcclusionMesh( occmesh ) );
	}
}

void deGraphicSystem::LoadParticleEmitter( deParticleEmitter *emitter ){
	if( GetIsRunning() ){
		if( emitter->GetPeerGraphic() ) DETHROW( deeInvalidParam );
		
		emitter->SetPeerGraphic( pActiveModule->CreateParticleEmitter( emitter ) );
		if( ! emitter->GetPeerGraphic() ) DETHROW( deeOutOfMemory );
	}
}

void deGraphicSystem::LoadParticleEmitterInstance( deParticleEmitterInstance *instance ){
	if( GetIsRunning() ){
		if( instance->GetPeerGraphic() ){
			DETHROW( deeInvalidParam );
		}
		
		instance->SetPeerGraphic( pActiveModule->CreateParticleEmitterInstance( instance ) );
		if( ! instance->GetPeerGraphic() ){
			DETHROW( deeOutOfMemory );
		}
	}
}

void deGraphicSystem::LoadPropField( dePropField *propField ){
	if( GetIsRunning() ){
		if( propField->GetPeerGraphic() ) DETHROW( deeInvalidParam );
		
		propField->SetPeerGraphic( pActiveModule->CreatePropField( propField ) );
		if( ! propField->GetPeerGraphic() ) DETHROW( deeOutOfMemory );
	}
}

void deGraphicSystem::LoadRenderWindow( deRenderWindow *renderWindow ){
	if( ! GetIsRunning() ) return;
	if( renderWindow->GetPeerGraphic() ) DETHROW( deeInvalidParam );
	renderWindow->SetPeerGraphic( pActiveModule->CreateRenderWindow( renderWindow ) );
	if( ! renderWindow->GetPeerGraphic() ) DETHROW( deeOutOfMemory );
}

void deGraphicSystem::LoadSkin( deSkin *skin ){
	if( ! skin ){
		DETHROW( deeInvalidParam );
	}
	
	if( GetIsRunning() && ! skin->GetPeerGraphic() ){
		skin->SetPeerGraphic( pActiveModule->CreateSkin( skin ) );
	}
}

void deGraphicSystem::LoadSky( deSky *sky ){
	if( ! GetIsRunning() ){
		return;
	}
	
	if( sky->GetPeerGraphic() ){
		DETHROW( deeInvalidParam );
	}
	
	sky->SetPeerGraphic( pActiveModule->CreateSky( sky ) );
}

void deGraphicSystem::LoadSkyInstance( deSkyInstance *instance ){
	if( ! GetIsRunning() ){
		return;
	}
	
	if( instance->GetPeerGraphic() ){
		DETHROW( deeInvalidParam );
	}
	
	instance->SetPeerGraphic( pActiveModule->CreateSkyInstance( instance ) );
}

void deGraphicSystem::LoadSmokeEmitter( deSmokeEmitter *smokeEmitter ){
	if( GetIsRunning() ){
		if( smokeEmitter->GetPeerGraphic() ) DETHROW( deeInvalidParam );
		
		smokeEmitter->SetPeerGraphic( pActiveModule->CreateSmokeEmitter( smokeEmitter ) );
		if( ! smokeEmitter->GetPeerGraphic() ) DETHROW( deeOutOfMemory );
	}
}

void deGraphicSystem::LoadVideo( deVideo *video ){
	if( GetIsRunning() ){
		if( video->GetPeerGraphic() ) DETHROW( deeInvalidParam );
		
		video->SetPeerGraphic( pActiveModule->CreateVideo( video ) );
	}
}

void deGraphicSystem::LoadVideoPlayer( deVideoPlayer *videoPlayer ){
	if( GetIsRunning() ){
		if( videoPlayer->GetPeerGraphic() ) DETHROW( deeInvalidParam );
		
		videoPlayer->SetPeerGraphic( pActiveModule->CreateVideoPlayer( videoPlayer ) );
	}
}

void deGraphicSystem::LoadWorld( deWorld *world ){
	if( GetIsRunning() ){
		if( world->GetPeerGraphic() ){
			DETHROW( deeInvalidParam );
		}
		world->SetPeerGraphic( pActiveModule->CreateWorld( world ) );
		if( ! world->GetPeerGraphic() ){
			DETHROW( deeOutOfMemory );
		}
	}
}
