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

#ifndef _DESERVICEOBJECT_H_
#define _DESERVICEOBJECT_H_

#include <stdint.h>

#include "../../common/collection/decObjectList.h"
#include "../../common/collection/decObjectDictionary.h"
#include "../../common/file/decMemoryFile.h"
#include "../../resources/deResource.h"


/**
 * \brief Service object.
 * \version 1.23
 * 
 * Stores data object for use with platform services. Object value can be a boolean,
 * an integer, a float, a string, a resource, data, a list of objects or a dictionary
 * of objects.
 * 
 * A newly created service object is of type evtDictionary.
 */
class DE_DLL_EXPORT deServiceObject : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deServiceObject> Ref;
	
	
public:
	/** \brief Value type. */
	enum eValueType{
		/** \brief Boolean. */
		evtBoolean,
		
		/** \brief Integer. */
		evtInteger,
		
		/** \brief Float. */
		evtFloat,
		
		/** \brief String. */
		evtString,
		
		/** \brief Resource. */
		evtResource,
		
		/** \brief Data. */
		evtData,
		
		/** \brief List of objects. */
		evtList,
		
		/** \brief Dictionary of objects. */
		evtDictionary
	};
	
	
	
private:
	eValueType pValueType;
	
	union{
		bool pBoolean;
		int pInteger;
		float pFloat;
	};
	decString pString;
	deResource::Ref pResource;
	decMemoryFile::Ref pData;
	decObjectList pList;
	decObjectDictionary pDictionary;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create service object of type evtDictionary. */
	deServiceObject();
	
	/** \brief Create service object of type evtBoolean. */
	static deServiceObject::Ref NewBool( bool value );
	
	/** \brief Create service object of type evtInteger. */
	static deServiceObject::Ref NewInt( int value );
	
	/** \brief Create service object of type evtFloat. */
	static deServiceObject::Ref NewFloat( float value );
	
	/** \brief Create service object of type evtString. */
	static deServiceObject::Ref NewString( const char *value );
	
	/** \brief Create service object of type evtResource. */
	static deServiceObject::Ref NewResource( deResource *value );
	
	/** \brief Create service object of type evtData. */
	static deServiceObject::Ref NewData( const decMemoryFile::Ref &value );
	
	/** \brief Create service object of type evtList. */
	static deServiceObject::Ref NewList();
	
	/** \brief Copy content from another object. */
	deServiceObject( const deServiceObject &object, bool deep = false );
	
