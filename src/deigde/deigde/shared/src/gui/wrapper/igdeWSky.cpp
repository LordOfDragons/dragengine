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

#include "igdeWSky.h"
#include "../../engine/igdeEngineController.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gamedefinition/igdeGameDefinition.h"
#include "../../gamedefinition/sky/igdeGDSky.h"
#include "../../gamedefinition/sky/igdeGDSkyController.h"
#include "../../gamedefinition/sky/igdeGDSkyManager.h"
#include "../../gameproject/igdeGameProject.h"
#include "../../loadsave/igdeLoadSky.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyManager.h>
#include <dragengine/resources/sky/deSkyController.h>
#include <dragengine/resources/sky/deSkyReference.h>
#include <dragengine/resources/sky/deSkyLayer.h>
#include <dragengine/resources/sky/deSkyInstance.h>
#include <dragengine/resources/sky/deSkyInstanceManager.h>
#include <dragengine/resources/world/deWorld.h>



// Asynchronous load finished listener
////////////////////////////////////////

igdeWSky::cAsyncLoadFinished::cAsyncLoadFinished(){
}

igdeWSky::cAsyncLoadFinished::~cAsyncLoadFinished(){
}



// Class igdeWSky
///////////////////

// Constructor, destructor
////////////////////////////

igdeWSky::igdeWSky( igdeEnvironment &environment ) :
pEnvironment( environment ),
pAsyncLoadFinished( NULL ),
pAsyncLoadCounter( 0 )
{
	pEngSkyInstance.TakeOver( environment.GetEngineController()->GetEngine()
		->GetSkyInstanceManager()->CreateSkyInstance() );
}

igdeWSky::~igdeWSky(){
	SetWorld( NULL );
}



// Management
///////////////

void igdeWSky::SetWorld( deWorld *world ){
	if( world == pEngWorld ){
		return;
	}
	
	if( pEngWorld && pEngSkyInstance ){
		pEngWorld->RemoveSky( pEngSkyInstance );
	}
	
	pEngWorld = world;
	
	if( world ){
		world->AddSky( pEngSkyInstance );
	}
}

int igdeWSky::GetControllerCount() const{
	return pEngSkyInstance->GetControllerCount();
}

const deSkyController &igdeWSky::GetControllerAt( int index ) const{
	return pEngSkyInstance->GetControllerAt( index );
}

void igdeWSky::SetControllerValue( int index, float value ){
	pEngSkyInstance->GetControllerAt( index ).SetCurrentValue( value );
	pEngSkyInstance->NotifyControllerChangedAt( index );
}

const deSky *igdeWSky::GetSky() const{
	return pEngSkyInstance->GetSky();
}

void igdeWSky::SetSky( deSky *sky ){
	if( pEngSkyInstance->GetSky() == sky ){
		return;
	}
	
	pEngSkyInstance->SetSky( sky );
	pGDSky = NULL;
	pPath.Empty();
	
	pMaxLightIntensity = 0.0f;
	if( sky ){
		const int countLayers = sky->GetLayerCount();
		int i;
		
		for( i=0; i<countLayers; i++ ){
			const deSkyLayer &layer = sky->GetLayerAt( i );
			pMaxLightIntensity += layer.GetLightIntensity() + layer.GetAmbientIntensity();
		}
	}
}

void igdeWSky::SetGDDefaultSky(){
	SetPath( pEnvironment.GetGameProject()->GetGameDefinition()->GetSkyManager()->GetDefaultPath() );
}

void igdeWSky::SetGDSky( igdeGDSky *gdsky ){
	if( pGDSky == gdsky ){
		return;
	}
	
	if( gdsky ){
		pLoadSky( gdsky->GetPath() );
		
	}else{
		SetSky( NULL );
	}
	
	// SetSky sets pGDSky to NULL if sky is different from the sky in the sky instance.
	// pLoadSky calls SetSky so the same problem applies to both code path. By setting
	// the sky after these calls are done it is ensured pGDSky is not suddenly NULL
	// although it should not be NULL
	pGDSky = gdsky;
}

void igdeWSky::SetPath( const char *path ){
	igdeGDSky *gdsky = pEnvironment.GetGameProject()->GetGameDefinition()
		->GetSkyManager()->GetSkyList().GetWithPath( path );
	
	if( ! gdsky ){
		gdsky = pEnvironment.GetGameProject()->GetGameDefinition()
			->GetSkyManager()->GetSkyList().GetWithPath( path );
	}
	
	if( gdsky ){
		SetGDSky( gdsky );
		
	}else{
		pLoadSky( path );
	}
}

void igdeWSky::SetAsyncLoadFinished( cAsyncLoadFinished *listener ){
	pAsyncLoadFinished = listener;
}



void igdeWSky::OnGameDefinitionChanged(){
	if( ! pGDSky ){
		return; // custom sky or loaded from file
	}
	
	decStringList names;
	int i;
	if( pEngSkyInstance ){
		const int count = pEngSkyInstance->GetControllerCount();
		for( i=0; i<count; i++ ){
			names.Add( pEngSkyInstance->GetControllerAt( i ).GetName() );
		}
	}
	
	float *values = NULL;
	if( pEngSkyInstance ){
		const int count = pEngSkyInstance->GetControllerCount();
		if( count > 0 ){
			values = new float[ count ];
			for( i=0; i<count; i++ ){
				values[ i ] = pEngSkyInstance->GetControllerAt( i ).GetCurrentValue();
			}
		}
	}
	
	try{
		SetPath( pGDSky->GetPath() );
		
		if( pEngSkyInstance ){
			const int count = names.GetCount();
			for( i=0; i<count; i++ ){
				const int index = names.IndexOf( names.GetAt( i ) );
				if( index != -1 ){
					pEngSkyInstance->GetControllerAt( index ).SetCurrentValue( values[ i ] );
					pEngSkyInstance->NotifyControllerChangedAt( index );
				}
			}
		}
		
		if( values ){
			delete [] values;
		}
		
	}catch( const deException &e ){
		if( values ){
			delete [] values;
		}
		throw;
	}
}



// Private Functions
/////////////////////

void igdeWSky::pLoadSky( const char *path ){
	deEngine &engine = *pEnvironment.GetEngineController()->GetEngine();
	
	pAsyncLoadCounter = 0;
	
	const decPath vfsPath( decPath::CreatePathUnix( path ) );
	if( ! engine.GetVirtualFileSystem()->ExistsFile( vfsPath ) ){
		pCheckAsyncLoadFinished();
		return;
	}
	
	igdeLoadSky loadsky( pEnvironment, pEnvironment.GetLogger(), "igdeWSky" );
	decBaseFileReaderReference reader;
	deSkyReference sky;
	
	try{
		sky.TakeOver( engine.GetSkyManager()->CreateSky() );
		
		reader.TakeOver( engine.GetVirtualFileSystem()->OpenFileForReading( vfsPath ) );
		loadsky.Load( path, sky, reader );
		
		SetSky( sky );
		
	}catch( const deException &e ){
		pCheckAsyncLoadFinished();
		throw;
	}
	
	pPath = path;
	pCheckAsyncLoadFinished();
}

void igdeWSky::pCheckAsyncLoadFinished(){
	if( pAsyncLoadCounter != 0 ){
		return;
	}
	
	if( pAsyncLoadFinished ){
		pAsyncLoadFinished->LoadFinished( *this, true );
	}
}
