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

#ifndef _DECTGAIMAGE_H_
#define _DECTGAIMAGE_H_

class decBaseFileReader;



/**
 * \brief Simple Targa Image container.
 */
class decTgaImage{
public:
	struct sPixel{
		unsigned char r, g, b, a;
	};
	
private:
	int pWidth;
	int pHeight;
	sPixel *pPixels;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create targa image from data. */
	decTgaImage( decBaseFileReader &reader );
	
	/** \brief Clean up targa image. */
	~decTgaImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Width of the image. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Height of the image. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Pointer to RGB aligned data. */
	inline sPixel *GetPixels() const{ return pPixels; }
	/*@}*/
	
private:
	void pReadTga( decBaseFileReader &reader );
};

#endif