protected:
	/** \brief Clean up service object. */
	~deServiceObject();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Value type. */
	inline eValueType GetValueType() const{ return pValueType; }
	
	
	
	/** \brief Is boolean value. */
	inline bool IsBoolean() const{ return pValueType == evtBoolean; }
	
	/**
	 * \brief Boolean value.
	 * \throws EInvalidParam Value type is not evtBoolean.
	 */
	bool GetBoolean() const;
	
	/**
	 * \brief Set object to boolean value.
	 * \throws EInvalidParam Value type is not evtBoolean.
	 */
	void SetBoolean( bool value );
	
	
	
	/** \brief Is integer value. */
	inline bool IsInteger() const{ return pValueType == evtInteger; }
	
	/**
	 * \brief Integer value.
	 * \throws EInvalidParam Value type is not evtInteger.
	 */
	int GetInteger() const;
	
	/**
	 * \brief Set object to integer value.
	 * \throws EInvalidParam Value type is not evtInteger.
	 */
	void SetInteger( int value );
	
	
	
	/** \brief Is float value. */
	inline bool IsFloat() const{ return pValueType == evtFloat; }
	
	/**
	 * \brief Float value.
	 * \throws EInvalidParam Value type is not evtFloat.
	 */
	float GetFloat() const;
	
	/**
	 * \brief Set object to float value.
	 * \throws EInvalidParam Value type is not evtFloat.
	 */
	void SetFloat( float value );
	
	
	
	/** \brief Is string value. */
	inline bool IsString() const{ return pValueType == evtString; }
	
	/**
	 * \brief String value.
	 * \throws EInvalidParam Value type is not evtString.
	 */
	const decString &GetString() const;
	
	/**
	 * \brief Set object to string value.
	 * \throws EInvalidParam Value type is not evtString.
	 */
	void SetString( const char *value );
	
	
	
	/** \brief Is resource value. */
	inline bool IsResource() const{ return pValueType == evtResource; }
	
	/**
	 * \brief Resource value.
	 * \throws EInvalidParam Value type is not evtResource.
	 */
	const deResource::Ref &GetResource() const;
	
	/**
	 * \brief Set object to resource value.
	 * \throws EInvalidParam Value type is not evtResource.
	 */
	void SetResource( const deResource::Ref &value );
	
	
	
	/** \brief Is data value. */
	inline bool IsData() const{ return pValueType == evtData; }
	
	/**
	 * \brief Data value.
	 * \throws EInvalidParam Value type is not evtData.
	 */
	const decMemoryFile::Ref &GetData() const;
	
	/**
	 * \brief Set object to data value.
	 * \throws EInvalidParam Value type is not evtData.
	 */
	void SetData( const decMemoryFile::Ref &value );
	
	
	
	/** \brief Is list value. */
	inline bool IsList() const{ return pValueType == evtList; }
	
	/** \brief Is dictionary value. */
	inline bool IsDictionary() const{ return pValueType == evtDictionary; }
	
	/**
	 * \brief Count of child objects.
	 * \throws EInvalidParam Value type is not evtList or evtDictionary.
	 */
	int GetChildCount() const;
	
	/**
	 * \brief List of all children keys.
	 * \throws EInvalidParam Value type is not evtDictionary.
	 */
	decStringList GetChildrenKeys() const;
	
	/**
	 * \brief Child object at index.
	 * \throws EInvalidParam Value type is not evtList.
	 */
	deServiceObject::Ref GetChildAt( int index ) const;
	
	/**
	 * \brief Child object with key or nullptr if absent.
	 * \throws EInvalidParam Value type is not evtDictionary.
	 */
	deServiceObject::Ref GetChildAt( const char *key ) const;
	
	/**
	 * \brief Add child object.
	 * \throws EInvalidParam Value type is not evtList.
	 */
	void AddChild( const deServiceObject::Ref &child );
	
	/**
	 * \brief Add child object.
	 * 
	 * Overwrites children with same name if present.
	 * 
	 * \throws EInvalidParam Value type is not evtDictionary.
	 */
	void SetChildAt( const char *key, const deServiceObject::Ref &child );
	
	/**
	 * \brief Add boolean type child object.
	 * \throws EInvalidParam Value type is not evtList.
	 */
	void AddBoolChild( bool value );
	
	/**
	 * \brief Add boolean type child object.
	 * 
	 * Overwrites children with same name if present.
	 * 
	 * \throws EInvalidParam Value type is not evtDictionary.
	 */
	void SetBoolChildAt( const char *key, bool value );
	
	/**
	 * \brief Add integer type child object.
	 * \throws EInvalidParam Value type is not evtList.
	 */
	void AddIntChild( int value );
	
	/**
	 * \brief Add integer type child object.
	 * 
	 * Overwrites children with same name if present.
	 * 
	 * \throws EInvalidParam Value type is not evtDictionary.
	 */
	void SetIntChildAt( const char *key, int value );
	
	/**
	 * \brief Add float type child object.
	 * \throws EInvalidParam Value type is not evtList.
	 */
	void AddFloatChild( float value );
	
	/**
	 * \brief Add float type child object.
	 * 
	 * Overwrites children with same name if present.
	 * 
	 * \throws EInvalidParam Value type is not evtDictionary.
	 */
	void SetFloatChildAt( const char *key, float value );
	
	/**
	 * \brief Add string type child object.
	 * \throws EInvalidParam Value type is not evtList.
	 */
	void AddStringChild( const char *value );
	
	/**
	 * \brief Add string type child object.
	 * 
	 * Overwrites children with same name if present.
	 * 
	 * \throws EInvalidParam Value type is not evtDictionary.
	 */
	void SetStringChildAt( const char *key, const char *value );
	
	/**
	 * \brief Add resource type child object.
	 * \throws EInvalidParam Value type is not evtDictionary.
	 */
	void AddResourceChild( deResource *value );
	
	/**
	 * \brief Add resource type child object.
	 * 
	 * Overwrites children with same name if present.
	 * 
	 * \throws EInvalidParam Value type is not evtDictionary.
	 */
	void SetResourceChildAt( const char *key, deResource *value );
	
	/**
	 * \brief Add data type child object.
	 * \throws EInvalidParam Value type is not evtList.
	 */
	void AddDataChild( const decMemoryFile::Ref &value );
	
	/**
	 * \brief Add data type child object.
	 * 
	 * Overwrites children with same name if present.
	 * 
	 * \throws EInvalidParam Value type is not evtDictionary.
	 */
	void SetDataChildAt( const char *key, const decMemoryFile::Ref &value );
	
	/**
	 * \brief Remove child object at index.
	 * \throws EInvalidParam Value type is not evtList.
	 */
	void RemoveChild( int index );
	
	/**
	 * \brief Remove child object with key if present.
	 * \throws EInvalidParam Value type is not evtDictionary.
	 */
	void RemoveChild( const char *key );
	
	/**
	 * \brief Remove all child objects.
	 * \throws EInvalidParam Value type is not evtList or evtDictionary.
	 */
	void RemoveAllChildren();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Service objects are equal. */
	bool operator==( const deServiceObject &other ) const;
	
	/** \brief Service objects are not equal. */
	bool operator!=( const deServiceObject &other ) const;
	/*@}*/
};

#endif
