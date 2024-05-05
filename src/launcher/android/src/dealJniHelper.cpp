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
