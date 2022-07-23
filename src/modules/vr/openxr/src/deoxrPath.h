/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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
	deoxrPath( const deoxrInstance &instance, XrPath path );
	
	/** Create path. */
	deoxrPath( const deoxrInstance &instance, const char *name );
	
	/** Create path. */
	deoxrPath( const deoxrPath &path );
	
	/** Clean up path. */
	~deoxrPath();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Path. */
	inline XrPath GetPath() const{ return pPath; }
	
	/** String path. */
	inline const decString &GetName() const{ return pName; }
	
	/** Empty. */
	inline bool IsEmpty() const{ return pPath == XR_NULL_PATH; }
	
	/** Not empty. */
	inline bool IsNotEmpty() const{ return pPath != XR_NULL_PATH; }
	
	/** Set empty. */
	void Empty();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Path. */
	inline operator XrPath() const{ return pPath; }
	
	/** String path. */
	inline operator const decString &() const{ return pName; }
	
	/** String path. */
	inline operator const char *() const{ return pName; }
	
	/** Is empty. */
	inline operator bool() const{ return pPath != XR_NULL_PATH; }
	
	/** Is empty. */
	inline bool operator!() const{ return pPath == XR_NULL_PATH; }
	
	/** Same path. */
	inline bool operator==( const deoxrPath &path ) const{ return pPath == path.pPath; }
	
	/** Same path. */
	inline bool operator==( XrPath path ) const{ return pPath == path; }
	
	/** Same path. */
	inline bool operator==( const decString &path ) const{ return pName == path; }
	
	/** Same path. */
	inline bool operator==( const char *path ) const{ return pName == path; }
	
	/** Different path. */
	inline bool operator!=( const deoxrPath &path ) const{ return pPath != path.pPath; }
	
	/** Different path. */
	inline bool operator!=( XrPath path ) const{ return pPath != path; }
	
	/** Different path. */
	inline bool operator!=( const decString &path ) const{ return pName != path; }
	
	/** Different path. */
	inline bool operator!=( const char *path ) const{ return pName != path; }
	
	/** Assign. */
	deoxrPath &operator=( const deoxrPath &path );
	/*@}*/
};

#endif

