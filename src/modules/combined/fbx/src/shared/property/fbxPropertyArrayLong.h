/* 
 * FBX Modules
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

#ifndef _FBXPROPERTYARRAYLONG_H_
#define _FBXPROPERTYARRAYLONG_H_


#include <stdint.h>

#include "../fbxProperty.h"


/**
 * \brief FBX property array integer.
 */
class fbxPropertyArrayLong : public fbxProperty{
private:
	int64_t *pValues;
	int pCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property. */
	fbxPropertyArrayLong();
	
	/** \brief Load property. */
	fbxPropertyArrayLong( decBaseFileReader &reader );
	
protected:
	/** \brief Clean up property. */
	virtual ~fbxPropertyArrayLong();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Value count. */
	inline int GetCount() const{ return pCount; }
	
	/** \brief Value at index. */
	int64_t GetValueAt( int index ) const;
	
	/** \brief Add value. */
	void AddValue( int64_t value );
	
	/** \brief Casting throwing exception if wrong type. */
	virtual fbxPropertyArrayLong &CastArrayLong();
	
	/** \brief Get values as specific type if possible. */
	virtual int GetValueCount();
	virtual bool GetValueAtAsBool( int index );
	virtual int GetValueAtAsInt( int index );
	virtual float GetValueAtAsFloat( int index );
	virtual double GetValueAtAsDouble( int index );
	
	
	
	/** \brief Save to file. */
	virtual void Save( decBaseFileWriter &writer );
	
	/** \brief Debug print property structure. */
	virtual void DebugPrintStructure( deBaseModule &logger, const decString &prefix ) const;
	/*@}*/
};

#endif
