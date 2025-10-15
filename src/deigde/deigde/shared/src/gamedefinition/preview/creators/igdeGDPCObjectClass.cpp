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

#include "igdeGDPCObjectClass.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../gamedefinition/igdeGameDefinition.h"
#include "../../../gamedefinition/class/igdeGDClass.h"
#include "../../../gamedefinition/sky/igdeGDSkyManager.h"
#include "../../../gameproject/igdeGameProject.h"
#include "../../../gui/wrapper/igdeWSky.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/light/deLightManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>



// Async listener
///////////////////

igdeGDPCObjectClass::cAsyncFinished::cAsyncFinished() :
asyncLoadFinished( false ){
}

void igdeGDPCObjectClass::cAsyncFinished::LoadFinished( igdeWObject&, bool ){
	asyncLoadFinished = true;
}



// Class igdeGDPCObjectClass
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDPCObjectClass::igdeGDPCObjectClass( igdeEnvironment &environment,
igdeGDClass *gdclass, const decPoint &size ) :
igdeGDPreviewCreator( environment, size ),
pGDClass( gdclass ),
pObject( NULL ),
pSky( NULL )
{
	if( ! gdclass ){
		DETHROW( deeInvalidParam );
	}
}

igdeGDPCObjectClass::~igdeGDPCObjectClass(){
	if( pSky ){
		delete pSky;
	}
}



// Management
///////////////

decString igdeGDPCObjectClass::DebugPrefix(){
	return decString( "GDPCObjectClass(" ) + ( pGDClass ? pGDClass->GetName() : decString( "?" ) ) + ")";
}

void igdeGDPCObjectClass::PrepareCanvasForRender(){
	igdeEnvironment &environment = GetEnvironment();
	deEngine &engine = *environment.GetEngineController()->GetEngine();
	
	// create world
	pWorld.TakeOver( engine.GetWorldManager()->CreateWorld() );
	pWorld->SetGravity( decVector( 0.0f, -9.81f, 0.0f ) );
	
	// create camera
	pCamera.TakeOver( engine.GetCameraManager()->CreateCamera() );
	pCamera->SetFov( 45.0f * DEG2RAD );
	pCamera->SetFovRatio( 1.0f );
	pCamera->SetImageDistance( 0.01f );
	pCamera->SetViewDistance( 500.0f );
	pCamera->SetExposure( 1.0f );
	pCamera->SetAdaptionTime( 0.0f );
	pWorld->AddCamera( pCamera );
	
	// create light
	
	// create sky
	pSky = new igdeWSky( environment );
	pSky->SetWorld( pWorld );
	pSky->SetGDDefaultSky();
	
	// adjust camera parameters to fit the sky lighting
	pCamera->SetLowestIntensity( pSky->GetMaxLightIntensity() );
	pCamera->SetHighestIntensity( pSky->GetMaxLightIntensity() );
	
	// create the object class wrapper
	pObject.TakeOver(new igdeWObject(environment));
	pObject->SetWorld( pWorld );
	pObject->SetAsyncLoadFinished( &pAsyncFinished );
	DebugLog( "set gdclass" );
	pObject->SetGDClass( pGDClass );
	
	// create render world canvas
	deCanvasView &container = *GetCanvas();
	pCanvasRenderWorld.TakeOver( engine.GetCanvasManager()->CreateCanvasRenderWorld() );
	pCanvasRenderWorld->SetCamera( pCamera );
	pCanvasRenderWorld->SetSize( container.GetSize() );
	container.AddCanvas( pCanvasRenderWorld );
}

bool igdeGDPCObjectClass::IsCanvasReadyForRender(){
	if( ! pAsyncFinished.asyncLoadFinished ){
		return false;
	}
	
	DebugLog( "finished asynchronous loading" );
	
	// update once
	const float initialUpdate = 0.1f;
	
	pObject->Update( initialUpdate );
	
	pWorld->ProcessPhysics( initialUpdate );
	pWorld->Update( initialUpdate );
	
	// center on object
	decVector minExtend, maxExtend;
	
	if( pObject->GetHasBoxExtends() ){
		minExtend = pObject->GetBoxMinExtend();
		maxExtend = pObject->GetBoxMaxExtend();
		
	}else{
		// TODO visit wrapper object sub-objects to find component and billboard boundaries
		//      if they are not scaling
		minExtend.Set( -0.5f, -0.5f, -0.5f );
		maxExtend.Set( 0.5f, 0.5f, 0.5f );
	}
	
	// TODO calculation of best distance to object can be improved by doing the following
	// 
	// offset = 0
	// for each vertex in component model lod0:
	//    offset = max( offset, camera_view_normalized dot vertex_position )
	// 
	// this way the camera distance starts at the closest vertex which results in the
	// largest appearance of the model in the image possible
	
	const decDVector center( ( minExtend + maxExtend ) * 0.5f );
	const decDVector size( maxExtend - minExtend );
	const double largestHalfSize = decMath::max( decMath::max( size.x, size.y ), 0.001 ) * 0.5;
	
	const decDVector viewRotation( 10.0, 10.0, 0.0 );
	const decDVector viewOffset( 0.0, 0.0, size.z * 0.5
		+ sin( fabs( viewRotation.x ) * DEG2RAD ) * largestHalfSize
		+ sin( fabs( viewRotation.y ) * DEG2RAD ) * largestHalfSize
		+ largestHalfSize * 1.05 / tan( ( double )pCamera->GetFov() * 0.5 ) );
	
	const decDMatrix matrix(
		decDMatrix::CreateRotationY( DEG2RAD * 180.0 )
		* decDMatrix::CreateTranslation( viewOffset )
		* decDMatrix::CreateRotation( viewRotation * DEG2RAD )
		* decDMatrix::CreateTranslation( center ) );
	
	pCamera->SetPosition( matrix.GetPosition() );
	pCamera->SetOrientation( matrix.ToQuaternion() );
	return true;
}

void igdeGDPCObjectClass::UpdateCanvasForRender( float elapsed ){
	pObject->Update( elapsed );
	
	pWorld->ProcessPhysics( elapsed );
	pWorld->Update( elapsed );
}
