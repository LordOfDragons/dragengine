/* 
 * Drag[en]gine DragonScript Script Module
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
#include <stdint.h>

#include "deDSEngineManager.h"
#include "deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>



// Class deDSEngineManager
////////////////////////////

// Constructor
////////////////

deDSEngineManager::deDSEngineManager( deScriptingDragonScript *ds ){
	if( ! ds ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
}

deDSEngineManager::~deDSEngineManager(){
}



// Management
///////////////

void deDSEngineManager::OutputMessage( const char *message ){
	pDS->LogInfo( message );
}

void deDSEngineManager::OutputWarning( const char *message, int warnID, dsScriptSource *script, int line, int position ){
	pDS->LogWarnFormat( "WARN#%i %s:%i(%i): %s", warnID, script->GetName(), line, position, message );
}

void deDSEngineManager::OutputWarningMore( const char *message ){
	pDS->LogWarnFormat( "   %s", message );
}

void deDSEngineManager::OutputError( const char *message, int errorID, dsScriptSource *script, int line, int position ){
	pDS->LogErrorFormat( "ERR#%i %s:%i(%i): %s", errorID, script->GetName(), line, position, message );
}

void deDSEngineManager::OutputErrorMore( const char *message ){
	pDS->LogErrorFormat( "   %s", message );
}

bool deDSEngineManager::ContinueParsing(){
	return true;
}
