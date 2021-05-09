/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLCOLLIDELISTPROPFIELD_H_
#define _DEOGLCOLLIDELISTPROPFIELD_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

class deoglRPropField;
class deoglRPropFieldType;
class deoglCollideListPropFieldType;
class deoglOcclusionTest;


/**
 * Collide List Prop Field.
 */
class deoglCollideListPropField{
private:
	deoglRPropField *pPropField;
	
	decPointerList pTypes;
	int pTypeCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create collide list prop field. */
	deoglCollideListPropField();
	
	/** Clean up collide list prop field. */
	~deoglCollideListPropField();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear collide list prop field. */
	void Clear();
	
	/** Start occlusion test. */
	void StartOcclusionTest( deoglOcclusionTest &occlusionTest, const decDVector &referencePosition );
	
	
	
	/** Prop field is empty. */
	bool GetIsEmpty() const;
	
	/** Prop field. */
	inline deoglRPropField *GetPropField() const{ return pPropField; }
	
	/** Set prop field. */
	void SetPropField( deoglRPropField *propField );
	
	/** Count of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	
	/** Type at the index. */
	deoglCollideListPropFieldType *GetTypeAt( int index ) const;
	
	/** Add type. */
	deoglCollideListPropFieldType *AddType( deoglRPropFieldType *type );
	
	/** Remove all types. */
	void RemoveAllTypes();
	/*@}*/
};

#endif
