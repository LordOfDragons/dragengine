/* 
 * Drag[en]gine IGDE Project Editor
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "projProject.h"
#include "projProjectListener.h"
#include "profile/projProfile.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Project"



// Class projProject
//////////////////////////

// Constructor, destructor
////////////////////////////

projProject::projProject( igdeEnvironment *environment ) :
igdeEditableEntity( environment ),

pScriptDirectory( "/scripts" ),
pGameObject( "MyGameApp" ),
pPathConfig( "/config" ),
pPathCapture( "/capture" ),

pActiveProfile( NULL )
{
	try{
		decPath path;
		path.SetFromNative( environment->GetGameProject()->GetDirectoryPath() );
		path.AddComponent( "project.xml" );
		SetFilePath( path.GetPathNative() );
		SetSaved( true );
		SetChanged( false );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

projProject::~projProject(){
	pCleanUp();
}



// Management
///////////////

void projProject::SetScriptDirectory( const char *directory ){
	if( pScriptDirectory == directory ){
		return;
	}
	
	pScriptDirectory = directory;
	NotifyProjectChanged();
}

void projProject::SetGameObject( const char *gameObject ){
	if( pGameObject == gameObject ){
		return;
	}
	
	pGameObject = gameObject;
	NotifyProjectChanged();
}

void projProject::SetPathConfig( const char *path ){
	if( pPathConfig == path ){
		return;
	}
	
	pPathConfig = path;
	NotifyProjectChanged();
}

void projProject::SetPathCapture( const char *path ){
	if( pPathCapture == path ){
		return;
	}
	
	pPathCapture = path;
	NotifyProjectChanged();
}



// Profiles
/////////////

void projProject::AddProfile( projProfile *profile ){
	pProfiles.Add( profile );
	profile->SetProject( this );
	NotifyProfileStructureChanged();
	
	if( ! pActiveProfile ){
		SetActiveProfile( profile );
	}
}

void projProject::RemoveProfile( projProfile *profile ){
	if( ! profile || profile->GetProject() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( profile == pActiveProfile ){
		if( pProfiles.GetCount() == 1 ){
			SetActiveProfile( NULL );
			
		}else{
			if( pProfiles.GetAt( 0 ) == profile ){
				SetActiveProfile( pProfiles.GetAt( 1 ) );
				
			}else{
				SetActiveProfile( pProfiles.GetAt( 0 ) );
			}
		}
	}
	
	profile->SetProject( NULL );
	pProfiles.Remove( profile );
	NotifyProfileStructureChanged();
}

void projProject::RemoveAllProfiles(){
	const int count = pProfiles.GetCount();
	int i;
	
	SetActiveProfile( NULL );
	
	for( i=0; i<count; i++ ){
		pProfiles.GetAt( i )->SetProject( NULL );
	}
	pProfiles.RemoveAll();
	NotifyProfileStructureChanged();
}



void projProject::SetActiveProfile( projProfile *profile ){
	if( profile == pActiveProfile ){
		return;
	}
	
	if( pActiveProfile ){
		pActiveProfile->FreeReference();
	}
	
	pActiveProfile = profile;
	
	if( profile ){
		profile->AddReference();
	}
	
	NotifyActiveProfileChanged();
}



void projProject::NotifyProfileStructureChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( projProjectListener* )pListeners.GetAt( i ) )->ProfileStructureChanged( this );
	}
	
	SetChanged( true );
}

void projProject::NotifyProfileChanged( projProfile *profile ){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( projProjectListener* )pListeners.GetAt( i ) )->ProfileChanged( this, profile );
	}
	
	SetChanged( true );
}

void projProject::NotifyProfileNameChanged( projProfile *profile ){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( projProjectListener* )pListeners.GetAt( i ) )->ProfileNameChanged( this, profile );
	}
	
	SetChanged( true );
}

void projProject::NotifyActiveProfileChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( projProjectListener* )pListeners.GetAt( i ) )->ActiveProfileChanged( this );
	}
}



// Test running
/////////////////

void projProject::SetActiveLaunchProfile( const char *profile ){
	if( profile == pActiveLaunchProfile ){
		return;
	}
	
	pActiveLaunchProfile = profile;
	
	NotifyActiveLaunchProfileChanged();
}



void projProject::NotifyActiveLaunchProfileChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( projProjectListener* )pListeners.GetAt( i ) )->ActiveLaunchProfileChanged( this );
	}
	
	SetChanged( true );
}



// Listeners
//////////////

void projProject::AddListener( projProjectListener *listener ){
	pListeners.Add( listener );
}

void projProject::RemoveListener( projProjectListener *listener ){
	pListeners.Remove( listener );
}



void projProject::NotifyStateChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( projProjectListener* )pListeners.GetAt( i ) )->StateChanged( this );
	}
}

void projProject::NotifyUndoChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( projProjectListener* )pListeners.GetAt( i ) )->UndoChanged( this );
	}
}

void projProject::NotifyProjectChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( projProjectListener* )pListeners.GetAt( i ) )->ProjectChanged( this );
	}
	
	SetChanged( true );
}



// Private Functions
//////////////////////

void projProject::pCleanUp(){
	pListeners.RemoveAll();
	
	GetUndoSystem()->RemoveAll();
	
	RemoveAllProfiles();
}
