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

#ifndef _DEALJNIHELPER_H_
#define _DEALJNIHELPER_H_

#include <jni.h>

#include <dragengine/common/string/decString.h>



/**
 * \brief Global referenced counted jobject.
 */
class jniGlobalJObject{
private:
	JNIEnv *pEnv;
	jobject pObject;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create jobject. */
	jniGlobalJObject();
	
	/** \brief Create jobject. */
	jniGlobalJObject( JNIEnv *env, jobject object );
	
	/** \brief Clean jobject. */
	~jniGlobalJObject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Cast. */
	inline jobject Ptr(){ return pObject; }
	
	/** \brief Auto-cast. */
	inline operator jobject(){ return pObject; }
	
	/** \brief Testing. */
	inline operator bool(){ return pObject != NULL; }
	
	/** \brief Testing. */
	inline bool operator!(){ return pObject == NULL; }
	
	/** \brief Copy. */
	jniGlobalJObject &operator=( const jniGlobalJObject &object );
	
	/** \brief Set. */
	void Set( JNIEnv *env, jobject object );
	
	/** \brief Clear. */
	void Clear();
	/*@}*/
};



/**
 * \brief Global referenced counted jclass.
 */
class jniGlobalJClass{
private:
	JNIEnv *pEnv;
	jclass pClass;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create jobject. */
	jniGlobalJClass();
	
	/** \brief Create jobject. */
	jniGlobalJClass( JNIEnv *env, jclass clazz );
	
	/** \brief Clean jobject. */
	~jniGlobalJClass();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Cast. */
	inline jobject Ptr(){ return pClass; }
	
	/** \brief Auto-cast. */
	inline operator jclass(){ return pClass; }
	
	/** \brief Testing. */
	inline operator bool(){ return pClass != NULL; }
	
	/** \brief Testing. */
	inline bool operator!(){ return pClass == NULL; }
	
	/** \brief Copy. */
	jniGlobalJClass &operator=( const jniGlobalJClass &clazz );
	
	/** \brief Set. */
	void Set( JNIEnv *env, jclass clazz );
	
	/** \brief Clear. */
	void Clear();
	/*@}*/
};



/**
 * \brief Global referenced counted jstring.
 */
class jniGlobalJString{
private:
	JNIEnv *pEnv;
	jstring pString;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create jobject. */
	jniGlobalJString();
	
	/** \brief Create jobject. */
	jniGlobalJString( JNIEnv *env, jstring string );
	
	/** \brief Create jobject. */
	jniGlobalJString( JNIEnv *env, const char *string );
	
	/** \brief Clean jobject. */
	~jniGlobalJString();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Cast. */
	inline jobject Ptr(){ return pString; }
	
	/** \brief Auto-cast. */
	inline operator jstring(){ return pString; }
	
	/** \brief Testing. */
	inline operator bool(){ return pString != NULL; }
	
	/** \brief Testing. */
	inline bool operator!(){ return pString == NULL; }
	
	/** \brief Auto-cast to decString. */
	operator decString();
	
	/** \brief Copy. */
	jniGlobalJString &operator=( const jniGlobalJString &string );
	
	/** \brief Set. */
	void Set( JNIEnv *env, jstring string );
	
	/** \brief Clear. */
	void Clear();
	/*@}*/
};

#endif
