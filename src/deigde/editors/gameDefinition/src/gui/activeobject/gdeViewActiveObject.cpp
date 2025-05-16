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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "gdeViewActiveObject.h"
#include "gdeViewActiveObjectListener.h"
#include "gdeVAOBillboard.h"
#include "gdeVAOComponent.h"
#include "gdeVAOCamera.h"
#include "gdeVAOLight.h"
#include "gdeVAOEnvMapProbe.h"
#include "gdeVAONavSpace.h"
#include "gdeVAONavBlocker.h"
#include "gdeVAOParticleEmitter.h"
#include "gdeVAOForceField.h"
#include "gdeVAOSnapPoint.h"
#include "gdeVAOSpeaker.h"
#include "gdeVAOWorld.h"
#include "../gdeWindowMain.h"
#include "../../gdEditor.h"
#include "../../configuration/gdeConfiguration.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/particleemitter/gdeParticleEmitter.h"
#include "../../gamedef/objectClass/gdeObjectClass.h"
#include "../../gamedef/objectClass/billboard/gdeOCBillboard.h"
#include "../../gamedef/objectClass/camera/gdeOCCamera.h"
#include "../../gamedef/objectClass/component/gdeOCComponent.h"
#include "../../gamedef/objectClass/inherit/gdeOCInherit.h"
#include "../../gamedef/objectClass/light/gdeOCLight.h"
#include "../../gamedef/objectClass/envmapprobe/gdeOCEnvMapProbe.h"
#include "../../gamedef/objectClass/particleemitter/gdeOCParticleEmitter.h"
#include "../../gamedef/objectClass/forceField/gdeOCForceField.h"
#include "../../gamedef/objectClass/navspace/gdeOCNavigationSpace.h"
#include "../../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"
#include "../../gamedef/objectClass/snappoint/gdeOCSnapPoint.h"
#include "../../gamedef/objectClass/speaker/gdeOCSpeaker.h"
#include "../../gamedef/objectClass/world/gdeOCWorld.h"
#include "../../gamedef/skin/gdeSkin.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/loadsave/igdeLoadAnimator.h>
#include <deigde/loadsave/igdeLoadParticleEmitter.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterManager.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceManager.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyManager.h>
#include <dragengine/resources/sky/deSkyInstance.h>
#include <dragengine/resources/sky/deSkyInstanceManager.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSoundManager.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/sound/deSpeakerManager.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>
#include <dragengine/resources/world/deWorld.h>


// Events
///////////

namespace {

class cCameraInteraction : public igdeMouseCameraListener {
	gdeViewActiveObject &pView;
	
public:
	cCameraInteraction( gdeViewActiveObject &view ) : pView( view ){
		SetEnabledAll( true );
	}
	
public:
	virtual void OnCameraChanged(){
		if( pView.GetGameDefinition() ){
			pView.GetGameDefinition()->NotifyCameraChanged();
		}
	}
};

}



// Class gdeViewActiveObject
//////////////////////////////

// Constructor, destructor
////////////////////////////

gdeViewActiveObject::gdeViewActiveObject( gdeWindowMain &windowMain ) :
igdeViewRenderWindow( windowMain.GetEnvironment() ),

pWindowMain( windowMain ),
pListener( NULL ),

pGameDefinition( NULL ),

pObjectClass( NULL ),

pShowEnvMapProbes( false ),
pShowNavBlockers( false )
{
	pListener = new gdeViewActiveObjectListener( *this );
	
	const deEngine &engine = *GetEngineController().GetEngine();
	pDebugDrawer.TakeOver( engine.GetDebugDrawerManager()->CreateDebugDrawer() );
	pDebugDrawer->SetXRay( true );
	
	pCameraInteraction.TakeOver( new cCameraInteraction( *this ) );
	
	AddListener( pCameraInteraction );
}

gdeViewActiveObject::~gdeViewActiveObject(){
	SetGameDefinition( NULL );
}



// Management
///////////////

void gdeViewActiveObject::ResetView(){
}

void gdeViewActiveObject::SetGameDefinition( gdeGameDefinition *gameDefinition ){
	if( gameDefinition == pGameDefinition ){
		return;
	}
	
	pCameraInteraction->SetCamera( NULL );
	SetRenderWorld( NULL );
	
	if( pGameDefinition ){
		pGameDefinition->RemoveListener( pListener );
		
		ClearResources();
		
		pPreviewModelBox = NULL;
		
		if( pDebugDrawer ){
			pGameDefinition->GetWorld()->RemoveDebugDrawer( pDebugDrawer );
		}
		
		pGameDefinition->FreeReference();
	}
	
	pGameDefinition = gameDefinition;
	
	if( gameDefinition ){
		gameDefinition->AddReference();
		SetRenderWorld( gameDefinition->GetCamera()->GetEngineCamera() );
		pCameraInteraction->SetCamera( gameDefinition->GetCamera() );
		
		deVirtualFileSystem * const vfs = gameDefinition->GetEnvironment()->GetFileSystemIGDE();
		deEngine &engine = *gameDefinition->GetEngine();
		deModelManager &modelManager = *engine.GetModelManager();
		
		pPreviewModelBox.TakeOver( modelManager.LoadModel( vfs, "/data/data/models/materialTest/box.demodel", "/" ) );
		
		gameDefinition->GetWorld()->AddDebugDrawer( pDebugDrawer );
		
		gameDefinition->AddListener( pListener );
	}
	
	pUpdateCameraFovRatio();
}



void gdeViewActiveObject::OnFrameUpdate( float elapsed ){
	igdeViewRenderWindow::OnFrameUpdate( elapsed );
	
	if( pGameDefinition ){
		pGameDefinition->Update( elapsed );
	}
	
	int i;
	const int componentCount = pOCComponents.GetCount();
	for( i=0; i<componentCount; i++ ){
		( ( gdeVAOComponent* )pOCComponents.GetAt( i ) )->Update( elapsed );
	}
	
	const int lightCount = pOCLights.GetCount();
	for( i=0; i<lightCount; i++ ){
		( ( gdeVAOLight* )pOCLights.GetAt( i ) )->Update( elapsed );
	}
}

