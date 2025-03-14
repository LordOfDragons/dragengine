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
