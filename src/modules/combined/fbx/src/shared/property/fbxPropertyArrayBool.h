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

#ifndef _FBXPROPERTYARRAYBOOL_H_
#define _FBXPROPERTYARRAYBOOL_H_


#include "../fbxProperty.h"


/**
 * \brief FBX property array bool.
 */
class fbxPropertyArrayBool : public fbxProperty{
private:
	bool *pValues;
	int pCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property. */
	fbxPropertyArrayBool();
	
	/** \brief Load property. */
	fbxPropertyArrayBool( decBaseFileReader &reader );
	
protected:
	/** \brief Clean up property. */
	virtual ~fbxPropertyArrayBool();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Value count. */
	inline int GetCount() const{ return pCount; }
	
	/** \brief Value at index. */
	bool GetValueAt( int index ) const;
	
	/** \brief Add value. */
	void AddValue( bool value );
	
	/** \brief Casting throwing exception if wrong type. */
	virtual fbxPropertyArrayBool &CastArrayBool();
	
	/** \brief Get values as specific type if possible. */
	virtual int GetValueCount() const;
	virtual bool GetValueAtAsBool( int index ) const;
	virtual int GetValueAtAsInt( int index ) const;
	virtual int64_t GetValueAtAsLong( int index ) const;
	virtual float GetValueAtAsFloat( int index ) const;
	virtual double GetValueAtAsDouble( int index ) const;
	
	
	
	/** \brief Save to file. */
	virtual void Save( decBaseFileWriter &writer );
	
	/** \brief Debug print property structure. */
	virtual void DebugPrintStructure( deBaseModule &logger, const decString &prefix ) const;
	/*@}*/
};

#endif
