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

#ifndef _DEALCONFIGWINDOW_H_
#define _DEALCONFIGWINDOW_H_



/**
 * \brief Window Configuration.
 */
class dealConfigWindow{
private:
	int pX;
	int pY;
	int pWidth;
	int pHeight;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window configuration. */
	dealConfigWindow();
	
	/** \brief Clean up window configuration. */
	~dealConfigWindow();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief X position. */
	inline int GetX() const{ return pX; }
	
	/** \brief Set x position. */
	void SetX( int x );
	
	/** \brief Y position. */
	inline int GetY() const{ return pY; }
	
	/** \brief Set y position. */
	void SetY( int y );
	
	/** \brief Width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Set widht. */
	void SetWidth( int width );
	
	/** \brief Height. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Set height. */
	void SetHeight( int height );
	/*@}*/
};

#endif