void gdeViewActiveObject::OnResize(){
	igdeViewRenderWindow::OnResize();
	pUpdateCameraFovRatio();
}

void gdeViewActiveObject::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if( pGameDefinition ){
		SetRenderWorld( pGameDefinition->GetCamera()->GetEngineCamera() );
	}
}



void gdeViewActiveObject::ResetCameraPosition(){
	igdeCamera &camera = *pGameDefinition->GetCamera();
	
	switch( pGameDefinition->GetSelectedObjectType() ){
	case gdeGameDefinition::eotNoSelection:
	case gdeGameDefinition::eotCategoryObjectClass:
	case gdeGameDefinition::eotCategorySkin:
	case gdeGameDefinition::eotCategorySky:
	case gdeGameDefinition::eotCategoryParticleEmitter:
		// nothing
		break;
		
	case gdeGameDefinition::eotObjectClass:
	case gdeGameDefinition::eotOCBillboard:
	case gdeGameDefinition::eotOCCamera:
	case gdeGameDefinition::eotOCComponent:
	case gdeGameDefinition::eotOCEnvMapProbe:
	case gdeGameDefinition::eotOCLight:
	case gdeGameDefinition::eotOCNavigationBlocker:
	case gdeGameDefinition::eotOCNavigationSpace:
	case gdeGameDefinition::eotOCParticleEmitter:
	case gdeGameDefinition::eotOCForceField:
	case gdeGameDefinition::eotOCSnapPoint:
	case gdeGameDefinition::eotOCSpeaker:
	case gdeGameDefinition::eotOCWorld:
		pCenterOnObjectClass( camera );
		break;
		
	case gdeGameDefinition::eotSkin:
		pCenterOnSkin( camera );
		break;
		
	case gdeGameDefinition::eotSky:
		pCenterOnSky( camera );
		break;
		
	case gdeGameDefinition::eotParticleEmitter:
		pCenterOnParticleEmitter( camera );
		break;
	}
}

void gdeViewActiveObject::ClearResources(){
	deWorld &world = *pGameDefinition->GetWorld();
	
	pOCBillboards.RemoveAll();
	pOCCameras.RemoveAll();
	pOCEnvMapProbes.RemoveAll();
	pOCLights.RemoveAll();
	pOCParticleEmitters.RemoveAll();
	pOCForceFields.RemoveAll();
	pOCNavBlockers.RemoveAll();
	pOCNavSpaces.RemoveAll();
	pOCSnapPoints.RemoveAll();
	pOCSpeakers.RemoveAll();
	pOCWorlds.RemoveAll();
	
	pOCComponents.RemoveAll();
	
	if( pPreviewParticleEmitter ){
		if( pPreviewParticleEmitter->GetParentWorld() ){
			world.RemoveParticleEmitter( pPreviewParticleEmitter );
		}
		pPreviewParticleEmitter = NULL;
	}
	
	if( pPreviewSky ){
		if( pPreviewSky->GetParentWorld() ){
			world.RemoveSky( pPreviewSky );
		}
		pPreviewSky = NULL;
	}
	
	pPreviewSkin = NULL;
	
	if( pPreviewComponent ){
		if( pPreviewComponent->GetParentWorld() ){
			world.RemoveComponent( pPreviewComponent );
		}
		pPreviewComponent = NULL;
	}
	
	if( pObjectClass ){
		pObjectClass->FreeReference();
		pObjectClass = NULL;
	}
}

void gdeViewActiveObject::InitSelectedObject(){
	try{
		switch( pGameDefinition->GetSelectedObjectType() ){
		case gdeGameDefinition::eotNoSelection:
		case gdeGameDefinition::eotCategoryObjectClass:
		case gdeGameDefinition::eotCategorySkin:
		case gdeGameDefinition::eotCategorySky:
		case gdeGameDefinition::eotCategoryParticleEmitter:
			// nothing
			break;
			
		case gdeGameDefinition::eotObjectClass:
		case gdeGameDefinition::eotOCBillboard:
		case gdeGameDefinition::eotOCCamera:
		case gdeGameDefinition::eotOCComponent:
		case gdeGameDefinition::eotOCEnvMapProbe:
		case gdeGameDefinition::eotOCLight:
		case gdeGameDefinition::eotOCNavigationBlocker:
		case gdeGameDefinition::eotOCNavigationSpace:
		case gdeGameDefinition::eotOCParticleEmitter:
		case gdeGameDefinition::eotOCForceField:
		case gdeGameDefinition::eotOCSnapPoint:
		case gdeGameDefinition::eotOCSpeaker:
		case gdeGameDefinition::eotOCWorld:
			pInitObjectClass();
			break;
			
		case gdeGameDefinition::eotSkin:
			pInitSkin();
			break;
			
		case gdeGameDefinition::eotSky:
			pInitSky();
			break;
			
		case gdeGameDefinition::eotParticleEmitter:
			pInitParticleEmitter();
			break;
		}
		
	}catch( const deException &e ){
		ClearResources();
		e.PrintError();
		// TODO mark the error in the panels using warnings for example
	}
}

