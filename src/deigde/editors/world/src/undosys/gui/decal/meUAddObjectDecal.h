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

#ifndef _MEUADDOBJECTDECAL_H_
#define _MEUADDOBJECTDECAL_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class meWorld;
#include "../../../world/decal/meDecal.h"
#include "../../../world/object/meObject.h"



/**
 * Adds a new decal to the an object.
 */
class meUAddObjectDecal : public igdeUndo{
public:
	typedef deTObjectReference<meUAddObjectDecal> Ref;
	
	
private:
	meWorld *pWorld;
	meDecal::Ref pDecal;
	meObject::Ref pParentObject;
	
public:
	// constructor, destructor
	meUAddObjectDecal(meWorld *world, meObject *parentObject);
	
protected:
	~meUAddObjectDecal();
	
public:
	// Management
	/** Retrieves the decal. */
	inline const meDecal::Ref &GetDecal() const{ return pDecal; }
	/** Retrieves the parent object or nullptr. */
	inline const meObject::Ref &GetParentObject() const{ return pParentObject; }
	
	// undo and redo operations
	virtual void Undo();
	virtual void Redo();
	
	// internal
	void UpdateInfos();
	
private:
	void pCleanUp();
};

// end of include only once
#endif
