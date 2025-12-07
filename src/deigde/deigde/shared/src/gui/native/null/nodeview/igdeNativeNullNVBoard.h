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

#ifndef _IGDENATIVENULLNVBOARD_H_
#define _IGDENATIVENULLNVBOARD_H_

#include <dragengine/common/math/decMath.h>

class igdeNVBoard;
class igdeNVLink;
class igdeNativeNullNVSlot;


/**
 * Null NodeView Board.
 */
class igdeNativeNullNVBoard{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullNVBoard();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullNVBoard();
	
	/** \brief Create native widget. */
	static igdeNativeNullNVBoard* CreateNativeWidget(igdeNVBoard &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Update enabled. */
	void UpdateEnabled();
	
	/** \brief Update color from owner. */
	void UpdateColors();
	
	/** \brief Update nodes from owner. */
	void UpdateNodes();
	
	/** \brief Update links from owner. */
	void UpdateLinks();
	
	/** \brief Update offset from owner. */
	void UpdateOffset();
	
	/** \brief Get Size. */
	virtual decPoint GetSize();
	
	
	
	/** \brief Begin create link drag and drop. */
	void BeginCreateLink(igdeNativeNullNVSlot *source);
	
	/** \brief Link drag and drop source or NULL. */
	inline igdeNativeNullNVSlot *GetCreateLinkSource() const{return nullptr;}
	
	/** \brief Set create link drag and drop position. */
	void SetCreateLinkPosition(const decPoint &position);
	
	/** \brief Create link drag and drop target or NULL. */
	inline igdeNativeNullNVSlot *GetCreateLinkTarget() const{return nullptr;}
	
	/** \brief Set create link drag and drop target or NULL. */
	void SetCreateLinkTarget(igdeNativeNullNVSlot *target);
	
	/** \brief Finish create link drag and drop. */
	void FinishCreateLink();
	
	/** \brief Link closest to position inside range in board coordinate system or NULL. */
	igdeNVLink *ClosestLinkNear(const decPoint &position, float range) const;
	
	/** \brief Hover link or NULL. */
	inline igdeNVLink *GetHoverLink() const{return nullptr;}
	
	/** \brief Set hover link. */
	void SetHoverLink(igdeNVLink *link);
	/*@}*/
};

typedef igdeNativeNullNVBoard igdeNativeNVBoard;

#endif
