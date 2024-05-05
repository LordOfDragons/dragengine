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
