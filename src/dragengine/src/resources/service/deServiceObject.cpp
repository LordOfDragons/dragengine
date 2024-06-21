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

#include <memory.h>

#include "deServiceObject.h"


// Class deServiceObject
//////////////////////////

// Constructor, destructor
////////////////////////////

deServiceObject::deServiceObject() :
pValueType( evtDictionary ){
}

deServiceObject::Ref deServiceObject::NewBool( bool value ){
	const deServiceObject::Ref object( deServiceObject::Ref::New( new deServiceObject ) );
	object->pValueType = evtBoolean;
	object->pBoolean = value;
	return object;
}

deServiceObject::Ref deServiceObject::NewInt( int value ){
	const deServiceObject::Ref object( deServiceObject::Ref::New( new deServiceObject ) );
	object->pValueType = evtInteger;
	object->pInteger = value;
	return object;
}

deServiceObject::Ref deServiceObject::NewFloat( float value ){
	const deServiceObject::Ref object( deServiceObject::Ref::New( new deServiceObject ) );
	object->pValueType = evtFloat;
	object->pFloat = value;
	return object;
}

deServiceObject::Ref deServiceObject::NewString( const char *value ){
	const deServiceObject::Ref object( deServiceObject::Ref::New( new deServiceObject ) );
	object->pValueType = evtString;
	object->pString = value;
	return object;
}

deServiceObject::Ref deServiceObject::NewResource( deResource *value ){
	const deServiceObject::Ref object( deServiceObject::Ref::New( new deServiceObject ) );
	object->pValueType = evtResource;
	object->pResource = value;
	return object;
}

deServiceObject::Ref deServiceObject::NewData( const decMemoryFile::Ref &value ){
	const deServiceObject::Ref object( deServiceObject::Ref::New( new deServiceObject ) );
	object->pValueType = evtData;
	object->pData = value;
	return object;
}

deServiceObject::Ref deServiceObject::NewList(){
	const deServiceObject::Ref object( deServiceObject::Ref::New( new deServiceObject ) );
	object->pValueType = evtList;
	return object;
}

deServiceObject::deServiceObject( const deServiceObject &object, bool deep ) :
pValueType( object.pValueType ),
pInteger( object.pInteger ),
pString( object.pString ),
pResource( object.pResource ),
pData( object.pData )
{
	if( deep ){
		const decStringList keys( object.pDictionary.GetKeys() );
		const int count = keys.GetCount();
		int i;
		
		pDictionary.RemoveAll();
		for( i=0; i<count; i++ ){
			const decString &key = keys.GetAt( i );
			const deServiceObject::Ref copy( deServiceObject::Ref::New( new deServiceObject(
				*( ( deServiceObject* )object.pDictionary.GetAt( key ) ), true ) ) );
			pDictionary.SetAt( key, copy );
		}
		
	} else {
		pDictionary = object.pDictionary;
	}
}

deServiceObject::~deServiceObject(){
}



// Management
///////////////

bool deServiceObject::GetBoolean() const{
	DEASSERT_TRUE( pValueType == evtBoolean )
	return pBoolean;
}

void deServiceObject::SetBoolean( bool value ){
	DEASSERT_TRUE( pValueType == evtBoolean )
	pBoolean = value;
}

int deServiceObject::GetInteger() const{
	DEASSERT_TRUE( pValueType == evtInteger )
	return pInteger;
}

void deServiceObject::SetInteger( int value ){
	DEASSERT_TRUE( pValueType == evtInteger )
	pInteger = value;
}

float deServiceObject::GetFloat() const{
	DEASSERT_TRUE( pValueType == evtFloat )
	return pFloat;
}

void deServiceObject::SetFloat( float value ){
	DEASSERT_TRUE( pValueType == evtFloat )
	pFloat = value;
}

const decString &deServiceObject::GetString() const{
	DEASSERT_TRUE( pValueType == evtString )
	return pString;
}

void deServiceObject::SetString( const char *value ){
	DEASSERT_TRUE( pValueType == evtString )
	pString = value;
}

const deResource::Ref &deServiceObject::GetResource() const{
	DEASSERT_TRUE( pValueType == evtResource )
	return pResource;
}

void deServiceObject::SetResource( const deResource::Ref &value ){
	DEASSERT_TRUE( pValueType == evtResource )
	pResource = value;
}

const decMemoryFile::Ref &deServiceObject::GetData() const{
	DEASSERT_TRUE( pValueType == evtData )
	return pData;
}

void deServiceObject::SetData( const decMemoryFile::Ref &value ){
	DEASSERT_TRUE( pValueType == evtData )
	pData = value;
}



int deServiceObject::GetChildCount() const{
	DEASSERT_TRUE( pValueType == evtList || pValueType == evtDictionary )
	return pValueType == evtDictionary ? pDictionary.GetCount() : pList.GetCount();
}

deServiceObject::Ref deServiceObject::GetChildAt( int index ) const{
	DEASSERT_TRUE( pValueType == evtList )
	return Ref( ( deServiceObject* )pList.GetAt( index ) );
}

