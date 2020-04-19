/* 
 * Drag[en]gine GUI Launcher
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

#include "deglGame.h"
#include "deglGameRunParams.h"
#include "patch/deglPatch.h"
#include "profile/deglGameProfile.h"

#include <dragengine/common/exceptions.h>



// Class deglGameRunParams
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglGameRunParams::deglGameRunParams() :
pGameProfile( NULL ),
pWidth( 0 ),
pHeight( 0 ),
pFullScreen( false ){
}

deglGameRunParams::~deglGameRunParams(){
	SetGameProfile( NULL );
}



// Management
///////////////

void deglGameRunParams::SetGameProfile( deglGameProfile *profile ){
	if( profile == pGameProfile ){
		return;
	}
	
	if( pGameProfile ){
		pGameProfile->FreeReference();
	}
	
	pGameProfile = profile;
	
	if( profile ){
		profile->AddReference();
	}
}

void deglGameRunParams::SetRunArguments( const char *arguments ){
	pRunArguments = arguments;
}

void deglGameRunParams::SetWidth( int width ){
	if( width < 1 ){
		DETHROW( deeInvalidParam );
	}
	pWidth = width;
}

void deglGameRunParams::SetHeight( int height ){
	if( height < 1 ){
		DETHROW( deeInvalidParam );
	}
	pHeight = height;
}

void deglGameRunParams::SetFullScreen( bool fullScreen ){
	pFullScreen = fullScreen;
}

bool deglGameRunParams::FindPatches( const deglGame &game, bool useLatestPatch,
const decUuid &useCustomPatch, decString &error ){
	pPatches.RemoveAll();
	
	// use latest patch
	if( useLatestPatch ){
		deglPatchList collected;
		game.FindPatches( collected );
		game.SortPatches( pPatches, collected );
		return true;
	}
	
	// use no patch at all
	if( ! useCustomPatch ){
		return true;
	}
	
	// use custom patch
	deglPatchList patches, collected;
	game.FindPatches( collected );
	game.SortPatches( patches, collected );
	
	deglPatch *usePatch = patches.GetWithID( useCustomPatch );
	if( ! usePatch ){
		error.Format( "No patch found with identifier '%s'", useCustomPatch.ToHexString( false ).GetString() );
		return false;
	}
	
	collected.RemoveAll();
	collected.Add( usePatch );
	
	int i;
	while( usePatch && usePatch->GetRequiredPatches().GetCount() > 0 ){
		const deglPatch &verifyPatch = *usePatch;
		
		for( i=0; i<verifyPatch.GetRequiredPatches().GetCount(); i++ ){
			deglPatch * const findPatch = patches.GetWithID( verifyPatch.GetRequiredPatches().GetAt( i ) );
			if( findPatch ){
				collected.Add( findPatch );
				usePatch = findPatch;
				break;
			}
		}
		
		if( i == verifyPatch.GetRequiredPatches().GetCount() ){
			decStringList names;
			decString name;
			for( i=0; i<verifyPatch.GetRequiredPatches().GetCount(); i++ ){
				name.Format( "'%s'", verifyPatch.GetRequiredPatches().GetAt( i ).ToHexString( false ).GetString() );
				names.Add( name );
			}
			name = names.Join( ", " );
			
			error.Format( "Required patches not found for patch with identifier '%s'."
				" Requires one of: %s", verifyPatch.GetIdentifier().ToHexString( false ).GetString(), name.GetString() );
			return false;
		}
	}
	
	game.SortPatches( pPatches, collected );
	return true;
}
