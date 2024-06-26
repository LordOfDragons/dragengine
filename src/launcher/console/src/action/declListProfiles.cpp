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

#include "declListProfiles.h"
#include "../declLauncher.h"
#include "../config/declConfiguration.h"

#include <delauncher/engine/delEngine.h>
#include <delauncher/game/delGame.h>
#include <delauncher/game/delGameManager.h>
#include <delauncher/game/patch/delPatchManager.h>
#include <delauncher/game/profile/delGameProfile.h>

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class declListProfiles
///////////////////////////

// Constructor, destructor
////////////////////////////

declListProfiles::declListProfiles( declLauncher &launcher ) :
pLauncher( launcher ){
}

declListProfiles::~declListProfiles(){
}



// Management
///////////////

void declListProfiles::PrintSyntax(){
	printf( "Drag[en]gine Console Launcher.\n" );
	printf( "Written by Plüss Roland ( roland@rptd.ch ).\n" );
	printf( "Released under the GPL ( http://www.gnu.org/licenses/gpl.html ), 2011.\n" );
	printf( "\n" );
	printf( "Lists game profiles.\n" );
	printf( "\n" );
	printf( "Syntax:\n" );
	printf( "delauncherconsole profiles\n" );
	printf( "\n" );
}



void declListProfiles::ParseArguments(){
}

void declListProfiles::Run(){
	ParseArguments();
	
	pLauncher.Prepare();
	
	const delGameProfileList &profiles = pLauncher.GetGameManager().GetProfiles();
	int i, count;
	
	printf( "Available Game Profiles:\n" );
	count = profiles.GetCount();
	for( i=0; i<count; i++ ){
		const delGameProfile &profile = *profiles.GetAt( i );
		printf( "- '%s'\n", profile.GetName().GetString() );
	}
}
