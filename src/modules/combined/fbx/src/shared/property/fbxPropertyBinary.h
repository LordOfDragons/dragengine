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

#ifndef _FBXPROPERTYBINARY_H_
#define _FBXPROPERTYBINARY_H_


#include <stdint.h>

#include "../fbxProperty.h"


/**
 * \brief FBX property string.
 */
class fbxPropertyBinary : public fbxProperty{
private:
	uint8_t *pValue;
	int pLength;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property. */
	fbxPropertyBinary();
	
	/** \brief Load property. */
	fbxPropertyBinary( decBaseFileReader &reader );
	
protected:
	/** \brief Clean up property. */
	virtual ~fbxPropertyBinary();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Value. */
	inline const uint8_t *GetValue() const{ return pValue; }
	
	/** \brief Value length. */
	inline int GetLength() const{ return pLength; }
	
	/** \brief Set value. */
	void SetValue( const uint8_t *value, int length );
	
	/** \brief Casting throwing exception if wrong type. */
	virtual fbxPropertyBinary &CastBinary();
	
	
	
	/** \brief Save to file. */
	virtual void Save( decBaseFileWriter &writer );
	
	/** \brief Debug print property structure. */
	virtual void DebugPrintStructure( deBaseModule &logger, const decString &prefix ) const;
	/*@}*/
};

#endif
