/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEICON_H_
#define _IGDEICON_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class igdeEnvironment;
class igdeEditorModule;
class decBaseFileReader;
class deImage;
class decPath;



/**
 * \brief IGDE UI Icon.
 * 
 * Icons are immutable.
 */
class DE_DLL_EXPORT igdeIcon : public deObject{
private:
	void *pNativeIcon;
	decPoint pSize;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create icon from image. */
	igdeIcon( deImage &image );
	
	/** \brief Create icon from image using scaling if required. */
	igdeIcon( deImage &image, int with, int height );
	
	/** \brief Create icon from PNG image in IGDE virtual file system. */
	static igdeIcon *LoadPNG( igdeEnvironment &environment, const char *filename );
	
	/** \brief Create icon from PNG image in Module Data Directory. */
	static igdeIcon *LoadPNG( const igdeEditorModule &editor, const char *filename );
	
	/** \brief Create icon from game engine image. */
	static igdeIcon *LoadImage( igdeEnvironment &environment, const char *filename );
	
	
	
private:
	igdeIcon( void *nativeIcon, const decPoint &size );
	
	
	
protected:
	/**
	 * \brief Clean up icon.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeIcon();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Icon size. */
	inline const decPoint &GetSize() const{ return pSize; }
	
	/** \brief Create scaled copy of icon. */
	igdeIcon *Scale( const decPoint &size ) const;
	
	/**
	 * \brief Update icon from image.
	 * 
	 * Requires image to be of same size as icon size. After updating widgets using this
	 * icon are required to be repainted for the change to take effect.
	 */
	void Update( deImage &image );
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Get native icon pointer.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	void *GetNativeIcon() const{ return pNativeIcon; }
	/*@}*/
};

#endif
