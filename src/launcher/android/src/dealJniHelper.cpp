/* 
 * Drag[en]gine Android Launcher
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

#include <stdlib.h>

#include "dealJniHelper.h"



// Class jniGlobalJObject
///////////////////////////

jniGlobalJObject::jniGlobalJObject() :
pEnv( NULL ),
pObject( NULL ){
}

jniGlobalJObject::jniGlobalJObject( JNIEnv *env, jobject object ) :
pEnv( env ),
pObject( NULL )
{
	if( object ){
		pObject = env->NewGlobalRef( object );
	}
}

jniGlobalJObject::~jniGlobalJObject(){
	if( pObject ){
		pEnv->DeleteGlobalRef( pObject );
	}
}

jniGlobalJObject &jniGlobalJObject::operator=( const jniGlobalJObject &object ){
	Set( object.pEnv, object.pObject );
	return *this;
}

void jniGlobalJObject::Set( JNIEnv *env, jobject object ){
	if( pObject ){
		pEnv->DeleteGlobalRef( pObject );
		pEnv = NULL;
		pObject = NULL;
	}
	if( object ){
		pEnv = env;
		pObject = env->NewGlobalRef( object );
	}
}

void jniGlobalJObject::Clear(){
	Set( NULL, NULL );
}



// Class jniGlobalJClass
//////////////////////////

jniGlobalJClass::jniGlobalJClass() :
pEnv( NULL ),
pClass( NULL ){
}

jniGlobalJClass::jniGlobalJClass( JNIEnv *env, jclass clazz ) :
pEnv( env ),
pClass( NULL )
{
	if( clazz ){
		pClass = ( jclass )env->NewGlobalRef( clazz );
	}
}

jniGlobalJClass::~jniGlobalJClass(){
	if( pClass ){
		pEnv->DeleteGlobalRef( pClass );
	}
}

jniGlobalJClass &jniGlobalJClass::operator=( const jniGlobalJClass &clazz ){
	Set( clazz.pEnv, clazz.pClass );
	return *this;
}

void jniGlobalJClass::Set( JNIEnv *env, jclass clazz ){
	if( pClass ){
		pEnv->DeleteGlobalRef( pClass );
		pEnv = NULL;
		pClass = NULL;
	}
	if( clazz ){
		pEnv = env;
		pClass = ( jclass )env->NewGlobalRef( clazz );
	}
}

void jniGlobalJClass::Clear(){
	Set( NULL, NULL );
}



// Class jniGlobalJString
///////////////////////////

jniGlobalJString::jniGlobalJString() :
pEnv( NULL ),
pString( NULL ){
}

jniGlobalJString::jniGlobalJString( JNIEnv *env, jstring string ) :
pEnv( env ),
pString( NULL )
{
	if( string ){
		pString = ( jstring )env->NewGlobalRef( string );
	}
}

jniGlobalJString::jniGlobalJString( JNIEnv *env, const char *string ) :
pEnv( env ),
pString( NULL )
{
	pString = ( jstring )env->NewGlobalRef( env->NewStringUTF( string ) );
}

jniGlobalJString::~jniGlobalJString(){
	if( pString ){
		pEnv->DeleteGlobalRef( pString );
	}
}

jniGlobalJString::operator decString(){
	decString string;
	
	if( pString ){
		const char * const strString = pEnv->GetStringUTFChars( pString, 0 );
		string = strString;
		pEnv->ReleaseStringUTFChars( pString, strString );
	}
	
	return string;
}

jniGlobalJString &jniGlobalJString::operator=( const jniGlobalJString &string ){
	Set( string.pEnv, string.pString );
	return *this;
}

void jniGlobalJString::Set( JNIEnv *env, jstring string ){
	if( pString ){
		pEnv->DeleteGlobalRef( pString );
		pEnv = NULL;
		pString = NULL;
	}
	if( string ){
		pEnv = env;
		pString = ( jstring )env->NewGlobalRef( string );
	}
}

void jniGlobalJString::Clear(){
	Set( NULL, NULL );
}
