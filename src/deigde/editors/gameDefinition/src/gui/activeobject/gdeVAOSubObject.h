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

#ifndef _GDEVAOSUBOBJECT_H_
#define _GDEVAOSUBOBJECT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class gdeViewActiveObject;
class gdeObjectClass;
class decShapeList;


/**
 * Active object view sub object.
 */
class gdeVAOSubObject : public deObject{
protected:
	gdeViewActiveObject &pView;
	
	const gdeObjectClass &pObjectClass;
	const decString pPropertyPrefix;
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeVAOSubObject> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create active object component. */
	gdeVAOSubObject(gdeViewActiveObject &view, const gdeObjectClass &objectClass,
		const decString &propertyPrefix);
	
protected:
	/**
	 * Clean up active object component.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~gdeVAOSubObject();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Rebuild resources. */
	virtual void RebuildResources();
	
	/** Selected object changed. */
	virtual void SelectedObjectChanged();
	
	/** Extends. */
	virtual void GetExtends(decVector &minExtend, decVector &maxExtend) const;
	
	/** Property value. */
	decString PropertyString(const decString &name, const decString &defaultValue) const;
	int PropertyInt(const decString &name, int defaultValue) const;
	float PropertyFloat(const decString &name, float defaultValue) const;
	bool PropertyBool(const decString &name, bool defaultValue) const;
	decColor PropertyColor(const decString &name, const decColor &defaultValue) const;
	decVector PropertyVector(const decString &name, const decVector &defaultValue) const;
	decVector2 PropertyVector2(const decString &name, const decVector2 &defaultValue) const;
	decQuaternion PropertyQuaternion(const decString &name, const decVector &defaultValue) const;
	void PropertyShapeList(const decString &name, decShapeList &list, const decShapeList &defaultValue) const;
	/*@}*/
	
	
	
protected:
	const char *pPropertyValue(const decString &name) const;
};

#endif
 
