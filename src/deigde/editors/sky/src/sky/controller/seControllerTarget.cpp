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

#include "seControllerTarget.h"
#include "../seSky.h"
#include "../link/seLink.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sky/deSkyControllerTarget.h>



// Class seControllerTarget
/////////////////////////////

// Constructor, destructor
////////////////////////////

seControllerTarget::seControllerTarget(){
}

seControllerTarget::seControllerTarget( const seControllerTarget &copy ) :
pLinks( copy.pLinks ){
}

seControllerTarget::~seControllerTarget(){
	RemoveAllLinks();
}



// Management
///////////////

void seControllerTarget::AddLink( seLink *link ){
	if( ! link ){
		DETHROW( deeInvalidParam );
	}
	pLinks.Add( link );
}

void seControllerTarget::RemoveLink( seLink *link ){
	pLinks.Remove( link );
}

void seControllerTarget::RemoveAllLinks(){
	pLinks.RemoveAll();
}



// Operators
//////////////

seControllerTarget &seControllerTarget::operator=( const seControllerTarget &copy ){
	pLinks = copy.pLinks;
	return *this;
}
