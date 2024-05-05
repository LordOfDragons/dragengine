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

#include "gdeVAOParticleEmitter.h"
#include "gdeViewActiveObject.h"
#include "../gdeWindowMain.h"
#include "../../gdEditor.h"
#include "../../configuration/gdeConfiguration.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/particleemitter/gdeOCParticleEmitter.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWCoordSysArrows.h>
#include <deigde/loadsave/igdeLoadParticleEmitter.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceManager.h>
#include <dragengine/resources/particle/deParticleEmitterManager.h>
#include <dragengine/resources/world/deWorld.h>



// Class gdeVAOParticleEmitter
//////////////////////

// Constructor, destructor
////////////////////////////

gdeVAOParticleEmitter::gdeVAOParticleEmitter( gdeViewActiveObject &view,
	const gdeObjectClass &objectClass, const decString &propertyPrefix,
	gdeOCParticleEmitter *ocemitter ) :
gdeVAOSubObject( view, objectClass, propertyPrefix ),
pOCParticleEmitter( ocemitter ),
pDDSCenter( NULL ),
pDDSCoordSystem( NULL )
{
	if( ! ocemitter ){
		DETHROW( deeInvalidParam );
	}
	
	pOCParticleEmitter->AddReference();
	
	try{
		pCreateDebugDrawer();
		pCreateParticleEmitter();
		pUpdateDDShapes();
		pUpdateDDShapeColor();
		
		AttachResources();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

gdeVAOParticleEmitter::~gdeVAOParticleEmitter(){
	pCleanUp();
}



// Management
///////////////

void gdeVAOParticleEmitter::RebuildResources(){
	DetachResources();
	pReleaseResources();
	
	pCreateParticleEmitter();
	AttachResources();
	
	pUpdateDDShapes();
	pUpdateDDShapeColor();
}

void gdeVAOParticleEmitter::AttachResources(){
	if( ! pInstance ){
		return;
	}
	
	deCollider * const attachCollider = pView.GetAttachComponentCollider();
	if( ! attachCollider ){
		return;
	}
	
	const decVector position( PropertyVector( pOCParticleEmitter->GetPropertyName(
		gdeOCParticleEmitter::epAttachPosition ), pOCParticleEmitter->GetPosition() ) );
	const decQuaternion orientation( PropertyQuaternion( pOCParticleEmitter->GetPropertyName(
		gdeOCParticleEmitter::epAttachRotation ), pOCParticleEmitter->GetRotation() ) );
	
	deColliderAttachment *attachment = NULL;
	try{
		// attach particleEmitter
		attachment = new deColliderAttachment( pInstance );
		attachment->SetPosition( position );
		attachment->SetOrientation( orientation );
		attachment->SetAttachType( deColliderAttachment::eatStatic );
		
		attachCollider->AddAttachment( attachment );
		attachment = NULL;
		
		// attach debug drawer
		attachment = new deColliderAttachment( pDebugDrawer );
		attachment->SetPosition( position );
		attachment->SetOrientation( orientation );
		attachment->SetAttachType( deColliderAttachment::eatStatic );
		
		attachCollider->AddAttachment( attachment );
		attachment = NULL;
		
	}catch( const deException & ){
		if( attachment ){
			delete attachment;
		}
		throw;
	}
}

void gdeVAOParticleEmitter::DetachResources(){
	if( ! pInstance ){
		return;
	}
	
	deCollider * const attachCollider = pView.GetAttachComponentCollider();
	if( ! attachCollider ){
		return;
	}
	
	deColliderAttachment *attachment = NULL;
	attachment = attachCollider->GetAttachmentWith( pInstance );
	if( attachment ){
		attachCollider->RemoveAttachment( attachment );
	}
	
	attachment = attachCollider->GetAttachmentWith( pDebugDrawer );
	if( attachment ){
		attachCollider->RemoveAttachment( attachment );
	}
}

void gdeVAOParticleEmitter::SelectedObjectChanged(){
	pUpdateDDShapeColor();
}



// Private functions
//////////////////////

void gdeVAOParticleEmitter::pCleanUp(){
	DetachResources();
	pReleaseResources();
	
	if( pDDSCoordSystem ){
		delete pDDSCoordSystem;
	}
	if( pDDSCenter ){
		delete pDDSCenter;
	}
	if( pDebugDrawer ){
		pView.GetGameDefinition()->GetWorld()->RemoveDebugDrawer( pDebugDrawer );
		pDebugDrawer = NULL;
	}
	
	if( pOCParticleEmitter ){
		pOCParticleEmitter->FreeReference();
	}
}



void gdeVAOParticleEmitter::pCreateDebugDrawer(){
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	
	// create debug drawer
	pDebugDrawer.TakeOver( engine.GetDebugDrawerManager()->CreateDebugDrawer() );
	pDebugDrawer->SetXRay( true );
	pView.GetGameDefinition()->GetWorld()->AddDebugDrawer( pDebugDrawer );
	
	// create center shape
	pDDSCenter = new igdeWDebugDrawerShape;
	pDDSCenter->AddSphereShape( 0.05f, decVector() );
	pDDSCenter->SetParentDebugDrawer( pDebugDrawer );
	
	// create coordinate system shape
	pDDSCoordSystem = new igdeWCoordSysArrows;
	pDDSCoordSystem->SetArrowLength( 0.2f );
	pDDSCoordSystem->SetArrowSize( 0.01f );
	pDDSCoordSystem->SetParentDebugDrawer( pDebugDrawer );
}

void gdeVAOParticleEmitter::pCreateParticleEmitter(){
	decString path( PropertyString( pOCParticleEmitter->GetPropertyName(
		gdeOCParticleEmitter::epPath ), pOCParticleEmitter->GetPath() ) );
	if( path.IsEmpty() ){
		return;
	}
	
	// load particle emitter
	deVirtualFileSystem * const vfs = pView.GetGameDefinition()->GetPreviewVFS();
	igdeEnvironment &environment = pView.GetWindowMain().GetEnvironment();
	igdeLoadParticleEmitter loader( environment, environment.GetLogger(), "gdeVAOParticleEmitter" );
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	decBaseFileReaderReference reader;
	
	try{
		pEmitter.TakeOver( engine.GetParticleEmitterManager()->CreateParticleEmitter() );
		reader.TakeOver( vfs->OpenFileForReading( decPath::CreatePathUnix( path ) ) );
		loader.Load( path, pEmitter, reader );
		
	}catch( const deException &e ){
		pEmitter = NULL;
		environment.GetLogger()->LogException( LOGSOURCE, e );
		return;
	}
	
	// create particle emitter instance
	pInstance.TakeOver( engine.GetParticleEmitterInstanceManager()->CreateInstance() );
	pInstance->SetEmitter( pEmitter );
	
	decLayerMask collisionMask;
	collisionMask.SetBit( 0 );
	const decCollisionFilter collisionFilter( collisionMask );
	pInstance->SetCollisionFilter( collisionFilter );
	
	pInstance->SetEnableCasting( true );
	pView.GetGameDefinition()->GetWorld()->AddParticleEmitter( pInstance );
}

void gdeVAOParticleEmitter::pUpdateDDShapes(){
}

void gdeVAOParticleEmitter::pUpdateDDShapeColor(){
	const gdeConfiguration &config = pView.GetWindowMain().GetConfiguration();
	
	if( pView.GetGameDefinition()->GetSelectedObjectType() == gdeGameDefinition::eotOCParticleEmitter
	&& pView.GetGameDefinition()->GetActiveOCParticleEmitter() == pOCParticleEmitter ){
		pDDSCenter->SetEdgeColor( decColor( config.GetColorParticleEmitterActive(), 1.0f ) );
		pDDSCenter->SetFillColor( config.GetColorParticleEmitterActive() );
		
	}else{
		pDDSCenter->SetEdgeColor( decColor( config.GetColorParticleEmitter(), 0.25f ) );
		pDDSCenter->SetFillColor( config.GetColorParticleEmitter() );
	}
}



void gdeVAOParticleEmitter::pReleaseResources(){
	deWorld &world = *pView.GetGameDefinition()->GetWorld();
	
	if( pInstance ){
		world.RemoveParticleEmitter( pInstance );
		pInstance = NULL;
	}
	pEmitter = NULL;
}
