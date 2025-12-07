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

#ifndef _DENAVIGATIONSPACEEDGE_H_
#define _DENAVIGATIONSPACEEDGE_H_

#include "../../../dragengine_export.h"


/**
 * \brief Navigation Space Edge.
 */
class DE_DLL_EXPORT deNavigationSpaceEdge{
private:
	unsigned short pVertex1;
	unsigned short pVertex2;
	unsigned short pType1;
	unsigned short pType2;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new navigation space face. */
	deNavigationSpaceEdge();
	
	/** \brief Clean up navigation space face. */
	~deNavigationSpaceEdge();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Index of the first vertex. */
	inline unsigned short GetVertex1() const{return pVertex1;}
	
	/** \brief Set index of the first vertex. */
	void SetVertex1(unsigned short vertex);
	
	/** \brief Index of the second vertex. */
	inline unsigned short GetVertex2() const{return pVertex2;}
	
	/** \brief Set index of the second vertex. */
	void SetVertex2(unsigned short vertex);
	
	/** \brief Type to use for this edge to travel from the first to the second vertex. */
	inline unsigned short GetType1() const{return pType1;}
	
	/** \brief Set type to use for this edge to travel from the first to the second vertex. */
	void SetType1(unsigned short type);
	
	/** \brief Type to use for this edge to travel from the second to the second first. */
	inline unsigned short GetType2() const{return pType2;}
	
	/** \brief Set type to use for this edge to travel from the second to the second first. */
	void SetType2(unsigned short type);
	/*@}*/
};

#endif
