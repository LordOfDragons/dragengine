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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "projLoadSaveSystem.h"
#include "../gui/projWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/filedialog/igdeFilePatternList.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/filesystem/dePatternList.h>



// Definitions
////////////////

#define LOGSOURCE "Project"



// Class projLoadSaveSystem
/////////////////////////////

// Constructor, destructor
////////////////////////////

projLoadSaveSystem::projLoadSaveSystem( projWindowMain &windowMain ) :
pWindowMain( windowMain )
{
	pBuildFilePattern();
}

projLoadSaveSystem::~projLoadSaveSystem(){
}



// Management
///////////////



// Private Functions
//////////////////////

void projLoadSaveSystem::pBuildFilePattern(){
	igdeFilePattern *filePattern = NULL;
	
	try{
		filePattern = new igdeFilePattern( "DELGA", "*.delga", ".delga" );
		pFPDelga.AddFilePattern( filePattern );
		
	}catch( const deException & ){
		if( filePattern ){
			delete filePattern;
		}
		throw;
	}
}
