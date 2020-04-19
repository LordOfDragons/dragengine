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

#ifndef _DEALIMAGE_H_
#define _DEALIMAGE_H_

#include <GLES/gl.h>
#include <GLES2/gl2.h>

#include "../deObject.h"

class dealDisplay;
class decTgaImage;


/**
 * \brief Image.
 */
class dealImage : public deObject{
private:
	dealDisplay &pDisplay;
	
	GLuint pTexture;
	int pWidth;
	int pHeight;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create image from file. */
	dealImage( dealDisplay &display, const char *filename );
	
	/** \brief Clean up image. */
	virtual ~dealImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Display. */
	inline dealDisplay &GetDisplay() const{ return pDisplay; }
	
	/** \brief Texture. */
	inline GLuint GetTexture() const{ return pTexture; }
	
	/** \brief Texture width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Texture height. */
	inline int GetHeight() const{ return pHeight; }
	/*@}*/
	
private:
	void pCleanUp();
	void pLoadImage( const char *filename );
	void pCreateTexture( decTgaImage &tgaImage );
};

#endif
 
 
