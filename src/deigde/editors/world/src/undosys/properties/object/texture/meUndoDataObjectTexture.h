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

#ifndef _MEUNDODATAOBJECTTEXTURE_H_
#define _MEUNDODATAOBJECTTEXTURE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

#include "../../../../world/object/texture/meObjectTextureList.h"

class meObject;



/**
 * \brief Object Property Undo Data.
 */
class meUndoDataObjectTexture : public deObject{
private:
	meObject *pObject;
	meObjectTexture *pOldTexture;
	meObjectTexture *pNewTexture;
	meObjectTextureList pOldTextureList;
	meObjectTextureList pNewTextureList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo data object. */
	meUndoDataObjectTexture( meObject *object );
	
protected:
	/** \brief Clean up undo data object. */
	virtual ~meUndoDataObjectTexture();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the object pointer. */
	inline meObject *GetObject() const{ return pObject; }
	/** Retrieves the old texture object or NULL if not existing. */
	inline meObjectTexture *GetOldTexture() const{ return pOldTexture; }
	/** Sets the old texture object or NULL if not existing. */
	void SetOldTexture( meObjectTexture *texture );
	/** Retrieves the new texture object or NULL if not existing. */
	inline meObjectTexture *GetNewTexture() const{ return pNewTexture; }
	/** Sets the new texture object or NULL if not existing. */
	void SetNewTexture( meObjectTexture *texture );
	/** Retrieves the old texture list if used. */
	inline meObjectTextureList &GetOldTextureList(){ return pOldTextureList; }
	inline const meObjectTextureList &GetOldTextureList() const{ return pOldTextureList; }
	/** Retrieves the new texture list if used. */
	inline meObjectTextureList &GetNewTextureList(){ return pNewTextureList; }
	inline const meObjectTextureList &GetNewTextureList() const{ return pNewTextureList; }
	/*@}*/
};

#endif