void gdeViewActiveObject::SelectedSubObjectChanged(){
	int i, count = pOCBillboards.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAOBillboard* )pOCBillboards.GetAt( i ) )->SelectedObjectChanged();
	}
	
	count = pOCCameras.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAOCamera* )pOCCameras.GetAt( i ) )->SelectedObjectChanged();
	}
	
	count = pOCComponents.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAOComponent* )pOCComponents.GetAt( i ) )->SelectedObjectChanged();
	}
	
	count = pOCEnvMapProbes.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAOEnvMapProbe* )pOCEnvMapProbes.GetAt( i ) )->SelectedObjectChanged();
	}
	
	count = pOCLights.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAOLight* )pOCLights.GetAt( i ) )->SelectedObjectChanged();
	}
	
	count = pOCNavBlockers.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAONavBlocker* )pOCNavBlockers.GetAt( i ) )->SelectedObjectChanged();
	}
	
	count = pOCNavSpaces.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAONavSpace* )pOCNavSpaces.GetAt( i ) )->SelectedObjectChanged();
	}
	
	count = pOCParticleEmitters.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAOParticleEmitter* )pOCParticleEmitters.GetAt( i ) )->SelectedObjectChanged();
	}
	
	count = pOCForceFields.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAOForceField* )pOCForceFields.GetAt( i ) )->SelectedObjectChanged();
	}
	
	count = pOCSnapPoints.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAOSnapPoint* )pOCSnapPoints.GetAt( i ) )->SelectedObjectChanged();
	}
	
	count = pOCSpeakers.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAOSpeaker* )pOCSpeakers.GetAt( i ) )->SelectedObjectChanged();
	}
	
	count = pOCWorlds.GetCount();
	for(i=0; i<count; i++){
		((gdeVAOWorld*)pOCWorlds.GetAt(i))->SelectedObjectChanged();
	}
}



deCollider *gdeViewActiveObject::GetAttachComponentCollider() const{
	if( pOCComponents.GetCount() == 0 ){
		return NULL;
	}
	
	return ( ( gdeVAOComponent* )pOCComponents.GetAt( 0 ) )->GetCollider();
}

void gdeViewActiveObject::RebuildOCComponents(){
	const decObjectList oldComponents( pOCComponents );
	pOCComponents.RemoveAll();
	if( pObjectClass ){
		pRebuildOCComponents( *pObjectClass, "" );
	}
	pOCReattachAllObjects();
}

void gdeViewActiveObject::RebuildOCComponent( gdeOCComponent *occomponent ){
	const int count = pOCComponents.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeVAOComponent * const vao = ( gdeVAOComponent* )pOCComponents.GetAt( i );
		if( vao->GetOCComponent() != occomponent ){
			continue;
		}
		
		vao->RebuildResources();
		pOCReattachAllObjects();
		break;
	}
}

void gdeViewActiveObject::UpdateOCComponentTexture( gdeOCComponent *occomponent,
gdeOCComponentTexture *texture ){
	const int count = pOCComponents.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeVAOComponent * const vao = ( gdeVAOComponent* )pOCComponents.GetAt( i );
		if( occomponent && vao->GetOCComponent() != occomponent ){
			continue;
		}
		
		vao->UpdateTexture( texture );
		break;
	}
}

void gdeViewActiveObject::RebuildOCBillboards(){
	const decObjectList oldBillboards( pOCBillboards );
	pOCBillboards.RemoveAll();
	if( pObjectClass ){
		pInitOCBillboards( *pObjectClass, "" );
	}
	pOCReattachAllObjects();
}

void gdeViewActiveObject::RebuildOCBillboard( gdeOCBillboard *ocbillboard ){
	const int count = pOCBillboards.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeVAOBillboard * const vao = ( gdeVAOBillboard* )pOCBillboards.GetAt( i );
		if( vao->GetOCBillboard() != ocbillboard ){
			continue;
		}
		
		vao->RebuildResources();
		pOCReattachAllObjects();
		break;
	}
}

void gdeViewActiveObject::RebuildOCCameras(){
	const decObjectList oldCameras( pOCCameras );
	pOCCameras.RemoveAll();
	if( pObjectClass ){
		pInitOCCameras( *pObjectClass, "" );
	}
}

void gdeViewActiveObject::RebuildOCCamera( gdeOCCamera *occamera ){
	const int count = pOCCameras.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeVAOCamera * const vao = ( gdeVAOCamera* )pOCCameras.GetAt( i );
		if( vao->GetOCCamera() != occamera ){
			continue;
		}
		
		vao->RebuildResources();
		break;
	}
}

void gdeViewActiveObject::RebuildOCLights(){
	const decObjectList oldLights( pOCLights );
	pOCLights.RemoveAll();
	if( pObjectClass ){
		pInitOCLights( *pObjectClass, "" );
	}
}

void gdeViewActiveObject::RebuildOCLight( gdeOCLight *oclight ){
	const int count = pOCLights.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeVAOLight * const vao = ( gdeVAOLight* )pOCLights.GetAt( i );
		if( vao->GetOCLight() != oclight ){
			continue;
		}
		
		vao->RebuildResources();
		break;
	}
}

void gdeViewActiveObject::RebuildOCEnvMapProbes(){
	const decObjectList oldEnvMapProbes( pOCEnvMapProbes );
	pOCEnvMapProbes.RemoveAll();
	if( pObjectClass ){
		pInitOCEnvMapProbes( *pObjectClass, "" );
	}
}

void gdeViewActiveObject::RebuildOCEnvMapProbe( gdeOCEnvMapProbe *ocenvMapProbe ){
	const int count = pOCEnvMapProbes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeVAOEnvMapProbe * const vao = ( gdeVAOEnvMapProbe* )pOCEnvMapProbes.GetAt( i );
		if( vao->GetOCEnvMapProbe() != ocenvMapProbe ){
			continue;
		}
		
		vao->RebuildResources();
		break;
	}
}

void gdeViewActiveObject::RebuildOCNavSpaces(){
	const decObjectList oldNavSpaces( pOCNavSpaces );
	pOCNavSpaces.RemoveAll();
	if( pObjectClass ){
		pInitOCNavigationSpaces( *pObjectClass, "" );
	}
}

void gdeViewActiveObject::RebuildOCNavSpace( gdeOCNavigationSpace *ocnavspace ){
	const int count = pOCNavSpaces.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeVAONavSpace * const vao = ( gdeVAONavSpace* )pOCNavSpaces.GetAt( i );
		if( vao->GetOCNavSpace() != ocnavspace ){
			continue;
		}
		
		vao->RebuildResources();
		break;
	}
}

