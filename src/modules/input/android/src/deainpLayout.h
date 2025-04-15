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

#ifndef _DEAINPLAYOUT_H_
#define _DEAINPLAYOUT_H_

#include <dragengine/common/math/decMath.h>



/**
 * \brief Layout properties.
 * 
 * Position is relative to screen. Positive positions are measured from
 * the top-left screen corner. Negative positions are measured from the
 * bottom-right screen corner.
 * 
 */
class deainpLayout{
private:
	decPoint pPosition;
	decPoint pSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create layout. */
	deainpLayout();
	
	/** \brief Create layout. */
	deainpLayout( const decPoint &position, const decPoint &size );
	
	/** \brief Create copy of layout. */
	deainpLayout( const deainpLayout &copy );
	
	/** \brief Clean up layout. */
	~deainpLayout();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decPoint &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decPoint &position );
	
	/** \brief Size. */
	inline const decPoint &GetSize() const{ return pSize; }
	
	/** \brief Set size. */
	void SetSize( const decPoint &size );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Layouts are equal. */
	bool operator==( const deainpLayout &layout ) const;
	
	/** \brief Set from another layout. */
	deainpLayout &operator=( const deainpLayout &layout );
	/*@}*/
};

#endif
