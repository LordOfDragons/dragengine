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

#ifndef _FBXPROPERTY_H_
#define _FBXPROPERTY_H_


#include <dragengine/deObject.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxPropertyBool;
class fbxPropertyShort;
class fbxPropertyInteger;
class fbxPropertyLong;
class fbxPropertyFloat;
class fbxPropertyDouble;
class fbxPropertyArrayBool;
class fbxPropertyArrayInteger;
class fbxPropertyArrayLong;
class fbxPropertyArrayFloat;
class fbxPropertyArrayDouble;
class fbxPropertyString;
class fbxPropertyBinary;

class decBaseFileReader;
class decBaseFileWriter;
class deBaseModule;


/**
 * \brief FBX property.
 */
class fbxProperty : public deObject{
public:
	/** \brief Type. */
	enum eType{
		etBoolean,
		etShort,
		etInteger,
		etLong,
		etFloat,
		etDouble,
		etArrayBoolean,
		etArrayInteger,
		etArrayLong,
		etArrayFloat,
		etArrayDouble,
		etString,
		etBinary
	};
	
	
	
private:
	eType pType;
	
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property. */
	fbxProperty( eType type );
	
	/** \brief Clean up property. */
	virtual ~fbxProperty();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Type. */
	inline eType GetType() const{ return pType; }
	
	
	
	/** \brief Casting throwing exception if wrong type. */
	virtual fbxPropertyBool &CastBool();
	virtual fbxPropertyShort &CastShort();
	virtual fbxPropertyInteger &CastInteger();
	virtual fbxPropertyLong &CastLong();
	virtual fbxPropertyFloat &CastFloat();
	virtual fbxPropertyDouble &CastDouble();
	virtual fbxPropertyArrayBool &CastArrayBool();
	virtual fbxPropertyArrayInteger &CastArrayInteger();
	virtual fbxPropertyArrayLong &CastArrayLong();
	virtual fbxPropertyArrayFloat &CastArrayFloat();
	virtual fbxPropertyArrayDouble &CastArrayDouble();
	virtual fbxPropertyString &CastString();
	virtual fbxPropertyBinary &CastBinary();
	
	/** \brief Get values as specific type if possible. */
	virtual bool GetValueAsBool() const;
	virtual int GetValueAsInt() const;
	virtual int64_t GetValueAsLong() const;
	virtual float GetValueAsFloat() const;
	virtual double GetValueAsDouble() const;
	
	virtual int GetValueCount() const;
	virtual bool GetValueAtAsBool( int index ) const;
	virtual int GetValueAtAsInt( int index ) const;
	virtual float GetValueAtAsFloat( int index ) const;
	virtual double GetValueAtAsDouble( int index ) const;
	
	/** \brief Read sequence of values as composed type. */
	virtual decVector2 GetValueAtAsVector2( int index ) const;
	virtual decVector GetValueAtAsVector( int index ) const;
	
	
	
	/** \brief Read from file. */
	static fbxProperty *Read( decBaseFileReader &reader );
	
	/** \brief Save to file. */
	virtual void Save( decBaseFileWriter &writer ) = 0;
	
	/** \brief Debug print property structure. */
	virtual void DebugPrintStructure( deBaseModule &logger, const decString &prefix ) const = 0;
	
	/** \brief Debug type names. */
	const char *DebugTypeName() const;
	/*@}*/
	
	
	
protected:
	void pGetValueReader( decBaseFileReader &reader, decBaseFileReaderReference &valueReader );
};

#endif
