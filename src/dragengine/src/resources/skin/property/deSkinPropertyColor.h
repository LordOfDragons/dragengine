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

#ifndef _DESKINPROPERTYCOLOR_H_
#define _DESKINPROPERTYCOLOR_H_

#include "deSkinProperty.h"
#include "../../../common/math/decMath.h"


/**
 * \brief Skin Texture Property Color.
 *
 * Defines a color property of a skin texture.
 */
class DE_DLL_EXPORT deSkinPropertyColor : public deSkinProperty{
private:
	decColor pColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create new named skin property color with the given type.
	 * \param type Type of the skin property obtained from adding a property
	 * name to the texture property map object held by the engine.
	 */
	deSkinPropertyColor(const char *type);
	
	/** \brief Clean up skin property color. */
	virtual ~deSkinPropertyColor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Static color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set static color. */
	void SetColor(const decColor &color);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit property. */
	virtual void Visit(deSkinPropertyVisitor &visitor);
	/*@}*/
};

#endif
