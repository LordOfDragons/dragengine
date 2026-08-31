/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDENATIBEOSNVBOARD_H_
#define _IGDENATIBEOSNVBOARD_H_

#include <interface/View.h>
#include "../../../nodeview/igdeNVBoard.h"
#include "../../../nodeview/igdeNVLink.h"

class igdeNVBoard;
class igdeNVLink;
class igdeGuiTheme;
class igdeNativeBeosNVSlot;


/**
 * \brief BeOS native NodeView board widget.
 */
class igdeNativeBeosNVBoard : public BView, public igdeNVBoard::cNativeNVBoard{
private:
	igdeNVBoard *pOwner;
	
	igdeNativeBeosNVSlot *pCreateLinkSource;
	igdeNativeBeosNVSlot *pCreateLinkTarget;
	decPoint pCreateLinkPosition;
	
	igdeNVLink::Ref pHoverLink;
	
	decPoint pDragBoard;
	bool pIsDragBoard;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeBeosNVBoard(igdeNVBoard &owner, BView *parent, const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	~igdeNativeBeosNVBoard() override;
	
	/** \brief Create native widget. */
	static igdeNativeBeosNVBoard* CreateNativeWidget(igdeNVBoard &owner);
	
	/** \brief Post create native widget. */
	void PostCreateNativeWidget() override;
	
	/** \brief Destroy native widget. */
	void DestroyNativeWidget() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Owner. */
	inline igdeNVBoard &GetOwner() const{ return *pOwner; }
	
	
	
	void UpdateEnabled() override;
	void UpdateColors() override;
	void UpdateNodes() override;
	void UpdateLinks() override;
	void UpdateOffset() override;
	decPoint GetSize() override;
	igdeNVLink *ClosestLinkNear(const decPoint &position, float range) const override;
	const igdeNVLink::Ref &GetHoverLink() const override;
	
	
	
	/** \brief Begin create link drag. */
	void BeginCreateLink(igdeNativeBeosNVSlot *source);
	
	/** \brief Create link source slot. */
	inline igdeNativeBeosNVSlot *GetCreateLinkSource() const{ return pCreateLinkSource; }
	
	/** \brief Set create link position. */
	void SetCreateLinkPosition(const decPoint &position);
	
	/** \brief Create link target slot. */
	inline igdeNativeBeosNVSlot *GetCreateLinkTarget() const{ return pCreateLinkTarget; }
	
	/** \brief Set create link target. */
	void SetCreateLinkTarget(igdeNativeBeosNVSlot *target);
	
	/** \brief Finish create link. */
	void FinishCreateLink();
	
	/** \brief Set hover link. */
	void SetHoverLink(igdeNVLink *link);
	/*@}*/
	
	
	
	/** \name BView overrides */
	/*@{*/
	void Draw(BRect updateRect) override;
	void MouseDown(BPoint where) override;
	void MouseMoved(BPoint where, uint32 code, const BMessage *message) override;
	void MouseUp(BPoint where) override;
	void FrameResized(float width, float height) override;
	/*@}*/
	
	
	
	/** \name Drawing helpers */
	/*@{*/
	void DrawLinks() const;
	void DrawCreateLink() const;
	
	void DrawBezier(rgb_color color, const decPoint &p1, const decPoint &p2,
		const decPoint &p3, const decPoint &p4) const;
	
	void DrawBezierLine(const decPoint &p1, const decPoint &p2,
		const decPoint &p3, const decPoint &p4) const;
	
	void InternalDrawBezier(const decVector2 &p1, const decVector2 &p2,
		const decVector2 &p3, const decVector2 &p4) const;
	
	float PointBezierDistance(const decPoint &p, const decPoint &bp1, const decPoint &bp2,
		const decPoint &bp3, const decPoint &bp4) const;
	
	float PointBezierDistance(const decVector2 &bp, const decVector2 &bp1, const decVector2 &bp2,
		const decVector2 &bp3, const decVector2 &bp4) const;
	/*@}*/
};

typedef igdeNativeBeosNVBoard igdeNativeNVBoard;

#endif