void gdeViewActiveObject::RebuildOCNavBlockers(){
	const decObjectList oldNavBlockers( pOCNavBlockers );
	pOCNavBlockers.RemoveAll();
	if( pObjectClass ){
		pInitOCNavigationBlockers( *pObjectClass, "" );
	}
}

void gdeViewActiveObject::RebuildOCNavBlocker( gdeOCNavigationBlocker *ocnavblocker ){
	const int count = pOCNavBlockers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeVAONavBlocker * const vao = ( gdeVAONavBlocker* )pOCNavBlockers.GetAt( i );
		if( vao->GetOCNavBlocker() != ocnavblocker ){
			continue;
		}
		
		vao->RebuildResources();
		break;
	}
}

void gdeViewActiveObject::RebuildOCParticleEmitters(){
	const decObjectList oldEmitters( pOCParticleEmitters );
	pOCParticleEmitters.RemoveAll();
	if( pObjectClass ){
		pInitOCParticleEmitters( *pObjectClass, "" );
	}
}

void gdeViewActiveObject::RebuildOCParticleEmitter( gdeOCParticleEmitter *ocemitter ){
	const int count = pOCParticleEmitters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeVAOParticleEmitter * const vao = ( gdeVAOParticleEmitter* )pOCParticleEmitters.GetAt( i );
		if( vao->GetOCParticleEmitter() != ocemitter ){
			continue;
		}
		
		vao->RebuildResources();
		break;
	}
}

void gdeViewActiveObject::RebuildOCForceFields(){
	const decObjectList oldEmitters( pOCForceFields );
	pOCForceFields.RemoveAll();
	if( pObjectClass ){
		pInitOCForceFields( *pObjectClass, "" );
	}
}

void gdeViewActiveObject::RebuildOCForceField( gdeOCForceField *ocfield ){
	const int count = pOCForceFields.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeVAOForceField * const vao = ( gdeVAOForceField* )pOCForceFields.GetAt( i );
		if( vao->GetOCForceField() != ocfield ){
			continue;
		}
		
		vao->RebuildResources();
		break;
	}
}

void gdeViewActiveObject::RebuildOCSnapPoints(){
	const decObjectList oldSnapPoints( pOCSnapPoints );
	pOCSnapPoints.RemoveAll();
	if( pObjectClass ){
		pInitOCSnapPoints( *pObjectClass, "" );
	}
}

void gdeViewActiveObject::RebuildOCSnapPoint( gdeOCSnapPoint *ocsnapPoint ){
	const int count = pOCSnapPoints.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeVAOSnapPoint * const vao = ( gdeVAOSnapPoint* )pOCSnapPoints.GetAt( i );
		if( vao->GetOCSnapPoint() != ocsnapPoint ){
			continue;
		}
		
		vao->RebuildResources();
		break;
	}
}

void gdeViewActiveObject::RebuildOCSpeakers(){
	const decObjectList oldSpeakers( pOCSpeakers );
	pOCSpeakers.RemoveAll();
	if( pObjectClass ){
		pInitOCSpeakers( *pObjectClass, "" );
	}
}

void gdeViewActiveObject::RebuildOCSpeaker( gdeOCSpeaker *ocspeaker ){
	const int count = pOCSpeakers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeVAOSpeaker * const vao = ( gdeVAOSpeaker* )pOCSpeakers.GetAt( i );
		if( vao->GetOCSpeaker() != ocspeaker ){
			continue;
		}
		
		vao->RebuildResources();
		break;
	}
}

void gdeViewActiveObject::RebuildOCWorlds(){
	const decObjectList oldWorlds(pOCWorlds);
	pOCWorlds.RemoveAll();
	if(pObjectClass){
		pInitOCWorlds(*pObjectClass, "");
	}
}

void gdeViewActiveObject::RebuildOCWorld(gdeOCWorld *ocworld){
	const int count = pOCWorlds.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		gdeVAOWorld * const vao = (gdeVAOWorld*)pOCWorlds.GetAt(i);
		if(vao->GetOCWorld() != ocworld){
			continue;
		}
		
		vao->RebuildResources();
		break;
	}
}



void gdeViewActiveObject::SetShowEnvMapProbes( bool show ){
	if( show == pShowEnvMapProbes ){
		return;
	}
	
	pShowEnvMapProbes = show;
	
	const int count = pOCEnvMapProbes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( gdeVAOEnvMapProbe* )pOCEnvMapProbes.GetAt( i ) )->UpdateDDVisibility();
	}
}

void gdeViewActiveObject::SetShowNavBlockers( bool show ){
	if( show == pShowNavBlockers ){
		return;
	}
	
	pShowNavBlockers = show;
	
	int i, count = pOCNavSpaces.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAONavSpace* )pOCNavSpaces.GetAt( i ) )->UpdateDDVisibility();
	}
	
	count = pOCNavBlockers.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAONavBlocker* )pOCNavBlockers.GetAt( i ) )->UpdateDDVisibility();
	}
}



// Private Functions
//////////////////////

void gdeViewActiveObject::pCleanUp(){
	SetGameDefinition( NULL );
}



void gdeViewActiveObject::pUpdateCameraFovRatio(){
	if( ! pGameDefinition ){
		return;
	}
	
	const decPoint size( GetRenderAreaSize() );
	pGameDefinition->SetViewRatio( ( float )decMath::max( size.x, 1 )
		/ ( float )decMath::max( size.y, 1 ) );
}



void gdeViewActiveObject::pInitObjectClass(){
	pObjectClass = pGameDefinition->GetActiveObjectClass();
	if( ! pObjectClass ){
		return;
	}
	pObjectClass->AddReference();
	pInitObjectClassOCs( *pObjectClass, "", igdeGDClass::FilterSubObjectsAll );
	pAddComponentShadowIgnore();
}

