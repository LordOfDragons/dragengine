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

#ifndef _MEUOBJECTADDUSEDTEXTURES_H_
#define _MEUOBJECTADDUSEDTEXTURES_H_

#include <deigde/undo/igdeUndo.h>
#include "../../../../world/object/texture/meObjectTextureList.h"

#include <dragengine/common/string/decStringList.h>

#include "../../../../world/object/meObject.h"



/**
 * \brief Undo Action Object Add Used Textures.
 */
class meUObjectAddUsedTextures : public igdeUndo{
private:
	meObject::Ref pObject;
	decStringList pTextureNameList;
	meObjectTextureList pTextureList;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meUObjectAddUsedTextures> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo object. */
	meUObjectAddUsedTextures(meObject *object);
	
protected:
	/** \brief Clean up undo object. */
	virtual ~meUObjectAddUsedTextures();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the list of textures names. */
	inline decStringList &GetTextureNameList(){ return pTextureNameList; }
	inline const decStringList &GetTextureNameList() const{ return pTextureNameList; }
	/** Retrieves the list of added textures if existing already. */
	inline meObjectTextureList &GetTextureList(){ return pTextureList; }
	inline const meObjectTextureList &GetTextureList() const{ return pTextureList; }
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
