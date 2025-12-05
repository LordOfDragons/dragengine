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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dealImage> Ref;


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
 
 