void gdeViewActiveObject::pInitSkin(){
	const gdeSkin * const skin = pGameDefinition->GetActiveSkin();
	if( ! skin ){
		return;
	}
	
	deVirtualFileSystem * const vfs = pGameDefinition->GetPreviewVFS();
	deEngine &engine = *pGameDefinition->GetEngine();
	pPreviewSkin.TakeOver( engine.GetSkinManager()->LoadSkin( vfs, skin->GetPath(), "/" ) );
	
	pPreviewComponent.TakeOver( engine.GetComponentManager()->CreateComponent( pPreviewModelBox, pPreviewSkin ) );
	pGameDefinition->GetWorld()->AddComponent( pPreviewComponent );
	
	if( pPreviewComponent->GetTextureCount() == 0 ){
		// assign skin also to texture directly. skins are supposed to have one texture but
		// the name is not defined. by using a generic model and allowing the user to
		// change the preview model it can be cumbersome to match the texture. by assigning
		// the skin to the texture this problem is solved
		pPreviewComponent->GetTextureAt( 0 ).SetSkin( pPreviewSkin );
		pPreviewComponent->GetTextureAt( 0 ).SetTexture( 0 );
		pPreviewComponent->NotifyTextureChanged( 0 );
	}
}

void gdeViewActiveObject::pInitSky(){
	// TODO
}

void gdeViewActiveObject::pInitParticleEmitter(){
	const gdeParticleEmitter * const particleEmitter = pGameDefinition->GetActiveParticleEmitter();
	if( ! particleEmitter ){
		return;
	}
	
	deVirtualFileSystem * const vfs = pGameDefinition->GetPreviewVFS();
	deEngine &engine = *pGameDefinition->GetEngine();
	deParticleEmitterReference engEmitter;
	decBaseFileReaderReference reader;
	
	igdeLoadParticleEmitter loader( *pGameDefinition->GetEnvironment(),
		pGameDefinition->GetEnvironment()->GetLogger(), "gdeGDEditActiveObject" );
	
	decPath path;
	path.SetFromUnix( particleEmitter->GetPath() );
	
	engEmitter.TakeOver( engine.GetParticleEmitterManager()->CreateParticleEmitter() );
	reader.TakeOver( vfs->OpenFileForReading( path ) );
	loader.Load( particleEmitter->GetPath(), engEmitter, reader );
	
	pPreviewParticleEmitter.TakeOver( engine.GetParticleEmitterInstanceManager()->CreateInstance() );
	pPreviewParticleEmitter->SetEmitter( engEmitter );
	pPreviewParticleEmitter->SetEnableCasting( true );
	
	pGameDefinition->GetWorld()->AddParticleEmitter( pPreviewParticleEmitter );
}



void gdeViewActiveObject::pInitObjectClassOCs( const gdeObjectClass &objectClass,
const decString &propertyPrefix, int filter ){
	if( pOCComponents.GetCount() == 0
	&& ( filter & igdeGDClass::efsoComponents ) == igdeGDClass::efsoComponents ){
		pInitOCComponents( objectClass, propertyPrefix );
	}
	
	if( pOCBillboards.GetCount() == 0
	&& ( filter & igdeGDClass::efsoBillboards ) == igdeGDClass::efsoBillboards ){
		pInitOCBillboards( objectClass, propertyPrefix );
	}
	if( pOCCameras.GetCount() == 0 ){
		pInitOCCameras( objectClass, propertyPrefix );
	}
	if( pOCEnvMapProbes.GetCount() == 0
	&& ( filter & igdeGDClass::efsoEnvMapProbes ) == igdeGDClass::efsoEnvMapProbes ){
		pInitOCEnvMapProbes( objectClass, propertyPrefix );
	}
	if( pOCLights.GetCount() == 0
	&& ( filter & igdeGDClass::efsoLights ) == igdeGDClass::efsoLights ){
		pInitOCLights( objectClass, propertyPrefix );
	}
	if( pOCNavSpaces.GetCount() == 0
	&& ( filter & igdeGDClass::efsoNavigationSpaces ) == igdeGDClass::efsoNavigationSpaces ){
		pInitOCNavigationSpaces( objectClass, propertyPrefix );
	}
	if( pOCNavBlockers.GetCount() == 0
	&& ( filter & igdeGDClass::efsoNavigationBlockers ) == igdeGDClass::efsoNavigationBlockers ){
		pInitOCNavigationBlockers( objectClass, propertyPrefix );
	}
	if( pOCParticleEmitters.GetCount() == 0
	&& ( filter & igdeGDClass::efsoParticleEmitters ) == igdeGDClass::efsoParticleEmitters ){
		pInitOCParticleEmitters( objectClass, propertyPrefix );
	}
	if( pOCForceFields.GetCount() == 0
	&& ( filter & igdeGDClass::efsoForceFields ) == igdeGDClass::efsoForceFields ){
		pInitOCForceFields( objectClass, propertyPrefix );
	}
	if( pOCSnapPoints.GetCount() == 0
	&& ( filter & igdeGDClass::efsoSnapPoints ) == igdeGDClass::efsoSnapPoints ){
		pInitOCSnapPoints( objectClass, propertyPrefix );
	}
	if( pOCSpeakers.GetCount() == 0
	&& ( filter & igdeGDClass::efsoSpeakers ) == igdeGDClass::efsoSpeakers ){
		pInitOCSpeakers( objectClass, propertyPrefix );
	}
	if( pOCWorlds.GetCount() == 0
	&& (filter & igdeGDClass::efsoWorlds) == igdeGDClass::efsoWorlds){
		pInitOCWorlds(objectClass, propertyPrefix);
	}
	
	if( pGameDefinition ){
		const gdeOCInheritList inherits = objectClass.GetInherits();
		const int inheritCount = inherits.GetCount();
		int i;
		
		filter &= objectClass.GetInheritSubObjects();
		
		for( i=0; i<inheritCount; i++ ){
			const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass( inherits.GetAt( i )->GetName() );
			if( ioc ){
				pInitObjectClassOCs( *ioc, propertyPrefix + ioc->GetDefaultInheritPropertyPrefix(), filter );
			}
		}
	}
}

