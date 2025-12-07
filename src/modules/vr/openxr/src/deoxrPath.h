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

#ifndef _DEOXRPATH_H_
#define _DEOXRPATH_H_

#include "deoxrBasics.h"

#include <dragengine/common/string/decString.h>

class deoxrInstance;


/**
 * Oxr path.
 */
class deoxrPath{
private:
	XrPath pPath;
	decString pName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create empty path. */
	deoxrPath();
	
	/** Create path. */
	deoxrPath(const deoxrInstance &instance, XrPath path);
	
	/** Create path. */
	deoxrPath(const deoxrInstance &instance, const char *name);
	
	/** Create path. */
	deoxrPath(const deoxrPath &path);
	
	/** Clean up path. */
	~deoxrPath();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Path. */
	inline XrPath GetPath() const{return pPath;}
	
	/** String path. */
	inline const decString &GetName() const{return pName;}
	
	/** Empty. */
	inline bool IsEmpty() const{return pPath == XR_NULL_PATH;}
	
	/** Not empty. */
	inline bool IsNotEmpty() const{return pPath != XR_NULL_PATH;}
	
	/** Set empty. */
	void Empty();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Path. */
	inline operator XrPath() const{return pPath;}
	
	/** String path. */
	inline operator const decString &() const{return pName;}
	
	/** String path. */
	inline operator const char *() const{return pName;}
	
	/** Is empty. */
	inline operator bool() const{return pPath != XR_NULL_PATH;}
	
	/** Is empty. */
	inline bool operator!() const{return pPath == XR_NULL_PATH;}
	
	/** Same path. */
	inline bool operator==(const deoxrPath &path) const{return pPath == path.pPath;}
	
	/** Same path. */
	inline bool operator==(XrPath path) const{return pPath == path;}
	
	/** Same path. */
	inline bool operator==(const decString &path) const{return pName == path;}
	
	/** Same path. */
	inline bool operator==(const char *path) const{return pName == path;}
	
	/** Different path. */
	inline bool operator!=(const deoxrPath &path) const{return pPath != path.pPath;}
	
	/** Different path. */
	inline bool operator!=(XrPath path) const{return pPath != path;}
	
	/** Different path. */
	inline bool operator!=(const decString &path) const{return pName != path;}
	
	/** Different path. */
	inline bool operator!=(const char *path) const{return pName != path;}
	
	/** Assign. */
	deoxrPath &operator=(const deoxrPath &path);
	/*@}*/
};

#endif

