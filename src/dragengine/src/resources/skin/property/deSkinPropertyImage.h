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

#ifndef _DESKINPROPERTYIMAGE_H_
#define _DESKINPROPERTYIMAGE_H_

#include "deSkinProperty.h"
#include "../../image/deImage.h"
#include "../../../common/string/decString.h"


/**
 * \brief Skin texture image property.
 */
class DE_DLL_EXPORT deSkinPropertyImage : public deSkinProperty{
private:
	decString pPath;
	deImage::Ref pImage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin image property. */
	deSkinPropertyImage(const char *type);
	
	/** \brief Clean up skin property. */
	virtual ~deSkinPropertyImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Image path. */
	inline const decString &GetPath() const{return pPath;}
	
	/** \brief Set image path. */
	void SetPath(const char *path);
	
	/** \brief Image. */
	inline deImage *GetImage() const{return pImage;}
	
	/** \brief Set image. */
	void SetImage(deImage *image);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit property. */
	virtual void Visit(deSkinPropertyVisitor &visitor);
	/*@}*/
};

#endif