void gdeViewActiveObject::pInitOCComponents( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	const gdeOCComponentList &list = objectClass.GetComponents();
	const int count = list.GetCount();
	deObjectReference vao;
	int i;
	
	for( i=0; i<count; i++ ){
		vao.TakeOver( new gdeVAOComponent( *this, objectClass, propertyPrefix, list.GetAt( i ) ) );
		pOCComponents.Add( vao );
	}
}

void gdeViewActiveObject::pInitOCBillboards( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	const gdeOCBillboardList &list = objectClass.GetBillboards();
	const int count = list.GetCount();
	deObjectReference vao;
	int i;
	
	for( i=0; i<count; i++ ){
		vao.TakeOver( new gdeVAOBillboard( *this, objectClass, propertyPrefix, list.GetAt( i ) ) );
		pOCBillboards.Add( vao );
	}
}

void gdeViewActiveObject::pInitOCCameras( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	const gdeOCCameraList &list = objectClass.GetCameras();
	const int count = list.GetCount();
	deObjectReference vao;
	int i;
	
	for( i=0; i<count; i++ ){
		vao.TakeOver( new gdeVAOCamera( *this, objectClass, propertyPrefix, list.GetAt( i ) ) );
		pOCCameras.Add( vao );
	}
}

void gdeViewActiveObject::pInitOCLights( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	const gdeOCLightList &list = objectClass.GetLights();
	const int count = list.GetCount();
	deObjectReference vao;
	int i;
	
	for( i=0; i<count; i++ ){
		vao.TakeOver( new gdeVAOLight( *this, objectClass, propertyPrefix, list.GetAt( i ) ) );
		pOCLights.Add( vao );
	}
}

void gdeViewActiveObject::pInitOCParticleEmitters( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	const gdeOCParticleEmitterList &list = objectClass.GetParticleEmitters();
	const int count = list.GetCount();
	deObjectReference vao;
	int i;
	
	for( i=0; i<count; i++ ){
		vao.TakeOver( new gdeVAOParticleEmitter( *this, objectClass, propertyPrefix, list.GetAt( i ) ) );
		pOCParticleEmitters.Add( vao );
	}
}

void gdeViewActiveObject::pInitOCForceFields( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	const gdeOCForceFieldList &list = objectClass.GetForceFields();
	const int count = list.GetCount();
	deObjectReference vao;
	int i;
	
	for( i=0; i<count; i++ ){
		vao.TakeOver( new gdeVAOForceField( *this, objectClass, propertyPrefix, list.GetAt( i ) ) );
		pOCForceFields.Add( vao );
	}
}

void gdeViewActiveObject::pInitOCEnvMapProbes( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	const gdeOCEnvMapProbeList &list = objectClass.GetEnvMapProbes();
	const int count = list.GetCount();
	deObjectReference vao;
	int i;
	
	for( i=0; i<count; i++ ){
		vao.TakeOver( new gdeVAOEnvMapProbe( *this, objectClass, propertyPrefix, list.GetAt( i ) ) );
		pOCEnvMapProbes.Add( vao );
	}
}

void gdeViewActiveObject::pInitOCNavigationSpaces( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	const gdeOCNavigationSpaceList &list = objectClass.GetNavigationSpaces();
	const int count = list.GetCount();
	deObjectReference aoc;
	int i;
	
	for( i=0; i<count; i++ ){
		aoc.TakeOver( new gdeVAONavSpace( *this, objectClass, propertyPrefix, list.GetAt( i ) ) );
		pOCNavSpaces.Add( aoc );
	}
}

void gdeViewActiveObject::pInitOCNavigationBlockers( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	const gdeOCNavigationBlockerList &list = objectClass.GetNavigationBlockers();
	const int count = list.GetCount();
	deObjectReference aoc;
	int i;
	
	for( i=0; i<count; i++ ){
		aoc.TakeOver( new gdeVAONavBlocker( *this, objectClass, propertyPrefix, list.GetAt( i ) ) );
		pOCNavBlockers.Add( aoc );
	}
}

void gdeViewActiveObject::pInitOCSnapPoints( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	const gdeOCSnapPointList &list = objectClass.GetSnapPoints();
	const int count = list.GetCount();
	deObjectReference vao;
	int i;
	
	for( i=0; i<count; i++ ){
		vao.TakeOver( new gdeVAOSnapPoint( *this, objectClass, propertyPrefix, list.GetAt( i ) ) );
		pOCSnapPoints.Add( vao );
	}
}

void gdeViewActiveObject::pInitOCSpeakers( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	const gdeOCSpeakerList &list = objectClass.GetSpeakers();
	const int count = list.GetCount();
	deObjectReference vao;
	int i;
	
	for( i=0; i<count; i++ ){
		vao.TakeOver( new gdeVAOSpeaker( *this, objectClass, propertyPrefix, list.GetAt( i ) ) );
		pOCSpeakers.Add( vao );
	}
}

void gdeViewActiveObject::pInitOCWorlds(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	const gdeOCWorldList &list = objectClass.GetWorlds();
	const int count = list.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pOCWorlds.Add(gdeVAOWorld::Ref::New(new gdeVAOWorld(
			*this, objectClass, propertyPrefix, list.GetAt(i))));
	}
}

void gdeViewActiveObject::pAddComponentShadowIgnore(){
	const int componentCount = pOCComponents.GetCount();
	const int lightCount = pOCLights.GetCount();
	int i, j;
	
	for( i=0; i<componentCount; i++ ){
		const gdeVAOComponent &occomp = *( ( gdeVAOComponent* )pOCComponents.GetAt( i ) );
		if( ! occomp.GetOCComponent()->GetLightShadowIgnore() ){
			continue;
		}
		
		for( j=0; j<lightCount; j++ ){
			( ( gdeVAOLight* )pOCLights.GetAt( j ) )->ShadowIgnoreComponent( occomp.GetComponent() );
		}
	}
}

