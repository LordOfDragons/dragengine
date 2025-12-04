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

#ifndef _FBXPROPERTY_H_
#define _FBXPROPERTY_H_

#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/file/decBaseFileReader::Ref.h>
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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<fbxProperty> Ref;
	
	
	
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
	virtual int64_t GetValueAtAsLong( int index ) const;
	virtual float GetValueAtAsFloat( int index ) const;
	virtual double GetValueAtAsDouble( int index ) const;
	
	/** \brief Read sequence of values as composed type. */
	virtual decVector2 GetValueAtAsVector2( int index ) const;
	virtual decVector GetValueAtAsVector( int index ) const;
	virtual decMatrix GetValueAtAsMatrix( int index ) const;
	
	inline decVector2 GetValueAsVector2() const{ return GetValueAtAsVector2( 0 ); }
	inline decVector GetValueAsVector() const{ return GetValueAtAsVector( 0 ); }
	inline decMatrix GetValueAsMatrix() const{ return GetValueAtAsMatrix( 0 ); }
	
	
	
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
	void pGetValueReader( decBaseFileReader &reader, decBaseFileReader::Ref &valueReader );
};

#endif