deServiceObject::Ref deServiceObject::GetChildAt( const char *key ) const{
	DEASSERT_TRUE( pValueType == evtDictionary )
	deObject *value;
	return Ref( pDictionary.GetAt( key, &value ) ? ( deServiceObject* )value : nullptr );
}

decStringList deServiceObject::GetChildrenKeys() const{
	DEASSERT_TRUE( pValueType == evtDictionary )
	return pDictionary.GetKeys();
}

void deServiceObject::AddChild( const deServiceObject::Ref &child ){
	DEASSERT_TRUE( pValueType == evtList )
	pList.Add( child );
}

void deServiceObject::SetChildAt( const char *key, const deServiceObject::Ref &child ){
	DEASSERT_TRUE( pValueType == evtDictionary )
	pDictionary.SetAt( key, child );
}

void deServiceObject::AddBoolChild( bool value ){
	DEASSERT_TRUE( pValueType == evtList )
	pList.Add( NewBool( value ) );
}

void deServiceObject::SetBoolChildAt( const char *key, bool value ){
	DEASSERT_TRUE( pValueType == evtDictionary )
	pDictionary.SetAt( key, NewBool( value ) );
}

void deServiceObject::AddIntChild( int value ){
	DEASSERT_TRUE( pValueType == evtList )
	pList.Add( NewInt( value ) );
}

void deServiceObject::SetIntChildAt( const char *key, int value ){
	DEASSERT_TRUE( pValueType == evtDictionary )
	pDictionary.SetAt( key, NewInt( value ) );
}

void deServiceObject::AddFloatChild( float value ){
	DEASSERT_TRUE( pValueType == evtList )
	pList.Add( NewFloat( value ) );
}

void deServiceObject::SetFloatChildAt( const char *key, float value ){
	DEASSERT_TRUE( pValueType == evtDictionary )
	pDictionary.SetAt( key, NewFloat( value ) );
}

void deServiceObject::AddStringChild( const char *value ){
	DEASSERT_TRUE( pValueType == evtList )
	pList.Add( NewString( value ) );
}

void deServiceObject::SetStringChildAt( const char *key, const char *value ){
	DEASSERT_TRUE( pValueType == evtDictionary )
	pDictionary.SetAt( key, NewString( value ) );
}

void deServiceObject::AddResourceChild( deResource *value ){
	DEASSERT_TRUE( pValueType == evtList )
	pList.Add( NewResource( value ) );
}

void deServiceObject::SetResourceChildAt( const char *key, deResource *value ){
	DEASSERT_TRUE( pValueType == evtDictionary )
	pDictionary.SetAt( key, NewResource( value ) );
}

void deServiceObject::AddDataChild( const decMemoryFile::Ref &value ){
	DEASSERT_TRUE( pValueType == evtList )
	pList.Add( NewData( value ) );
}

void deServiceObject::SetDataChildAt( const char *key, const decMemoryFile::Ref &value ){
	DEASSERT_TRUE( pValueType == evtDictionary )
	pDictionary.SetAt( key, NewData( value ) );
}

void deServiceObject::RemoveChild( int index ){
	DEASSERT_TRUE( pValueType == evtList )
	pList.RemoveFrom( index );
}

void deServiceObject::RemoveChild( const char *key ){
	DEASSERT_TRUE( pValueType == evtDictionary )
	pDictionary.RemoveIfPresent( key );
}

void deServiceObject::RemoveAllChildren(){
	DEASSERT_TRUE( pValueType == evtList || pValueType == evtDictionary )
	
	if( pValueType == evtDictionary ){
		pDictionary.RemoveAll();
		
	}else{
		pList.RemoveAll();
	}
}



// Operators
//////////////

bool deServiceObject::operator==( const deServiceObject &other ) const{
	if( pValueType != other.pValueType ){
		return false;
	}
	
	switch( pValueType ){
	case evtBoolean:
		return pBoolean == other.pBoolean;
		
	case evtInteger:
		return pInteger == other.pInteger;
		
	case evtFloat:
		return pFloat == other.pFloat;
		
	case evtString:
		return pString == pString;
		
	case evtResource:
		return pResource == other.pResource;
		
	case evtData:
		if( pData ){
			return other.pData && pData->GetLength() == other.pData->GetLength()
				&& memcmp( pData->GetPointer(), other.pData->GetPointer(), pData->GetLength() ) == 0;
		}else{
			return ! other.pData;
		}
		
	case evtDictionary:{
		if( pDictionary.GetCount() != other.pDictionary.GetCount() ){
			return false;
		}
		
		const decStringList keys( pDictionary.GetKeys() );
		const int count = keys.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			const decString &key = keys.GetAt( i );
			const deServiceObject &so1 = *( ( deServiceObject* )pDictionary.GetAt( key ) );
			const deServiceObject &so2 = *( ( deServiceObject* )other.pDictionary.GetAt( key ) );
			if( so1 != so2 ){
				return false;
			}
		}
		return true;
		}
		
	default:
		return false;
	}
}

bool deServiceObject::operator!=( const deServiceObject &other ) const{
	return ! ( *this == other );
}
