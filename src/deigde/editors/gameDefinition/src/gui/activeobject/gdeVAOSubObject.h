/* 
 * Drag[en]gine IGDE Game Definition Editor
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create active object component. */
	gdeVAOSubObject( gdeViewActiveObject &view, const gdeObjectClass &objectClass,
		const decString &propertyPrefix );
	
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
	virtual void GetExtends( decVector &minExtend, decVector &maxExtend ) const;
	
	/** Property value. */
	decString PropertyString( const decString &name, const decString &defaultValue ) const;
	int PropertyInt( const decString &name, int defaultValue ) const;
	float PropertyFloat( const decString &name, float defaultValue ) const;
	bool PropertyBool( const decString &name, bool defaultValue ) const;
	decColor PropertyColor( const decString &name, const decColor &defaultValue ) const;
	decVector PropertyVector( const decString &name, const decVector &defaultValue ) const;
	decVector2 PropertyVector2( const decString &name, const decVector2 &defaultValue ) const;
	decQuaternion PropertyQuaternion( const decString &name, const decVector &defaultValue ) const;
	void PropertyShapeList( const decString &name, decShapeList &list, const decShapeList &defaultValue ) const;
	/*@}*/
	
	
	
protected:
	const char *pPropertyValue( const decString &name ) const;
};

#endif
 
