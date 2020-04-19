/* 
 * Drag[en]gine Game Engine
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

#ifndef _DECXMLCHARREFERENCE_H_
#define _DECXMLCHARREFERENCE_H_

#include "decXmlElement.h"
#include "../string/decString.h"


/**
 * \brief XML Character Reference.
 */
class decXmlCharReference : public decXmlElement{
public:
	/** \brief Radix enumeration. */
	enum eRadices{
		/** \brief Decimal radix. */
		erDecimal,
		
		/** \brief Hexadecimal radix. */
		erHexadecimal
	};
	
	
	
private:
	decString pData;
	eRadices pRadix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create xml character reference wwith the given data and radix.
	 * \param data Character Data
	 * \param radix Value from eRadices
	 */
	decXmlCharReference( const char *data, eRadices radix );
	
protected:
	/** \brief Clean up xml character data. */
	virtual ~decXmlCharReference();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Data. */
	inline const decString &GetData() const{ return pData; }
	
	/** \brief Radix. */
	inline eRadices GetRadix() const{ return pRadix; }
	
	/** \brief Is decimal. */
	inline bool IsDecimal() const{ return pRadix == erDecimal; }
	
	/** \brief Is hexadecimal. */
	inline bool IsHexadecimal() const{ return pRadix == erHexadecimal; }
	
	/** \brief Set data. */
	void SetData( const char *data );
	
	/** \brief Set radix. */
	void SetRadix( eRadices radix );
	/*@}*/
	
	
	
	/** \name Visiting */
	void Visit( decXmlVisitor &visitor );
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	virtual bool CanCastToCharReference() const;
	virtual decXmlCharReference *CastToCharReference();
	/*@}*/
};

#endif
