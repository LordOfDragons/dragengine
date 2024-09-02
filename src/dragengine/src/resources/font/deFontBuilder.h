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

#ifndef _DEFONTBUILDER_H_
#define _DEFONTBUILDER_H_

#include "../../dragengine_export.h"

class deFont;


/**
 * \brief Font Builder.
 *
 * The font builder is provided for applications requiring to produce
 * font resources not from a file but from data somewhere in memory.
 * Usually used by editors to create font resources from an in-editor
 * representation of a font.
 */
class DE_DLL_EXPORT deFontBuilder{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new font builder. */
	deFontBuilder();
	
	/** \brief Clean up font builder. */
	virtual ~deFontBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Build font into the provided font object. */
	virtual void BuildFont( deFont *font ) = 0;
	/*@}*/
};

#endif
