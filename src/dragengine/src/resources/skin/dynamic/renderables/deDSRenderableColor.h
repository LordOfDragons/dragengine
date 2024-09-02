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

#ifndef _DEDSRENDERABLECOLOR_H_
#define _DEDSRENDERABLECOLOR_H_

#include "deDSRenderable.h"
#include "../../../../common/math/decMath.h"


/**
 * \brief Renderable Color.
 *
 * The entire texture is defined by one single color for all pixels.
 * Use this type if the renderable is used as the source of a Color
 * or Image Type Skin Property. If used for an Image Type Skin
 * Property the dimension of the texture is undefined but usually
 * very small ( possibly down to 1x1 ).
 */
class DE_DLL_EXPORT deDSRenderableColor : public deDSRenderable{
private:
	decColor pColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new renderable. */
	deDSRenderableColor( const char *name );
	
	/** \brief Clean up renderable. */
	virtual ~deDSRenderableColor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Static color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set static color. */
	void SetColor( const decColor &color );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the renderable. */
	virtual void Visit( deDSRenderableVisitor &visitor );
};

#endif
