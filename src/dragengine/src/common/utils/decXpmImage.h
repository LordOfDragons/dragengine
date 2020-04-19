/* 
 * Drag[en]gine Game Engine
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

#ifndef _DECXPMIMAGE_H_
#define _DECXPMIMAGE_H_


/**
 * \brief Simple XPM Image container.
 *
 * A simple parser for XPM images linked into the code.
 * It is very basic and stores the image as RGB format.
 */
class decXpmImage{
private:
	int pWidth, pHeight;
	char *pData;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create new xpm image object from the given xmp data source.
	 * \param xpmData Pointer to the xpm data. Usually the pointer to the compiled in xpm data.
	 * \param flip Determines if the image has to be flippep upside down.
	 */
	decXpmImage( const char *xpmData[], bool flip );
	
	/** \brief Clean up xpm image object. */
	~decXpmImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Width of the image. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Height of the image. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Pointer to the RGB aligned data. */
	inline const char *GetData() const{ return pData; }
	/*@}*/
	
	
	
private:
	int pReadInt( const char **pdata );
	int pReadColor( const char **pdata );
};

#endif
