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

#ifndef _IGDENATIVENULLNVNODE_H_
#define _IGDENATIVENULLNVNODE_H_

#include <dragengine/common/math/decMath.h>

class igdeNVNode;


/**
 * Null NodeView Node.
 */
class igdeNativeNullNVNode{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullNVNode();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullNVNode();
	
	/** \brief Create native widget. */
	static igdeNativeNullNVNode* CreateNativeWidget( igdeNVNode &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Update title from owner. */
	virtual void UpdateTitle();
	
	/** \brief Update description from owner. */
	virtual void UpdateDescription();
	
	/** \brief Update enabled from owner. */
	virtual void UpdateEnabled();
	
	/** \brief Update active from owner. */
	virtual void UpdateActive();
	
	/** \brief Update color from owner. */
	virtual void UpdateColors();
	
	/** \brief Update position from owner. */
	virtual void UpdatePosition();
	
	/** \brief Resize node to fit size to content. */
	virtual void FitSizeToContent();
	
	/** \brief Get Size. */
	virtual decPoint GetSize();
	/*@}*/
};

typedef igdeNativeNullNVNode igdeNativeNVNode;

#endif
