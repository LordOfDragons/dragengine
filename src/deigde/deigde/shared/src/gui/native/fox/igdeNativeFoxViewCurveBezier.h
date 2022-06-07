/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _IGDENATIVEFOXVIEWCURVEBEZIER_H_
#define _IGDENATIVEFOXVIEWCURVEBEZIER_H_

#include "foxtoolkit.h"
#include "../../event/igdeAction.h"
#include "../../resources/igdeFontReference.h"

#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>


class igdeViewCurveBezier;
class igdeGuiTheme;
class igdeNativeFoxResizer;


/**
 * \brief FOX toolkit Native list box view.
 */
class igdeNativeFoxViewCurveBezierView : public FXFrame{
	FXDECLARE( igdeNativeFoxViewCurveBezierView )
protected:
	   igdeNativeFoxViewCurveBezierView();
	
public:
	enum eFoxIDs{
		ID_SELF = FXFrame::ID_LAST,
		ID_LAST
	};
	
	enum eSelectionModes{
		esmPoint,
		esmHandle1,
		esmHandle2
	};
	
	enum eDragModes{
		edmNone,
		edmDragCurvePoint,
		edmDragCurveHandle1,
		edmDragCurveHandle2,
		edmDragView,
		edmScaleView,
		edmDragRulerX,
		edmScaleRulerX,
		edmDragRulerY,
		edmScaleRulerY
	};
	
	
	
private:
	igdeViewCurveBezier *pOwner;
	igdeFontReference pFont;
	
	decVector2 pGridMin;
	decVector2 pGridMax;
	decVector2 pGridCenter;
	decVector2 pGridZoom;
	
	int pDirtyGridParams;
	decPoint pWindowCenter;
	decVector2 pGridScale;
	decPoint pGridUnitMode;
	
	int pDotOffset;
	int pDotSize;
	int pHitThreshold;
	int pStickyDragThreshold;
	
	eDragModes pDragMode;
	decPoint pDragOrg;
	decPoint pDragCur;
	decVector pOldOrientation;
	
	bool pStickyDragging;
	decPoint pDragPoint;
	bool pUseXLowerLimit;
	bool pUseXUpperLimit;
	float pDragXLowerLimit;
	float pDragXUpperLimit;
	decVector2 pDragHandle1Offset;
	decVector2 pDragHandle2Offset;
	decCurveBezierPoint pDragOrgPoint;
	decVector2 pDragOldCenter;
	decVector2 pDragOldZoom;
	
	bool pDragShift;
	bool pDragControl;
	decPoint pRulerSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxViewCurveBezierView( igdeViewCurveBezier &owner, FXComposite *parent,
		int layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxViewCurveBezierView();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateEnabled();
	virtual void UpdateDefaultSize();
	virtual void UpdateCurve();
	virtual void UpdateSelectedPoint();
	virtual void UpdateClamp();
	
	inline igdeViewCurveBezier &GetOwner() const{ return *pOwner; }
	
	void SetGridZoom( const decVector2 &zoom );
	virtual void ResetView();
	virtual void FitViewToCurve();
	
	int IndexOfPointAt( int x, int y, eSelectionModes mode ) const;
	
	void DrawGrid( FXDC &dc );
	void DrawRulers( FXDC &dc );
	void DrawCurve( FXDC &dc );
	void DrawCurveHandles( FXDC &dc );
	void ClampHandles( const decVector2 &p1, decVector2 &h1,
		decVector2 &h2, const decVector2 &p2 ) const;
	void InternalDrawCurve( FXDC &dc, const decVector2 &p1,
		const decVector2 &p2, const decVector2 &p3, const decVector2 &p4 );
	
	inline int CurveXToWindow( float curveCoord ) const{
		return pWindowCenter.x + ( int )( ( curveCoord - pGridCenter.x ) * pGridScale.x );
	}
	inline int CurveYToWindow( float curveCoord ) const{
		return pWindowCenter.y - ( int )( ( curveCoord - pGridCenter.y ) * pGridScale.y );
	}
	inline float WindowXToCurve( int windowCoord ) const{
		return ( float )( windowCoord - pWindowCenter.x ) / pGridScale.x + pGridCenter.x;
	}
	inline float WindowYToCurve( int windowCoord ) const{
		return ( float )( pWindowCenter.y - windowCoord ) / pGridScale.y + pGridCenter.y;
	}
	
	void DisplayPopupView( const decPoint &position );
	void UpdateParameters();
	
	
	static int ViewCurveBezierFlags( const igdeViewCurveBezier &owner );
	static igdeFont *ViewCurveBezierFont( const igdeViewCurveBezier &owner, const igdeGuiTheme &guitheme );
	
	virtual FXint getDefaultWidth();
	virtual FXint getDefaultHeight();
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onResize( FXObject*, FXSelector, void* );
	long onPaint( FXObject*, FXSelector, void* );
	long onLeftMouseDown( FXObject*, FXSelector, void* );
	long onLeftMouseUp( FXObject*, FXSelector, void* );
	long onMouseMove( FXObject*, FXSelector, void* );
	long onMouseWheel( FXObject*, FXSelector, void* );
	long onRightMouseDown( FXObject*, FXSelector, void* );
	long onRightMouseUp( FXObject*, FXSelector, void* );
	long onMiddleMouseDown( FXObject*, FXSelector, void* );
	long onMiddleMouseUp( FXObject*, FXSelector, void* );
	/*@}*/
};


/**
 * \brief FOX toolkit Native list box.
 * 
 * Wraps igdeNativeFoxViewCurveBezierView with resizing support.
 */
class igdeNativeFoxViewCurveBezier : public FXVerticalFrame{
	FXDECLARE( igdeNativeFoxViewCurveBezier )
protected:
	   igdeNativeFoxViewCurveBezier();
	
	
public:
	enum eFoxIDs{
		ID_VIEW_CURVE_BEZIER = FXVerticalFrame::ID_LAST,
		ID_RESIZER,
		ID_LAST
	};
	
	
private:
	igdeNativeFoxViewCurveBezierView *pView;
	igdeNativeFoxResizer *pResizer;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxViewCurveBezier( igdeViewCurveBezier &owner, FXComposite *parent,
		const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxViewCurveBezier();
	
	/** \brief Create native widget. */
	static igdeNativeFoxViewCurveBezier* CreateNativeWidget( igdeViewCurveBezier &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief View curve bezier. */
	inline igdeNativeFoxViewCurveBezierView &GetView() const{ return *pView; }
	
	/** \brief Resizer or \em NULL. */
	inline igdeNativeFoxResizer *GetResizser() const{ return pResizer; }
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onResizerDrag( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxViewCurveBezier igdeNativeViewCurveBezier;

#endif