void gdeViewActiveObject::pRebuildOCComponents( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	pInitOCComponents( objectClass, propertyPrefix );
	
	if( pGameDefinition ){
		const gdeOCInheritList inherits = objectClass.GetInherits();
		const int inheritCount = inherits.GetCount();
		int i;
		for( i=0; i<inheritCount; i++ ){
			const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass( inherits.GetAt( i )->GetName() );
			if( ioc ){
				pRebuildOCComponents( *ioc, propertyPrefix + ioc->GetDefaultInheritPropertyPrefix() );
			}
		}
	}
}

void gdeViewActiveObject::pRebuildOCBillboards( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	pInitOCBillboards( objectClass, propertyPrefix );
	
	if( pGameDefinition ){
		const gdeOCInheritList inherits = objectClass.GetInherits();
		const int inheritCount = inherits.GetCount();
		int i;
		for( i=0; i<inheritCount; i++ ){
			const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass( inherits.GetAt( i )->GetName() );
			if( ioc ){
				pRebuildOCBillboards( *ioc, propertyPrefix + ioc->GetDefaultInheritPropertyPrefix() );
			}
		}
	}
}

void gdeViewActiveObject::pRebuildOCCameras( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	pInitOCCameras( objectClass, propertyPrefix );
	
	if( pGameDefinition ){
		const gdeOCInheritList inherits = objectClass.GetInherits();
		const int inheritCount = inherits.GetCount();
		int i;
		for( i=0; i<inheritCount; i++ ){
			const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass( inherits.GetAt( i )->GetName() );
			if( ioc ){
				pRebuildOCCameras( *ioc, propertyPrefix + ioc->GetDefaultInheritPropertyPrefix() );
			}
		}
	}
}

void gdeViewActiveObject::pRebuildOCLights( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	pInitOCLights( objectClass, propertyPrefix );
	
	if( pGameDefinition ){
		const gdeOCInheritList inherits = objectClass.GetInherits();
		const int inheritCount = inherits.GetCount();
		int i;
		for( i=0; i<inheritCount; i++ ){
			const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass( inherits.GetAt( i )->GetName() );
			if( ioc ){
				pRebuildOCLights( *ioc, propertyPrefix + ioc->GetDefaultInheritPropertyPrefix() );
			}
		}
	}
}

void gdeViewActiveObject::pRebuildOCEnvMapProbes( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	pInitOCEnvMapProbes( objectClass, propertyPrefix );
	
	if( pGameDefinition ){
		const gdeOCInheritList inherits = objectClass.GetInherits();
		const int inheritCount = inherits.GetCount();
		int i;
		for( i=0; i<inheritCount; i++ ){
			const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass( inherits.GetAt( i )->GetName() );
			if( ioc ){
				pRebuildOCEnvMapProbes( *ioc, propertyPrefix + ioc->GetDefaultInheritPropertyPrefix() );
			}
		}
	}
}

void gdeViewActiveObject::pRebuildOCNavigationSpaces( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	pInitOCNavigationSpaces( objectClass, propertyPrefix );
	
	if( pGameDefinition ){
		const gdeOCInheritList inherits = objectClass.GetInherits();
		const int inheritCount = inherits.GetCount();
		int i;
		for( i=0; i<inheritCount; i++ ){
			const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass( inherits.GetAt( i )->GetName() );
			if( ioc ){
				pRebuildOCNavigationSpaces( *ioc, propertyPrefix + ioc->GetDefaultInheritPropertyPrefix() );
			}
		}
	}
}

void gdeViewActiveObject::pRebuildOCNavigationBlockers( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	pInitOCNavigationBlockers( objectClass, propertyPrefix );
	
	if( pGameDefinition ){
		const gdeOCInheritList inherits = objectClass.GetInherits();
		const int inheritCount = inherits.GetCount();
		int i;
		for( i=0; i<inheritCount; i++ ){
			const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass( inherits.GetAt( i )->GetName() );
			if( ioc ){
				pRebuildOCNavigationBlockers( *ioc, propertyPrefix + ioc->GetDefaultInheritPropertyPrefix() );
			}
		}
	}
}

void gdeViewActiveObject::pRebuildOCParticleEmitters( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	pInitOCParticleEmitters( objectClass, propertyPrefix );
	
	if( pGameDefinition ){
		const gdeOCInheritList inherits = objectClass.GetInherits();
		const int inheritCount = inherits.GetCount();
		int i;
		for( i=0; i<inheritCount; i++ ){
			const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass( inherits.GetAt( i )->GetName() );
			if( ioc ){
				pRebuildOCParticleEmitters( *ioc, propertyPrefix + ioc->GetDefaultInheritPropertyPrefix() );
			}
		}
	}
}

void gdeViewActiveObject::pRebuildOCForceFields( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	pInitOCForceFields( objectClass, propertyPrefix );
	
	if( pGameDefinition ){
		const gdeOCInheritList inherits = objectClass.GetInherits();
		const int inheritCount = inherits.GetCount();
		int i;
		for( i=0; i<inheritCount; i++ ){
			const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass( inherits.GetAt( i )->GetName() );
			if( ioc ){
				pRebuildOCForceFields( *ioc, propertyPrefix + ioc->GetDefaultInheritPropertyPrefix() );
			}
		}
	}
}

void gdeViewActiveObject::pRebuildOCSnapPoints( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	pInitOCSnapPoints( objectClass, propertyPrefix );
	
	if( pGameDefinition ){
		const gdeOCInheritList inherits = objectClass.GetInherits();
		const int inheritCount = inherits.GetCount();
		int i;
		for( i=0; i<inheritCount; i++ ){
			const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass( inherits.GetAt( i )->GetName() );
			if( ioc ){
				pRebuildOCSnapPoints( *ioc, propertyPrefix + ioc->GetDefaultInheritPropertyPrefix() );
			}
		}
	}
}

