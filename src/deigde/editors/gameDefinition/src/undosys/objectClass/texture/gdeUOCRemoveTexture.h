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


#ifndef _GDEUOCREMOVETEXTURE_H_
#define _GDEUOCREMOVETEXTURE_H_

#include <deigde/undo/igdeUndo.h>

#include "../../../gamedef/objectClass/component/gdeOCComponentTexture.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"



/**
 * Undo action object class remove texture.
 */
class gdeUOCRemoveTexture : public igdeUndo{
public:
	typedef deTObjectReference<gdeUOCRemoveTexture> Ref;
	
	
private:
	gdeObjectClass::Ref pObjectClass;
	gdeOCComponentTexture::Ref pTexture;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo action. */
	gdeUOCRemoveTexture(gdeObjectClass *objectClass, gdeOCComponentTexture *texture);
	
protected:
	/** Clean up undo action. */
	virtual ~gdeUOCRemoveTexture();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Undo. */
	virtual void Undo();
	
	/** Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
