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

#ifndef _DEDSRENDERABLE_H_
#define _DEDSRENDERABLE_H_

#include "../../../../common/string/decString.h"

class deDSRenderableVisitor;


/**
 * \brief Dynamic Skin Renderable.
 *
 * Renderables are dynamic texture images the developer can write to
 * during runtime. A renderable can define a texture using various
 * methods to allow for large flexibility. Most of the types stem
 * directly from Skin Resources. For all types the same rules apply
 * as the target Skin Property. For all other cases where this
 * renderable is has not a Skin Property as target the rules of the
 * destination object apply. Renderables have an optional name useful
 * to attach multiple renderables to one target object.
 */
class DE_DLL_EXPORT deDSRenderable{
private:
	decString pName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new renderable. */
	deDSRenderable(const char *name);
	
	/** \brief Clean up renderable. */
	virtual ~deDSRenderable();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the renderable. */
	virtual void Visit(deDSRenderableVisitor &visitor);
};

#endif
