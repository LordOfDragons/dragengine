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
#ifndef _MEUOBJECTADDTEXTURE_H_
#define _MEUOBJECTADDTEXTURE_H_

// includes
#include <deigde/undo/igdeUndo.h>

// predefinitions
class meWorld;
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/texture/meObjectTexture.h"



/**
 * Undo action for adding a texture to an object.
 */
class meUObjectAddTexture : public igdeUndo{
public:
	typedef deTObjectReference<meUObjectAddTexture> Ref;
	
	
public:
	
private:
	meObject::Ref pObject;
	meObjectTexture::Ref pTexture;
	
public:
	// constructor, destructor
	meUObjectAddTexture(meObject *object, meObjectTexture *texture);
	
protected:
	virtual ~meUObjectAddTexture();
	
public:
	// management
	void Undo();
	void Redo();
};

// end of include only once
#endif
