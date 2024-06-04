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

#ifndef _MEUNDODATAOBJECTPROPERTY_H_
#define _MEUNDODATAOBJECTPROPERTY_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringDictionary.h>

class meObject;



/**
 * \brief Object Property Undo Data.
 */
class meUndoDataObjectProperty : public deObject{
private:
	meObject *pObject;
	decString pOldValue;
	bool pPropertyExists;
	decStringDictionary pOldProperties;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo data object. */
	meUndoDataObjectProperty( meObject *object );
	
protected:
	/** \brief Clean up undo data object. */
	virtual ~meUndoDataObjectProperty();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the object pointer. */
	inline meObject *GetObject() const{ return pObject; }
	/** Retrieves the old property value. */
	inline const decString &GetOldValue() const{ return pOldValue; }
	/** Sets the old property value. */
	void SetOldValue( const char *value );
	/** Determines if the property exists already before the redo operation. */
	inline bool GetPropertyExists() const{ return pPropertyExists; }
	/** Sets if the property exists already before the redo operation. */
	void SetPropertyExists( bool exists );
	/** Retrieves the old property list. */
	inline decStringDictionary &GetOldProperties(){ return pOldProperties; }
	inline const decStringDictionary &GetOldProperties() const{ return pOldProperties; }
	/*@}*/
};

#endif
