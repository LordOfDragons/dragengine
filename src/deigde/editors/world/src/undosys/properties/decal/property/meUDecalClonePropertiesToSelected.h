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

#ifndef _MEUDECALCLONEPROPERTIESTOSELECTED_H_
#define _MEUDECALCLONEPROPERTIESTOSELECTED_H_

#include "meUndoDataDecalProperty.h"

#include <dragengine/common/string/decStringDictionary.h>

#include <deigde/undo/igdeUndo.h>


/**
 * \brief Undo action decal clone properties to all selected decals.
 */
class meUDecalClonePropertiesToSelected : public igdeUndo{
public:
	typedef deTObjectReference<meUDecalClonePropertiesToSelected> Ref;
	
	
private:
	meUndoDataDecalProperty::List pList;
	decStringDictionary pPropertyList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo decal. */
	meUDecalClonePropertiesToSelected(const meDecal::List &list, const decStringDictionary &propertyList);
	
protected:
	/** \brief Clean up undo decal. */

protected:
	virtual ~meUDecalClonePropertiesToSelected();

public:
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief List of decals to manipulate. */
	inline meUndoDataDecalProperty::List &GetList(){ return pList; }
	inline const meUndoDataDecalProperty::List &GetList() const{ return pList; }
	
	/** \brief List of properties to apply. */
	inline decStringDictionary &GetPropertyList(){ return pPropertyList; }
	inline const decStringDictionary &GetPropertyList() const{ return pPropertyList; }
	
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
