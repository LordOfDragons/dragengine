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

#ifndef _DENAVIGATIONSPACECORNER_H_
#define _DENAVIGATIONSPACECORNER_H_

#include "../../../dragengine_export.h"


/**
 * \brief Navigation Space Corner.
 */
class DE_DLL_EXPORT deNavigationSpaceCorner{
private:
	unsigned short pVertex;
	unsigned short pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new navigation space corner. */
	deNavigationSpaceCorner();
	
	/** \brief Clean up navigation space corner. */
	~deNavigationSpaceCorner();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Index of the vertex. */
	inline unsigned short GetVertex() const{ return pVertex; }
	
	/** \brief Set index of the vertex. */
	void SetVertex(unsigned short vertex);
	
	/** \brief Type to use for crossing the face edge starting with this corner. */
	inline unsigned short GetType() const{ return pType; }
	
	/** \brief Set type to use for crossing the face edge starting with this corner. */
	void SetType(unsigned short type);
	/*@}*/
};

#endif
