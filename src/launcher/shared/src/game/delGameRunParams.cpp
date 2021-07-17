/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "delGame.h"
#include "delGameRunParams.h"
#include "patch/delPatch.h"

#include <dragengine/common/exceptions.h>



// Class delGameRunParams
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGameRunParams::delGameRunParams() :
pWidth( 0 ),
pHeight( 0 ),
pFullScreen( false ){
}

delGameRunParams::~delGameRunParams(){
}



// Management
///////////////

void delGameRunParams::SetGameProfile( delGameProfile *profile ){
	pGameProfile = profile;
}

void delGameRunParams::SetRunArguments( const char *arguments ){
	pRunArguments = arguments;
}

void delGameRunParams::SetWidth( int width ){
	if( width < 1 ){
		DETHROW_INFO( deeInvalidParam, "width < 1" );
	}
	pWidth = width;
}

void delGameRunParams::SetHeight( int height ){
	if( height < 1 ){
		DETHROW_INFO( deeInvalidParam, "height < 1" );
	}
	pHeight = height;
}

void delGameRunParams::SetFullScreen( bool fullScreen ){
	pFullScreen = fullScreen;
}

bool delGameRunParams::FindPatches( const delGame &game, bool useLatestPatch,
const decUuid &useCustomPatch, decString &error ){
	pPatches.RemoveAll();
	
	// use latest patch
	if( useLatestPatch ){
		delPatchList collected;
		game.FindPatches( collected );
		game.SortPatches( pPatches, collected );
		return true;
	}
	
	// use no patch at all
	if( ! useCustomPatch ){
		return true;
	}
	
	// use custom patch
	delPatchList patches, collected;
	game.FindPatches( collected );
	game.SortPatches( patches, collected );
	
	delPatch *usePatch = patches.GetWithID( useCustomPatch );
	if( ! usePatch ){
		error.Format( "No patch found with identifier '%s'", useCustomPatch.ToHexString( false ).GetString() );
		return false;
	}
	
	collected.RemoveAll();
	collected.Add( usePatch );
	
	int i;
	while( usePatch && usePatch->GetRequiredPatches().GetCount() > 0 ){
		const delPatch &verifyPatch = *usePatch;
		
		for( i=0; i<verifyPatch.GetRequiredPatches().GetCount(); i++ ){
			delPatch * const findPatch = patches.GetWithID( verifyPatch.GetRequiredPatches().GetAt( i ) );
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
