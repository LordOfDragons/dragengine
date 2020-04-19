/* 
 * Drag[en]gine IGDE Font Editor
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

// include only once
#ifndef _FEFONTIMAGE_H_
#define _FEFONTIMAGE_H_

// includes
#include <dragengine/deObject.h>

#include <dragengine/common/string/decString.h>

// predefinitions
class feFont;

class deEngine;
class deImage;



/**
 * @brief Font Image.
 * Image in a font containing glyphs.
 */
class feFontImage : public deObject{
private:
	deEngine *pEngine;
	feFont *pParentFont;
	
	deImage *pEngImage;
	
	decString pFilename;
	int pWidth;
	int pHeight;
	
	bool pChanged;
	bool pSaved;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new font image. */
	feFontImage( deEngine *engine );
	/** Cleans up the font image. */
	virtual ~feFontImage();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the linked engine object. */
	inline deEngine *GetEngine() const{ return pEngine; }
	/** Retrieves the engine image. */
	inline deImage *GetEngineImage() const{ return pEngImage; }
	
	/** Retrieves the parent font or NULL if not assigned. */
	inline feFont *GetParentFont() const{ return pParentFont; }
	/** Sets the parent font or NULL it not assigned. */
	void SetParentFont( feFont *font );
	
	/** Retrieves the width of the image. */
	inline int GetWidth() const{ return pWidth; }
	/** Retrieves the height of the image. */
	inline int GetHeight() const{ return pHeight; }
	/** Sets the size of the image. */
	void SetSize( int width, int height );
	/** Retrieves the filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	/** Sets the filename. */
	void SetFilename( const char *filename, bool load );
	/** Loads the image from the stored path. */
    void LoadImage();
	
	/** Determines if the image has changed. */
	inline bool GetChanged() const{ return pChanged; }
	/** Sets if the image has changed. */
	void SetChanged( bool changed );
	/** Determines if the image has been saved. */
	inline bool GetSaved() const{ return pSaved; }
	/** Sets if the image has been saved. */
	void SetSaved( bool saved );
	
	/** Notifies the parent if assigned that the image changed. */
    void NotifyImageChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
