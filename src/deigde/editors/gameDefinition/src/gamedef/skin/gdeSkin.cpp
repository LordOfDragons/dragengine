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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdeSkin.h"
#include "../gdeGameDefinition.h"

#include <dragengine/common/exceptions.h>



// Class gdeSkin
//////////////////

// Constructor, destructor
////////////////////////////

gdeSkin::gdeSkin() :
pGameDefinition( NULL ){
}

gdeSkin::gdeSkin( const char *path, const char *name ) :
pGameDefinition( NULL ),
pPath( path ),
pName( name ){
}

gdeSkin::gdeSkin( const gdeSkin &skin ) :
pGameDefinition( NULL ),

pPath( skin.pPath ),
pName( skin.pName ),
pDescription( skin.pDescription ),
pCategory( skin.pCategory ),
pTags( skin.pTags ){
}

gdeSkin::~gdeSkin(){
	pCleanUp();
}



// Management
///////////////

void gdeSkin::SetGameDefinition( gdeGameDefinition *gamedef ){
	pGameDefinition = gamedef;
}



void gdeSkin::SetPath( const char *path ){
	if( pPath == path ){
		return;
	}
	
	pPath = path;
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkinChanged( this );
	}
}

void gdeSkin::SetName( const char *name ){
	if( pName == name ){
		return;
	}
	
	pName = name;
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkinNameChanged( this );
	}
}

void gdeSkin::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkinChanged( this );
	}
}

void gdeSkin::SetCategory( const char *category ){
	if( pCategory == category ){
		return;
	}
	
	pCategory = category;
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkinChanged( this );
	}
}

void gdeSkin::SetTags( const decStringSet &tags ){
	if( pTags == tags ){
		return;
	}
	
	pTags = tags;
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkinChanged( this );
	}
}



// Private functions
//////////////////////

void gdeSkin::pCleanUp(){
	SetGameDefinition( NULL );
}
