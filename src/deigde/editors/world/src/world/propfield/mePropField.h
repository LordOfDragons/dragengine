/* 
 * Drag[en]gine IGDE World Editor
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
#ifndef _MEPROPFIELD_H_
#define _MEPROPFIELD_H_

// includes
#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

// predefinitions
class mePropFieldType;
class meWorld;

class deEngine;
class dePropField;



/**
 * @brief Prop Field.
 *
 * Defines a prop field.
 */
class mePropField{
private:
	deEngine *pEngine;
	
	meWorld *pWorld;
	dePropField *pEngPF;
	
	decDVector pPosition;
	
	mePropFieldType **pTypes;
	int pTypeCount;
	int pTypeSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	mePropField( deEngine *engine );
	/** Cleans up the object. */
	~mePropField();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent world or NULL. */
	inline meWorld *GetWorld() const{ return pWorld; }
	/** Sets the parent world or NULL. */
	void SetWorld( meWorld *world );
	
	/** Retrieves the engine height terrain. */
	inline dePropField *GetEnginePropField() const{ return pEngPF; }
	
	/** Retrieves the position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition( const decDVector &position );
	
	/** Sets the world changed. */
	void SetWorldChanged();
	/*@}*/
	
	/** @name Types */
	/*@{*/
	/** Retrieves the number of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	/** Retrieves the type at the given index. */
	mePropFieldType *GetTypeAt( int index ) const;
	/** Retrieves the index of the given type or -1 if not found. */
	int IndexOfType( mePropFieldType *type ) const;
	/** Adds a new type. */
	void AddType( mePropFieldType *type );
	/** Removes a type. */
	void RemoveType( mePropFieldType *type );
	/** Removes all types. */
	void RemoveAllTypes();
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
