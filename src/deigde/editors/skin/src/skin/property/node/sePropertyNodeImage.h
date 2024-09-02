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

#ifndef _SEPROPERTYNODEIMAGE_H_
#define _SEPROPERTYNODEIMAGE_H_

#include "sePropertyNode.h"

class deImage;


/**
 * \brief Texture property image node.
 */
class sePropertyNodeImage : public sePropertyNode{
private:
	decString pPath;
	deImage *pImage;
	decPoint pRepeat;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create node. */
	sePropertyNodeImage( deEngine &engine );
	
	/** \brief Create copy of node. */
	sePropertyNodeImage( const sePropertyNodeImage &node );
	
	/** \brief Clean up node. */
	virtual ~sePropertyNodeImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Image path or empty string if not set. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set image path or empty path if not set. */
	void SetPath( const char *path );
	
	/** \brief Image or \em NULL if not set. */
	inline deImage *GetImage() const{ return pImage; }
	
	/** \brief Update image. */
	void UpdateImage();
	
	/** \brief Repeat count. */
	inline const decPoint &GetRepeat() const{ return pRepeat; }
	
	/**
	 * \brief Set repeat count.
	 * \throws deeInvalidParam One or more components of \em count are less than 1.
	 */
	void SetRepeat( const decPoint &repeat );
	
	
	
	/** \brief Create copy of node. */
	virtual sePropertyNode *Copy() const;
	
	/** \brief Update resources. */
	virtual void UpdateResources();
	/*@}*/
};

#endif
