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

delGameRunParams::delGameRunParams(const delGameRunParams &params) :
pGameProfile(params.pGameProfile),
pRunArguments(params.pRunArguments),
pWidth(params.pWidth),
pHeight(params.pHeight),
pFullScreen(params.pFullScreen),
pPatches(params.pPatches){
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
		error.Format( "No patch found with identifier '%s'",
			useCustomPatch.ToHexString( false ).GetString() );
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
