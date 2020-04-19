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

// include only once
#ifndef _DEOGLCOLLIDELISTPROPFIELD_H_
#define _DEOGLCOLLIDELISTPROPFIELD_H_

// includes

// predefinitions
class deoglRPropField;
class deoglCollideListPropFieldType;



/**
 * @brief Collide List Prop Field.
 *
 * A prop field in a collide list.
 */
class deoglCollideListPropField{
private:
	deoglRPropField *pPropField;
	
	deoglCollideListPropFieldType **pTypes;
	int pTypeCount;
	int pTypeSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collide list prop field. */
	deoglCollideListPropField();
	/** Cleans up the collide list prop field. */
	~deoglCollideListPropField();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Clears the collide list prop field. */
	void Clear();
	/** Determines if the prop field is empty. */
	bool GetIsEmpty() const;
	
	/** Retrieves the prop field. */
	inline deoglRPropField *GetPropField() const{ return pPropField; }
	/** Sets the prop field. */
	void SetPropField( deoglRPropField *propField );
	
	/** Retrieves the number of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	/** Retrieves the type at the given position. */
	deoglCollideListPropFieldType *GetTypeAt( int index ) const;
	/*@}*/
};

// end of include only once
#endif
