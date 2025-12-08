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

// include only once
#ifndef _MEUSETOBJECTCLASS_H_
#define _MEUSETOBJECTCLASS_H_

// includes
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>
#include "meObject.h"

// predefinitions
class meObjectList;



/**
 * Undo action for setting object class.
 */
class meUSetObjectClass : public igdeUndo{
private:
	struct sObject{
		meObject::Ref object;
		decString oldcname;
		decString newcname;
		decVector oldsize;
		decVector oldscaling;
	};
	
private:
	sObject *pObjects;
	int pObjectCount;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meUSetObjectClass> Ref;
	
	// constructor, destructor
	meUSetObjectClass(meObject *object, const char *newcname);
	meUSetObjectClass(meObjectList &objects, const char *newcname);
	
protected:
	~meUSetObjectClass();
	
public:
	// management
	void Undo();
	void Redo();
	
private:
	void pCleanUp();
};

// end of include only once
#endif
