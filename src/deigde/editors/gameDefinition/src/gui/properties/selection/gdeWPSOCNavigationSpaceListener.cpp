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

#include "gdeWPSOCNavigationSpace.h"
#include "gdeWPSOCNavigationSpaceListener.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/navspace/gdeOCNavigationSpace.h"

#include <dragengine/common/exceptions.h>



// Class gdeWPSOCNavigationSpaceListener
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCNavigationSpaceListener::gdeWPSOCNavigationSpaceListener( gdeWPSOCNavigationSpace &panel ) :
pPanel( panel ){
}

gdeWPSOCNavigationSpaceListener::~gdeWPSOCNavigationSpaceListener(){
}



// Management
///////////////

void gdeWPSOCNavigationSpaceListener::OCPropertyNameChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeProperty* ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCNavigationSpaceListener::OCPropertiesChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCNavigationSpaceListener::ActiveObjectClassChanged( gdeGameDefinition* ){
	pPanel.UpdatePropertyList();
}



void gdeWPSOCNavigationSpaceListener::OCNavigationSpacesChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdateNavigationSpace();
}

void gdeWPSOCNavigationSpaceListener::OCNavigationSpaceChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCNavigationSpace *navspace ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	if( pPanel.GetNavigationSpace() != navspace ){
		return;
	}
	pPanel.UpdateNavigationSpace();
}

void gdeWPSOCNavigationSpaceListener::ActiveOCNavigationSpaceChanged( gdeGameDefinition* ){
	pPanel.UpdateNavigationSpace();
}
