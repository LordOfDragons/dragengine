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