void gdeViewActiveObject::pRebuildOCSpeakers( const gdeObjectClass &objectClass, const decString &propertyPrefix ){
	pInitOCSpeakers( objectClass, propertyPrefix );
	
	if( pGameDefinition ){
		const gdeOCInheritList inherits = objectClass.GetInherits();
		const int inheritCount = inherits.GetCount();
		int i;
		for( i=0; i<inheritCount; i++ ){
			const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass( inherits.GetAt( i )->GetName() );
			if( ioc ){
				pRebuildOCSpeakers( *ioc, propertyPrefix + ioc->GetDefaultInheritPropertyPrefix() );
			}
		}
	}
}

void gdeViewActiveObject::pRebuildOCWorlds(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	pInitOCWorlds(objectClass, propertyPrefix);
	
	if(pGameDefinition){
		const gdeOCInheritList inherits = objectClass.GetInherits();
		const int inheritCount = inherits.GetCount();
		int i;
		for(i=0; i<inheritCount; i++){
			const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass(inherits.GetAt(i)->GetName());
			if(ioc){
				pRebuildOCWorlds(*ioc, propertyPrefix + ioc->GetDefaultInheritPropertyPrefix());
			}
		}
	}
}



void gdeViewActiveObject::pOCReattachAllObjects(){
	// remove all attachments
	int i, count = pOCComponents.GetCount();
	for( i=0; i<count; i++ ){
		deCollider * const collider = ( ( gdeVAOComponent* )pOCComponents.GetAt( i ) )->GetCollider();
		if( collider ){
			collider->RemoveAllAttachments();
		}
	}
	
	// attach all objects
	count = pOCLights.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAOLight* )pOCLights.GetAt( i ) )->AttachResources();
	}
	
	count = pOCParticleEmitters.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAOParticleEmitter* )pOCParticleEmitters.GetAt( i ) )->AttachResources();
	}
	
	count = pOCForceFields.GetCount();
	for( i=0; i<count; i++ ){
		( ( gdeVAOForceField* )pOCForceFields.GetAt( i ) )->AttachResources();
	}
}



void gdeViewActiveObject::pCenterOnObjectClass( igdeCamera &camera ){
	decVector resMinExtend, resMaxExtend;
	decVector boxMinExtend, boxMaxExtend;
	bool first = true;
	int i;
	
	const int componentCount = pOCComponents.GetCount();
	for( i=0; i<componentCount; i++ ){
		( ( gdeVAOComponent* )pOCComponents.GetAt( i ) )->GetExtends( resMinExtend, resMaxExtend );
		
		if( first ){
			boxMinExtend = resMinExtend;
			boxMaxExtend = resMaxExtend;
			first = false;
			
		}else{
			boxMinExtend.SetSmallest( resMinExtend );
			boxMaxExtend.SetLargest( resMaxExtend );
		}
	}
	
	const int billboardCount = pOCBillboards.GetCount();
	for( i=0; i<billboardCount; i++ ){
		( ( gdeVAOBillboard* )pOCBillboards.GetAt( i ) )->GetExtends( resMinExtend, resMaxExtend );
		
		if( first ){
			boxMinExtend = resMinExtend;
			boxMaxExtend = resMaxExtend;
			first = false;
			
		}else{
			boxMinExtend.SetSmallest( resMinExtend );
			boxMaxExtend.SetLargest( resMaxExtend );
		}
	}
	
	pCenterOnBox( camera, boxMinExtend, boxMaxExtend );
}

void gdeViewActiveObject::pCenterOnSkin( igdeCamera &camera ){
	const gdeSkin * const skin = pGameDefinition->GetActiveSkin();
	if( ! skin ){
		return;
	}
	
	const decVector extends( 1.0f, 1.0f, 1.0f );
	pCenterOnBox( camera, -extends, extends );
}

void gdeViewActiveObject::pCenterOnSky( igdeCamera &camera ){
	camera.SetPosition( decDVector() );
	camera.SetOrientation( decVector() );
	camera.SetDistance( 0.0f );
}

void gdeViewActiveObject::pCenterOnParticleEmitter( igdeCamera &camera ){
	const decVector extends( 1.0f, 1.0f, 1.0f );
	pCenterOnBox( camera, -extends, extends );
}

void gdeViewActiveObject::pCenterOnBox( igdeCamera &camera,
const decVector &minExtend, const decVector &maxExtend ){
	const decVector viewRotation( -10.0f, 10.0f, 0.0f );
	const decMatrix matrixRotation( decMatrix::CreateRotation( viewRotation * DEG2RAD ) );
	
	const decVector halfSize( ( maxExtend - minExtend ) * 0.5f );
	const decVector corners[ 8 ] = {
		decVector( halfSize.x, halfSize.y, halfSize.z ),
		decVector( -halfSize.x, halfSize.y, halfSize.z ),
		decVector( halfSize.x, halfSize.y, -halfSize.z ),
		decVector( -halfSize.x, halfSize.y, -halfSize.z ),
		decVector( halfSize.x, -halfSize.y, halfSize.z ),
		decVector( -halfSize.x, -halfSize.y, halfSize.z ),
		decVector( halfSize.x, -halfSize.y, -halfSize.z ),
		decVector( -halfSize.x, -halfSize.y, -halfSize.z ) };
	
	const float halfFovX = tanf( camera.GetFov() * 0.5f * DEG2RAD );
	const float halfFovY = tanf( ( camera.GetFov()
		/ pGameDefinition->GetViewRatio() ) * 0.5f * DEG2RAD );
	
	const float borderScale = 1.1f; // leave 10% space between object and border
	
	float viewDistance = 0.0f;
	int i;
	for( i=0; i<8; i++ ){
		const decVector corner( matrixRotation * corners[ i ] );
		if( corner.z < 0.0f ){
			continue; // back side vertex are always smaller
		}
		
		viewDistance = decMath::max( viewDistance, corner.z + decMath::max(
			( corner.x * borderScale ) / halfFovX,
			( corner.y * borderScale ) / halfFovY ) );
	}
	
	camera.SetPosition( decDVector( ( minExtend + maxExtend ) * 0.5f ) );
	camera.SetOrientation( viewRotation + decVector( 0.0f, 180.0f, 0.0f ) );
	camera.SetDistance( viewDistance );
}
