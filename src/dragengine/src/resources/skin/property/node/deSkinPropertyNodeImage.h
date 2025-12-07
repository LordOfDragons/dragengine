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

#ifndef _DESKINPROPERTYNODEIMAGE_H_
#define _DESKINPROPERTYNODEIMAGE_H_

#include "deSkinPropertyNode.h"
#include "../../../image/deImage.h"
#include "../../../../common/string/decString.h"


/**
 * \brief Skin property image node for constructed texture property.
 */
class DE_DLL_EXPORT deSkinPropertyNodeImage : public deSkinPropertyNode{
private:
	decString pPath;
	deImage::Ref pImage;
	decPoint pRepeat;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constructed property node. */
	deSkinPropertyNodeImage();
	
	/** \brief Clean up constructed property node. */
	virtual ~deSkinPropertyNodeImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Image path or empty string if not set. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set image path or empty path if not set. */
	void SetPath(const char *path);
	
	/** \brief Image or nullptr if not set. */
	inline const deImage::Ref &GetImage() const{ return pImage; }
	
	/** \brief Set image or nullptr if not set. */
	void SetImage(deImage *image);
	
	/** \brief Repeat count. */
	inline const decPoint &GetRepeat() const{ return pRepeat; }
	
	/**
	 * \brief Set repeat count.
	 * \throws deeInvalidParam One or more components of \em count are less than 1.
	 */
	void SetRepeat(const decPoint &count);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits node. */
	virtual void Visit(deSkinPropertyNodeVisitor &visitor);
	/*@}*/
};

#endif
