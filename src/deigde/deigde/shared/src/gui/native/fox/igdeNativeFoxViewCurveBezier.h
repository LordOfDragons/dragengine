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
