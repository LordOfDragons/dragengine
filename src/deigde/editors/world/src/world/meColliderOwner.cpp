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

#include "meColliderOwner.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Class meColliderOwner
//////////////////////////

// Constructor, destructor
////////////////////////////

meColliderOwner::meColliderOwner(meObject *object) :
pObject(object),
pShape(NULL),
pSnapPoint(NULL),
pDecal(NULL),
pNavigationSpace(NULL)
{
	if(! object){
		DETHROW(deeInvalidParam);
	}
}

meColliderOwner::meColliderOwner(meObjectShape *shape) :
pObject(NULL),
pShape(shape),
pSnapPoint(NULL),
pDecal(NULL),
pNavigationSpace(NULL)
{
	if(! shape){
		DETHROW(deeInvalidParam);
	}
}

meColliderOwner::meColliderOwner(meObjectSnapPoint *snapPoint) :
pObject(NULL),
pShape(NULL),
pSnapPoint(snapPoint),
pDecal(NULL),
pNavigationSpace(NULL)
{
	if(! snapPoint){
		DETHROW(deeInvalidParam);
	}
}

meColliderOwner::meColliderOwner(meDecal *decal) :
pObject(NULL),
pShape(NULL),
pSnapPoint(NULL),
pDecal(decal),
pNavigationSpace(NULL)
{
	if(! decal){
		DETHROW(deeInvalidParam);
	}
}

meColliderOwner::meColliderOwner(meNavigationSpace *navspace) :
pObject(NULL),
pShape(NULL),
pSnapPoint(NULL),
pDecal(NULL),
pNavigationSpace(navspace)
{
	if(! navspace){
		DETHROW(deeInvalidParam);
	}
}



// Management
///////////////

const meColliderOwner *meColliderOwner::GetColliderOwner(
igdeEnvironment &environment, deCollider *collider){
	if(! collider){
		return NULL;
	}
	
	return (const meColliderOwner*)environment.GetColliderUserPointer(collider);
}
