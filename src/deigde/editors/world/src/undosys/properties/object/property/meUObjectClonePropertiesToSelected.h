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

#ifndef _MEUOBJECTCLONEPROPERTIESTOSELECTED_H_
#define _MEUOBJECTCLONEPROPERTIESTOSELECTED_H_

#include "meUndoDataObjectProperty.h"

#include <dragengine/common/string/decStringDictionary.h>

#include <deigde/undo/igdeUndo.h>



/**
 * \brief Undo Action Clone Object Properties to all selected objects.
 */
class meUObjectClonePropertiesToSelected : public igdeUndo{
public:
	typedef deTObjectReference<meUObjectClonePropertiesToSelected> Ref;
	
	
private:
	meUndoDataObjectProperty::List pList;
	decStringDictionary pProperties;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo object. */
	meUObjectClonePropertiesToSelected(const meObject::List &list, const decStringDictionary &properties);
	
protected:
	/** \brief Clean up undo object. */

protected:
	virtual ~meUObjectClonePropertiesToSelected();

public:
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the list of objects to manipulate. */
	inline meUndoDataObjectProperty::List &GetList(){ return pList; }
	inline const meUndoDataObjectProperty::List &GetList() const{ return pList; }
	/** Retrieves the list of properties to apply. */
	inline decStringDictionary &GetProperties(){ return pProperties; }
	inline const decStringDictionary &GetProperties() const{ return pProperties; }
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
