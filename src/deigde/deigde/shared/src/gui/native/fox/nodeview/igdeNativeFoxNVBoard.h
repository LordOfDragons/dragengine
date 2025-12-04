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

#ifndef _IGDENATIVEFOXNVBOARD_H_
#define _IGDENATIVEFOXNVBOARD_H_

#include "../foxtoolkit.h"
#include "../../../resources/igdeFont::Ref.h"
#include "../../../nodeview/igdeNVLink::Ref.h"


class igdeNVBoard;
class igdeNVLink;
class igdeGuiTheme;
class igdeNativeFoxNVSlot;


/**
 * \brief FOX toolkit Native NodeView Board.
 */
class igdeNativeFoxNVBoard : public FXPacker{
	FXDECLARE( igdeNativeFoxNVBoard )
protected:
	   igdeNativeFoxNVBoard();
	
public:
	enum eFoxIDs{
		ID_SELF = FXPacker::ID_LAST,
		ID_LAST
	};
	
private:
	igdeNVBoard *pOwner;
	FXImage *pDoubleBuffer;
	
	igdeNativeFoxNVSlot *pCreateLinkSource;
	igdeNativeFoxNVSlot *pCreateLinkTarget;
	decPoint pCreateLinkPosition;
	
	igdeNVLink::Ref pHoverLink;
	
	decPoint pDragBoard;
	bool pIsDragBoard;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxNVBoard( igdeNVBoard &owner, FXComposite *parent, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxNVBoard();
	
	/** \brief Create native widget. */
	static igdeNativeFoxNVBoard* CreateNativeWidget( igdeNVBoard &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual FXbool canFocus() const;
	
	
	
	/** \brief Owner. */
	inline igdeNVBoard &GetOwner() const{ return *pOwner; }
	
	
	
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
	void BeginCreateLink( igdeNativeFoxNVSlot *source );
	
	/** \brief Link drag and drop source or NULL. */
	inline igdeNativeFoxNVSlot *GetCreateLinkSource() const{ return pCreateLinkSource; }
	
	/** \brief Set create link drag and drop position. */
	void SetCreateLinkPosition( const decPoint &position );
	
	/** \brief Create link drag and drop target or NULL. */
	inline igdeNativeFoxNVSlot *GetCreateLinkTarget() const{ return pCreateLinkTarget; }
	
	/** \brief Set create link drag and drop target or NULL. */
	void SetCreateLinkTarget( igdeNativeFoxNVSlot *target );
	
	/** \brief Finish create link drag and drop. */
	void FinishCreateLink();
	
	/** \brief Link closest to position inside range in board coordinate system or NULL. */
	igdeNVLink *ClosestLinkNear( const decPoint &position, float range ) const;
	
	/** \brief Hover link or NULL. */
	inline igdeNVLink *GetHoverLink() const{ return pHoverLink; }
	
	/** \brief Set hover link. */
	void SetHoverLink( igdeNVLink *link );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onPaint( FXObject*, FXSelector, void* );
	long onResize( FXObject*, FXSelector, void* );
	long onChildLayoutFlags( FXObject*, FXSelector, void* );
	long onLeftMousePress( FXObject*, FXSelector, void* );
	long onLeftMouseRelease( FXObject*, FXSelector, void* );
	long onMouseMoved( FXObject*, FXSelector, void* );
	long onRightMousePress( FXObject*, FXSelector, void* );
	long onRightMouseRelease( FXObject*, FXSelector, void* );
	
	
	
	void DrawLinks( FXDC &dc ) const;
	void DrawCreateLink( FXDC &dc ) const;
	
	void DrawBezier( FXDC &dc, FXColor color, const decPoint &p1, const decPoint &p2,
		const decPoint &p3, const decPoint &p4 ) const;
	
	void DrawBezier( FXDC &dc, const decPoint &p1, const decPoint &p2, const decPoint &p3,
		const decPoint &p4 ) const;
	
	void InternalDrawBezier( FXDC &dc, const decVector2 &p1, const decVector2 &p2,
		const decVector2 &p3, const decVector2 &p4 ) const;
	
	bool LineIntersectsBezier( const decPoint &lp1, const decPoint &lp2, const decPoint &bp1,
		const decPoint &bp2, const decPoint &bp3, const decPoint &bp4 ) const;
	
	bool InternalLineIntersectsBezier( const decVector2 &ldir, float llen, const decVector2 &lpos,
		const decVector2 &bp1, const decVector2 &bp2, const decVector2 &bp3, const decVector2 &bp4 ) const;
	
	float PointBezierDistance( const decPoint &p, const decPoint &bp1,
		const decPoint &bp2, const decPoint &bp3, const decPoint &bp4 ) const;
	
	float PointBezierDistance( const decVector2 &bp, const decVector2 &bp1, const decVector2 &bp2,
		const decVector2 &bp3, const decVector2 &bp4 ) const;
	/*@}*/
};

typedef igdeNativeFoxNVBoard igdeNativeNVBoard;

#endif
