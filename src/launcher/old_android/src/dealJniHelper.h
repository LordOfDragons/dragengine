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
	jniGlobalJObject(JNIEnv *env, jobject object);
	
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
	jniGlobalJObject &operator=(const jniGlobalJObject &object);
	
	/** \brief Set. */
	void Set(JNIEnv *env, jobject object);
	
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
	jniGlobalJClass(JNIEnv *env, jclass clazz);
	
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
	jniGlobalJClass &operator=(const jniGlobalJClass &clazz);
	
	/** \brief Set. */
	void Set(JNIEnv *env, jclass clazz);
	
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
	jniGlobalJString(JNIEnv *env, jstring string);
	
	/** \brief Create jobject. */
	jniGlobalJString(JNIEnv *env, const char *string);
	
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
	jniGlobalJString &operator=(const jniGlobalJString &string);
	
	/** \brief Set. */
	void Set(JNIEnv *env, jstring string);
	
	/** \brief Clear. */
	void Clear();
	/*@}*/
};

#endif
