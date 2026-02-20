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

#ifndef _MEUSETOBJECTCLASS_H_
#define _MEUSETOBJECTCLASS_H_

#include "../../../world/object/meObject.h"

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>



/**
 * Undo action for setting object class.
 */
class meUSetObjectClass : public igdeUndo{
public:
	using Ref = deTObjectReference<meUSetObjectClass>;
	
	
private:
	class cObject : public deObject{
	public:
		using Ref = deTObjectReference<cObject>;
		using List = decTObjectOrderedSet<cObject>;
		
		meObject::Ref object;
		decString oldcname, newcname;
		decVector oldsize, oldscaling;
		
		cObject() = default;
	protected:
		~cObject() override = default;
	};
	
private:
	cObject::List pObjects;
	
public:
	// constructor, destructor
	meUSetObjectClass(meObject *object, const char *newcname);
	meUSetObjectClass(meObject::List &objects, const char *newcname);
	
protected:
	~meUSetObjectClass() override;
	
public:
	// management
	void Undo() override;
	void Redo() override;
};

#endif
