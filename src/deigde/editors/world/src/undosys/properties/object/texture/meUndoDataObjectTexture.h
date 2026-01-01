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
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>

#include "../../../../world/object/texture/meObjectTexture.h"

#include "../../../../world/object/meObject.h"



/**
 * \brief Object Property Undo Data.
 */
class meUndoDataObjectTexture : public deObject{
private:
	meObject::Ref pObject;
	meObjectTexture::Ref pOldTexture;
	meObjectTexture::Ref pNewTexture;
	meObjectTexture::List pOldTextureList;
	meObjectTexture::List pNewTextureList;
	
public:
	typedef deTObjectReference<meUndoDataObjectTexture> Ref;
	typedef decTObjectOrderedSet<meUndoDataObjectTexture> List;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo data object. */
	meUndoDataObjectTexture(meObject *object);
	
protected:
	/** \brief Clean up undo data object. */

protected:
	virtual ~meUndoDataObjectTexture();

public:
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the object pointer. */
	inline const meObject::Ref &GetObject() const{ return pObject; }
	/** Retrieves the old texture object or nullptr if not existing. */
	inline const meObjectTexture::Ref &GetOldTexture() const{ return pOldTexture; }
	/** Sets the old texture object or nullptr if not existing. */
	void SetOldTexture(meObjectTexture *texture);
	/** Retrieves the new texture object or nullptr if not existing. */
	inline const meObjectTexture::Ref &GetNewTexture() const{ return pNewTexture; }
	/** Sets the new texture object or nullptr if not existing. */
	void SetNewTexture(meObjectTexture *texture);
	/** Retrieves the old texture list if used. */
	inline meObjectTexture::List &GetOldTextureList(){ return pOldTextureList; }
	inline const meObjectTexture::List &GetOldTextureList() const{ return pOldTextureList; }
	/** Retrieves the new texture list if used. */
	inline meObjectTexture::List &GetNewTextureList(){ return pNewTextureList; }
	inline const meObjectTexture::List &GetNewTextureList() const{ return pNewTextureList; }
	/*@}*/
};

#endif
