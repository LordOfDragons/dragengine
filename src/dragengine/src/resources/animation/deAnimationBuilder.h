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

#ifndef _DEANIMATIONBUILDER_H_
#define _DEANIMATIONBUILDER_H_

#include "../../dragengine_export.h"

class deAnimation;


/**
 * \brief Animation Builder.
 *
 * The animation builder is provided for applications requiring to produce
 * animation resources not from a file but from data somewhere in memory.
 * Usually used by editors to create animation resources from an in-editor
 * representation of a animation.
 */
class DE_DLL_EXPORT deAnimationBuilder{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animation builder. */
	deAnimationBuilder();
	
	/** \brief Clean up animation builder. */
	virtual ~deAnimationBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Build animation into the provided animation object. */
	virtual void BuildAnimation(deAnimation *animation) = 0;
	/*@}*/
};

#endif
