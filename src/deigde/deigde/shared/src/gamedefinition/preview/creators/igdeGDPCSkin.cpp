/* 
 * Drag[en]gine IGDE
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

#include "igdeGDPCSkin.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../gamedefinition/igdeGameDefinition.h"
#include "../../../gameproject/igdeGameProject.h"
#include "../../../gamedefinition/skin/igdeGDSkin.h"
#include "../../../gamedefinition/sky/igdeGDSkyManager.h"
#include "../../../gui/wrapper/igdeWSky.h"
#include "../../../resourceloader/igdeResourceLoaderListener.h"
#include "../../../resourceloader/igdeResourceLoaderTask.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkinManager.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableColor.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/light/deLightManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>



// Resource loader listener
/////////////////////////////

class igdeGDPCSkinResLoader : public igdeResourceLoaderListener{
private:
	igdeGDPCSkin *pOwner;
	decString pPathModel;
	decString pPathSkin;
	deModelReference pModel;
	deSkinReference pSkin;
	int pCounter;
	bool pSuccess;
	bool &pFinished;
	
public:
	igdeGDPCSkinResLoader( igdeGDPCSkin &owner, bool &finished ) :
	pOwner( &owner ), pCounter( 0 ), pSuccess( true ), pFinished( finished ){
	}
	
	virtual ~igdeGDPCSkinResLoader(){
	}
	
	void Drop(){
		pOwner = NULL;
	}
	
	void LoadModel( const char *path ){
		pPathModel = path;
		pOwner->GetEnvironment().AsyncLoadResource( path, deResourceLoader::ertModel, this );
		pCounter++;
	}
	inline deModel *GetModel() const{ return pModel; }
	
	void LoadSkin( const char *path ){
		pPathSkin = path;
		pOwner->GetEnvironment().AsyncLoadResource( path, deResourceLoader::ertSkin, this );
		pCounter++;
	}
	inline deSkin *GetSkin() const{ return pSkin; }
	
	virtual void LoadingFinished( const igdeResourceLoaderTask &task, deFileResource *resource){
		if( ! pOwner ){
			return;
		}
		
		const deResourceLoader::eResourceType type = task.GetResourceType();
		const decString &filename = task.GetFilename();
// 		pOwner->GetEnvironment().GetLogger()->LogInfoFormat( "IGDE",
// 			"igdeGDPCSkinResLoader.LoadingFinished: %p %s (%d)[%d %d]", pOwner,
// 			filename.GetString(), type, pCounter, pSuccess );
		
		if( type == deResourceLoader::ertModel && pPathModel == filename ){
			pModel = ( deModel* )resource;
			pCounter--;
		}
		if( type == deResourceLoader::ertSkin && pPathSkin == filename ){
			pSkin = ( deSkin* )resource;
			pCounter--;
		}
		CheckFinished();
	}
	
	virtual void LoadingFailed( const igdeResourceLoaderTask &task ){
		if( ! pOwner ){
			return;
		}
		
		const deResourceLoader::eResourceType type = task.GetResourceType();
		const decString &filename = task.GetFilename();
// 		pOwner->GetEnvironment().GetLogger()->LogInfoFormat( "IGDE",
// 			"igdeGDPCSkinResLoader.LoadingFailed: %p %s (%d)[%d %d]", pOwner,
// 			filename.GetString(), type, pCounter, pSuccess );
		
		if( type == deResourceLoader::ertModel && pPathModel == filename ){
			pCounter--;
			pSuccess = false;
		}
		if( type == deResourceLoader::ertSkin && pPathSkin == filename ){
			pCounter--;
			pSuccess = false;
		}
		CheckFinished();
	}
	
	void CheckFinished(){
		if( pOwner && pCounter == 0 ){
			pFinished = true;
		}
	}
};



// Class igdeGDPCSkin
///////////////////////

// Constructor, destructor
////////////////////////////

igdeGDPCSkin::igdeGDPCSkin( igdeEnvironment &environment, igdeGDSkin *gdskin, const decPoint &size ) :
igdeGDPreviewCreator( environment, size ),
pGDSkin( gdskin ),
pSky( NULL ),
pResLoadFinished( false )
{
	if( ! gdskin ){
		DETHROW( deeInvalidParam );
	}
}

igdeGDPCSkin::~igdeGDPCSkin(){
	if( pResLoader ){
		( ( igdeGDPCSkinResLoader& )( igdeResourceLoaderListener& )pResLoader ).Drop();
		pResLoader = NULL;
	}
	if( pSky ){
		delete pSky;
	}
}



// Management
///////////////

decString igdeGDPCSkin::DebugPrefix(){
	return decString( "GDPCSkin(" ) + ( pGDSkin ? pGDSkin->GetPath() : "?" ) + ")";
}

void igdeGDPCSkin::PrepareCanvasForRender(){
	igdeEnvironment &environment = GetEnvironment();
	deEngine &engine = *environment.GetEngineController()->GetEngine();
	
	// create world
	pWorld.TakeOver( engine.GetWorldManager()->CreateWorld() );
	
	// create camera
	pCamera.TakeOver( engine.GetCameraManager()->CreateCamera() );
	pCamera->SetFov( 90.0f * DEG2RAD );
	pCamera->SetFovRatio( 1.0f );
	pCamera->SetImageDistance( 0.01f );
	pCamera->SetViewDistance( 500.0f );
	pCamera->SetLowestIntensity( 1.0f );
	pCamera->SetHighestIntensity( 1.0f );
	pCamera->SetExposure( 1.0f );
	pCamera->SetAdaptionTime( 0.0f );
	pCamera->SetPosition( decDVector( 0.0, 0.0, 1.0 ) );
	pCamera->SetOrientation( decQuaternion::CreateFromEulerY( DEG2RAD * 180.0f ) );
	pWorld->AddCamera( pCamera );
	
	// create sky
	pSky = new igdeWSky( environment );
	pSky->SetWorld( pWorld );
	pSky->SetGDDefaultSky();
	
	// adjust camera parameters to fit the sky lighting
	pCamera->SetLowestIntensity( pSky->GetMaxLightIntensity() );
	pCamera->SetHighestIntensity( pSky->GetMaxLightIntensity() );
	
	// create light
	/*
	pLight.TakeOver( engine.GetLightManager()->CreateLight() );
	pLight->SetActivated( true );
	pLight->SetCastShadows( true );
	pLight->SetColor( decColor( 1.0f, 1.0f, 1.0f ) );
	pLight->SetHalfIntensityDistance( 0.5f );
	pLight->SetHintLightImportance( 100 );
	pLight->SetHintMovement( deLight::emhStationary );
	pLight->SetHintParameter( deLight::ephStatic );
	pLight->SetHintShadowImportance( 0 );
	pLight->SetIntensity( 1.0f );
	pLight->SetPosition( decDVector( 1.0f, 1.0f, 5.0f ) );
	pLight->SetRange( 100.0f );
	pLight->SetType( deLight::eltPoint );
	pWorld->AddLight( pLight );
	*/
	
	// load resources
	if( pResLoader ){
		( ( igdeGDPCSkinResLoader& )( igdeResourceLoaderListener& )pResLoader ).Drop();
		pResLoader = NULL;
	}
	pResLoadFinished = false;
	pResLoader.TakeOver( new igdeGDPCSkinResLoader( *this, pResLoadFinished ) );
	igdeGDPCSkinResLoader &rl = ( igdeGDPCSkinResLoader& )( igdeResourceLoaderListener& )pResLoader;
	
	rl.LoadModel( "/igde/models/previewBuilder/skin.demodel" );
	if( ! pGDSkin->GetPath().IsEmpty() ){
		rl.LoadSkin( pGDSkin->GetPath() );
	}
	
	// create render world canvas
	deCanvasView &container = *GetCanvas();
	pCanvasRenderWorld.TakeOver( engine.GetCanvasManager()->CreateCanvasRenderWorld() );
	pCanvasRenderWorld->SetCamera( pCamera );
	pCanvasRenderWorld->SetSize( container.GetSize() );
	container.AddCanvas( pCanvasRenderWorld );
}

bool igdeGDPCSkin::IsCanvasReadyForRender(){
	if( ! pResLoadFinished ){
		return false;
	}
	
	igdeGDPCSkinResLoader &rl = ( igdeGDPCSkinResLoader& )( igdeResourceLoaderListener& )pResLoader;
	
	// create component
	pModel = rl.GetModel();
	pSkin = rl.GetSkin();
	
	if( pModel && pSkin ){
		deEngine &engine = *GetEnvironment().GetEngineController()->GetEngine();
		pComponent.TakeOver( engine.GetComponentManager()->CreateComponent( pModel, pSkin ) );
		
		if( pComponent->GetTextureCount() == 1 ){
			pComponent->GetTextureAt( 0 ).SetSkin( pSkin );
			pComponent->NotifyTextureChanged( 0 );
		}
		
		pWorld->AddComponent( pComponent );
	}
	
	rl.Drop();
	pResLoader = NULL;
	
	// update once
	const float initialUpdate = 0.1f;
	pWorld->Update( initialUpdate );
	
	return true;
}

void igdeGDPCSkin::UpdateCanvasForRender( float elapsed ){
	pWorld->Update( elapsed );
}
