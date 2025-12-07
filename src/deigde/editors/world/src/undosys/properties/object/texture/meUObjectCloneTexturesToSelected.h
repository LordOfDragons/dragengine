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

#ifndef _MEUOBJECTCLONETEXTURESTOSELECTED_H_
#define _MEUOBJECTCLONETEXTURESTOSELECTED_H_

#include <dragengine/common/collection/decObjectList.h>

#include <deigde/undo/igdeUndo.h>
#include "../../../../world/object/texture/meObjectTextureList.h"

class meObjectList;



/**
 * \brief Undo Action Clone Object Textures to all selected objects.
 */
class meUObjectCloneTexturesToSelected : public igdeUndo{
private:
	decObjectList pList;
	meObjectTextureList pTextureList;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meUObjectCloneTexturesToSelected> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo object. */
	meUObjectCloneTexturesToSelected(const meObjectList &list, const meObjectTextureList &textureList);
	
protected:
	/** \brief Clean up undo object. */
	virtual ~meUObjectCloneTexturesToSelected();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the list of objects to manipulate. */
	inline decObjectList &GetList(){return pList;}
	inline const decObjectList &GetList() const{return pList;}
	/** Retrieves the list of textures to apply. */
	inline meObjectTextureList &GetPropertyList(){return pTextureList;}
	inline const meObjectTextureList &GetPropertyList() const{return pTextureList;}
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
