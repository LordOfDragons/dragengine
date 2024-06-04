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

#ifdef IGDE_TOOLKIT_FOX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "igdeNativeFoxResizer.h"
#include "igdeNativeFoxViewCurveBezier.h"
#include "../../igdeUIHelper.h"
#include "../../curveedit/igdeViewCurveBezier.h"
#include "../../curveedit/igdeDialogCurveBezierClamp.h"
#include "../../curveedit/igdeDialogCurveBezierCoord.h"
#include "../../dialog/igdeDialogReference.h"
#include "../../menu/igdeMenuCascade.h"
#include "../../menu/igdeMenuCascadeReference.h"
#include "../../resources/igdeFont.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define CE_MIN_DIST		0.001f

struct sScaleModes{
	const char *rulerFormatString;
	float unitSize;
};

#define SCALE_MODE_COUNT 19
static sScaleModes vScaleModes[ SCALE_MODE_COUNT ] = {
	{ "%.3f", 0.001f }, { "%.3f", 0.002f }, { "%.3f", 0.005f },
	{ "%.2f", 0.01f }, { "%.2f", 0.02f }, { "%.2f", 0.05f },
	{ "%.1f", 0.1f }, { "%.1f", 0.2f }, { "%.1f", 0.5f },
	{ "%.0f", 1.0f }, { "%.0f", 2.0f }, { "%.0f", 5.0f },
	{ "%.0f", 10.0f }, { "%.0f", 20.0f }, { "%.0f", 50.0f },
	{ "%.0f", 100.0f }, { "%.0f", 200.0f }, { "%.0f", 500.0f },
	{ "%.0f", 1000.0f }
};



// Event map
//////////////

FXDEFMAP( igdeNativeFoxViewCurveBezierView ) igdeNativeFoxViewCurveBezierViewMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxViewCurveBezierView::onResize ),
	FXMAPFUNC( SEL_PAINT, 0, igdeNativeFoxViewCurveBezierView::onPaint ),
	FXMAPFUNC( SEL_LEFTBUTTONPRESS, 0, igdeNativeFoxViewCurveBezierView::onLeftMouseDown ),
	FXMAPFUNC( SEL_LEFTBUTTONRELEASE, 0, igdeNativeFoxViewCurveBezierView::onLeftMouseUp ),
	FXMAPFUNC( SEL_MOTION, 0, igdeNativeFoxViewCurveBezierView::onMouseMove ),
	FXMAPFUNC( SEL_MOUSEWHEEL, 0, igdeNativeFoxViewCurveBezierView::onMouseWheel ),
	FXMAPFUNC( SEL_RIGHTBUTTONPRESS, 0, igdeNativeFoxViewCurveBezierView::onRightMouseDown ),
	FXMAPFUNC( SEL_RIGHTBUTTONRELEASE, 0, igdeNativeFoxViewCurveBezierView::onRightMouseUp ),
	FXMAPFUNC( SEL_MIDDLEBUTTONPRESS, 0, igdeNativeFoxViewCurveBezierView::onMiddleMouseDown ),
	FXMAPFUNC( SEL_MIDDLEBUTTONRELEASE, 0, igdeNativeFoxViewCurveBezierView::onMiddleMouseUp ),
};

FXIMPLEMENT( igdeNativeFoxViewCurveBezierView, FXFrame,
	igdeNativeFoxViewCurveBezierViewMap, ARRAYNUMBER( igdeNativeFoxViewCurveBezierViewMap ) )



// Class igdeNativeFoxViewCurveBezier
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxViewCurveBezierView::igdeNativeFoxViewCurveBezierView(){ }

igdeNativeFoxViewCurveBezierView::igdeNativeFoxViewCurveBezierView( igdeViewCurveBezier &powner,
	FXComposite *pparent, int layoutFlags, const igdeGuiTheme &guitheme ) :
FXFrame( pparent, layoutFlags | ViewCurveBezierFlags( powner ), 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &powner ),
pFont( ViewCurveBezierFont( powner, guitheme ) ),

pGridMin( 0.0f, 0.0f ),
pGridMax( 1.0f, 1.0f ),
pGridCenter( 0.5f, 0.5f ),
pGridZoom( 1.0f, 1.0f ),

pDirtyGridParams( true ),
pGridScale( 1.0f, 1.0f ),
pGridUnitMode( 6, 7 ),

pDotOffset( 2 ),
pDotSize( 5 ),
pHitThreshold( 3 ),
pStickyDragThreshold( 4 ),

pDragMode( edmNone ),

pStickyDragging( false ),
pUseXLowerLimit( false ),
pUseXUpperLimit( false ),
pDragXLowerLimit( false ),
pDragXUpperLimit( false ),

pDragShift( false ),
pDragControl( false ),

pRulerSize( pFont->TextSize( "88.8" ) + decPoint( 4, 2 ) )
{
	if( ! pOwner->GetVisible() ){
		hide();
	}
	
	UpdateEnabled();
}

igdeNativeFoxViewCurveBezierView::~igdeNativeFoxViewCurveBezierView(){
}




// Management
///////////////

void igdeNativeFoxViewCurveBezierView::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxViewCurveBezierView::UpdateDefaultSize(){
	recalc();
	update();
}

void igdeNativeFoxViewCurveBezierView::UpdateCurve(){
	update();
}

void igdeNativeFoxViewCurveBezierView::UpdateSelectedPoint(){
	update();
}

void igdeNativeFoxViewCurveBezierView::UpdateClamp(){
	update();
}

void igdeNativeFoxViewCurveBezierView::SetGridZoom( const decVector2 &zoom ){
	pGridZoom = zoom.Largest( decVector2( 0.001f, 0.001f ) ).Smallest( decVector2( 1000.0f, 1000.0f ) );
	pDirtyGridParams = true;
}

void igdeNativeFoxViewCurveBezierView::ResetView(){
	const decVector2 range( decVector2( 0.01f, 0.01f ).Largest( pOwner->GetClampMax() - pOwner->GetClampMin() ) );
	const decVector2 center( ( pOwner->GetClampMin() + pOwner->GetClampMax() ) * 0.5f );
	
	pGridCenter = center;
	pDirtyGridParams = true;
	
	const decPoint gridSize( pGridMax - pGridMin );
	SetGridZoom( decVector2( gridSize.x / range.x, gridSize.y / range.y ) );
	
	update();
}

void igdeNativeFoxViewCurveBezierView::FitViewToCurve(){
	const decCurveBezier &curve = pOwner->GetCurve();
	int p, pointCount = curve.GetPointCount();
	decVector2 minExtend, maxExtend( 1.0f, 1.0f );
	
	// determine the extends of the curve
	if( pointCount > 0 ){
		minExtend = maxExtend = curve.GetPointAt( 0 ).GetPoint();
		
		for( p=1; p<pointCount; p++ ){
			const decCurveBezierPoint &point = curve.GetPointAt( p );
			minExtend.SetSmallest( point.GetPoint() );
			maxExtend.SetLargest( point.GetPoint() );
		}
		
		if( maxExtend.x - minExtend.x < 0.001f ){
			const float center = ( minExtend.x + maxExtend.x ) * 0.5f;
			minExtend.x = center - 0.0005f;
			maxExtend.x = center + 0.0005f;
		}
		
		if( maxExtend.y - minExtend.y < 0.001f ){
			const float center = ( minExtend.y + maxExtend.y ) * 0.5f;
			minExtend.y = center - 0.0005f;
			maxExtend.y = center + 0.0005f;
		}
	}
	
	// ensure the view is not shrunk too small
	const decVector2 enlarge( ( decVector2( 0.1f, 0.1f ) - ( maxExtend - minExtend ) / 2 ).Largest( decVector2() ) );
	minExtend -= enlarge;
	maxExtend += enlarge;
	
	// set the origin and scaling to fit the curve
	pGridCenter = ( minExtend + maxExtend ) * 0.5f;
	pDirtyGridParams = true;
	
	const decVector2 gridSize( pGridMax - pGridMin );
	const decVector2 extendSize( maxExtend - minExtend );
	SetGridZoom( decVector2( gridSize.x / extendSize.x, gridSize.y / extendSize.y ) );
	
	update();
}


int igdeNativeFoxViewCurveBezierView::IndexOfPointAt( int x, int y, eSelectionModes mode ) const{
	const decCurveBezier &curve = pOwner->GetCurve();
	int pointCount = curve.GetPointCount();
	int p, ptx, pty;
	
	if( mode == esmPoint || mode == esmHandle1 || mode == esmHandle2 ){
		for( p=0; p<pointCount; p++ ){
			const decCurveBezierPoint &point = curve.GetPointAt( p );
			
			if( mode == esmPoint ){
				const decVector2 &pt = point.GetPoint();
				ptx = CurveXToWindow( pt.x );
				pty = CurveYToWindow( pt.y );
				
			}else if( mode == esmHandle1 ){
				const decVector2 &pt = point.GetHandle1();
				ptx = CurveXToWindow( pt.x );
				pty = CurveYToWindow( pt.y );
				
			}else{ //esmHandle2
				const decVector2 &pt = point.GetHandle2();
				ptx = CurveXToWindow( pt.x );
				pty = CurveYToWindow( pt.y );
			}
			
			if( abs( ptx - x ) <= pHitThreshold && abs( pty - y ) <= pHitThreshold ){
				return p;
			}
		}
	}
	
	return -1;
}


void igdeNativeFoxViewCurveBezierView::DrawGrid( FXDC &dc ){
	FXColor clrBackground;
	FXColor clrGrid;
	FXColor clrGridSub;
	const float unitSizeX = vScaleModes[ pGridUnitMode.x ].unitSize;
	const float unitSizeY = vScaleModes[ pGridUnitMode.y ].unitSize;
	float position;
	int x, y;
	
	// colors are different if we are enabled or disabled
	if( pOwner->GetEnabled() ){
		clrBackground = FXRGB( 255, 255, 255 );
		clrGrid = FXRGB( 92, 92, 92 );
		clrGridSub = FXRGB( 192, 192, 192 );
		
	}else{
		clrBackground = FXRGB( 255, 255, 255 ); //getApp()->getBaseColor();
		clrGrid = FXRGB( 92, 92, 92 );
		clrGridSub = FXRGB( 192, 192, 192 );
	}
	
	// background
	dc.setForeground( clrBackground );
	dc.fillRectangle( pRulerSize.x, 0, width - pRulerSize.x, height - pRulerSize.y );
	
	// grid lines
	dc.setForeground( clrGridSub );
	
	position = ceilf( WindowXToCurve( pRulerSize.x ) / unitSizeX ) * unitSizeX;
	while( true ){
		x = CurveXToWindow( position );
		if( x >= width ) break;
		dc.drawLine( x, 0, x, height - pRulerSize.y );
		position += unitSizeX;
	}
	
	position = ceilf( WindowYToCurve( height - pRulerSize.y ) / unitSizeY ) * unitSizeY;
	while( true ){
		y = CurveYToWindow( position );
		if( y < 0 ) break;
		dc.drawLine( pRulerSize.x, y, width, y );
		position += unitSizeY;
	}
	
	// origin lines
	dc.setForeground( clrGrid );
	x = CurveXToWindow( 0.0f );
	y = CurveYToWindow( 0.0f );
	dc.drawLine( pRulerSize.x, y, width, y );
	dc.drawLine( x, 0, x, height - pRulerSize.y );
}

void igdeNativeFoxViewCurveBezierView::DrawRulers( FXDC &dc ){
	const char * const unitFormatX = vScaleModes[ pGridUnitMode.x ].rulerFormatString;
	const char * const unitFormatY = vScaleModes[ pGridUnitMode.y ].rulerFormatString;
	const float unitSizeX = vScaleModes[ pGridUnitMode.x ].unitSize;
	const float unitSizeY = vScaleModes[ pGridUnitMode.y ].unitSize;
	const FXColor clrRulerBg = getApp()->getBaseColor();
	const FXColor clrRulerFont = FXRGB( 0, 0, 0 );
	float position;
	FXString text;
	int tw, th;
	int x, y;
	
	// background
	dc.setForeground( clrRulerBg );
	dc.fillRectangle( 0, 0, pRulerSize.x, height );
	dc.fillRectangle( pRulerSize.x, height - pRulerSize.y, width - pRulerSize.x, pRulerSize.y );
	
	// markers
	FXFont * const font = ( FXFont* )pFont->GetNativeFont();
	dc.setFont( font );
	dc.setForeground( clrRulerFont );
	
	position = ceilf( WindowYToCurve( height - pRulerSize.y ) / unitSizeY ) * unitSizeY;
	while( true ){
		y = CurveYToWindow( position );
		if( y < 0 ) break;
		if( fabsf( position ) < 1e-5f ) position = 0.0f; // avoid -0.0 output
		text.format( unitFormatY, position );
		th = font->getTextHeight( text );
		dc.drawText( 2, y + th / 2 + 1, text );
		position += unitSizeY;
	}
	
	position = ceilf( WindowXToCurve( pRulerSize.x ) / unitSizeX ) * unitSizeX;
	while( true ){
		x = CurveXToWindow( position );
		if( x >= width ) break;
		if( fabsf( position ) < 1e-5f ) position = 0.0f; // avoid -0.0 output
		text.format( unitFormatX, position );
		tw = font->getTextWidth( text );
		dc.drawText( x - tw / 2, height - 2, text );
		position += unitSizeX;
	}
}

void igdeNativeFoxViewCurveBezierView::DrawCurve( FXDC &dc ){
	const decCurveBezier &curve = pOwner->GetCurve();
	int p, pointCount = curve.GetPointCount();
	FXColor clrLine = FXRGB( 0, 96, 255 );
	int x, y;
	
	dc.setForeground( clrLine );
	
	if( pointCount == 0 ){
		y = CurveYToWindow( 0.0f );
		dc.drawLine( pRulerSize.x, y, width, y );
		
	}else if( pointCount == 1 ){
		y = CurveYToWindow( curve.GetPointAt( 0 ).GetPoint().y );
		dc.drawLine( pRulerSize.x, y, pRulerSize.x, y );
		
	}else{
		int x2, y2;
		
		if( curve.GetInterpolationMode() == decCurveBezier::eimConstant ){
			for( p=1; p<pointCount; p++ ){
				const decCurveBezierPoint &p1 = curve.GetPointAt( p - 1 );
				const decCurveBezierPoint &p2 = curve.GetPointAt( p );
				
				x = CurveXToWindow( p1.GetPoint().x );
				y = CurveYToWindow( p1.GetPoint().y );
				
				x2 = CurveXToWindow( p2.GetPoint().x );
				y2 = CurveYToWindow( p2.GetPoint().y );
				
				if( p == 1 ){
					dc.drawLine( pRulerSize.x, y, x2, y );
					
				}else{
					dc.drawLine( x, y, x2, y );
				}
				
				if( p == pointCount - 1 ){
					dc.drawLine( x2, y2, width, y2 );
				}
			}
			
		}else if( curve.GetInterpolationMode() == decCurveBezier::eimLinear ){
			for( p=1; p<pointCount; p++ ){
				const decCurveBezierPoint &p1 = curve.GetPointAt( p - 1 );
				const decCurveBezierPoint &p2 = curve.GetPointAt( p );
				
				x = CurveXToWindow( p1.GetPoint().x );
				y = CurveYToWindow( p1.GetPoint().y );
				
				x2 = CurveXToWindow( p2.GetPoint().x );
				y2 = CurveYToWindow( p2.GetPoint().y );
				
				if( p == 1 ){
					dc.drawLine( pRulerSize.x, y, x, y );
				}
				
				if( x < width && x2 >= pRulerSize.x ){
					dc.drawLine( x, y, x2, y2 );
				}
				
				if( p == pointCount - 1 ){
					dc.drawLine( x2, y2, width, y2 );
				}
			}
			
		}else{ // curve.GetInterpolationMode() == decCurveBezier::eimBezier
			decVector2 bp1, bp2, bp3, bp4;
			int h1x, h1y, h2x, h2y;
			
			for( p=1; p<pointCount; p++ ){
				const decCurveBezierPoint &p1 = curve.GetPointAt( p - 1 );
				const decCurveBezierPoint &p2 = curve.GetPointAt( p );
				
				x = CurveXToWindow( p1.GetPoint().x );
				y = CurveYToWindow( p1.GetPoint().y );
				
				h1x = CurveXToWindow( p1.GetHandle2().x );
				h1y = CurveYToWindow( p1.GetHandle2().y );
				
				h2x = CurveXToWindow( p2.GetHandle1().x );
				h2y = CurveYToWindow( p2.GetHandle1().y );
				
				x2 = CurveXToWindow( p2.GetPoint().x );
				y2 = CurveYToWindow( p2.GetPoint().y );
				
				if( p == 1 ){
					dc.drawLine( pRulerSize.x, y, x, y );
				}
				
				if( x < width && x2 >= pRulerSize.x ){
					bp1.Set( ( float )x - 0.5f, ( float )y - 0.5f );
					bp2.Set( ( float )h1x - 0.5f, ( float )h1y - 0.5f );
					bp3.Set( ( float )h2x - 0.5f, ( float )h2y - 0.5f );
					bp4.Set( ( float )x2 - 0.5f, ( float )y2 - 0.5f );
					
					ClampHandles( bp1, bp2, bp3, bp4 );
					InternalDrawCurve( dc, bp1, bp2, bp3, bp4 );
				}
				
				if( p == pointCount - 1 ){
					dc.drawLine( x2, y2, width, y2 );
				}
			}
		}
	}
	
	DrawCurveHandles( dc );
}

void igdeNativeFoxViewCurveBezierView::DrawCurveHandles( FXDC &dc ){
	const int selectedPoint = pOwner->GetSelectedPoint();
	const decCurveBezier &curve = pOwner->GetCurve();
	int p, pointCount = curve.GetPointCount();
	const FXColor clrLine = FXRGB( 0, 96, 255 );
	const FXColor clrDot = FXRGB( 0, 128, 0 );
	const FXColor clrDotSel = FXRGB( 255, 128, 0 );
	int x, y, x2, y2;
	
	if( curve.GetInterpolationMode() == decCurveBezier::eimBezier ){
		dc.setForeground( clrLine );
		for( p=0; p<pointCount; p++ ){
			const decCurveBezierPoint &point = curve.GetPointAt( p );
			
			x = CurveXToWindow( point.GetPoint().x );
			y = CurveYToWindow( point.GetPoint().y );
			
			x2 = CurveXToWindow( point.GetHandle1().x );
			y2 = CurveYToWindow( point.GetHandle1().y );
			dc.drawLine( x2, y2, x, y );
			
			x2 = CurveXToWindow( point.GetHandle2().x );
			y2 = CurveYToWindow( point.GetHandle2().y );
			dc.drawLine( x, y, x2, y2 );
		}
		
		dc.setForeground( clrDot );
		for( p=0; p<pointCount; p++ ){
			if( p != selectedPoint ){
				const decCurveBezierPoint &point = curve.GetPointAt( p );
				
				x = CurveXToWindow( point.GetPoint().x );
				y = CurveYToWindow( point.GetPoint().y );
				dc.fillEllipse( x - pDotOffset, y - pDotOffset, pDotSize, pDotSize );
				
				x = CurveXToWindow( point.GetHandle1().x );
				y = CurveYToWindow( point.GetHandle1().y );
				dc.fillEllipse( x - pDotOffset, y - pDotOffset, pDotSize, pDotSize );
				
				x = CurveXToWindow( point.GetHandle2().x );
				y = CurveYToWindow( point.GetHandle2().y );
				dc.fillEllipse( x - pDotOffset, y - pDotOffset, pDotSize, pDotSize );
			}
		}
		
		if( selectedPoint != -1 ){
			dc.setForeground( clrDotSel );
			
			const decCurveBezierPoint &point = curve.GetPointAt( selectedPoint );
			
			x = CurveXToWindow( point.GetPoint().x );
			y = CurveYToWindow( point.GetPoint().y );
			dc.fillEllipse( x - pDotOffset, y - pDotOffset, pDotSize, pDotSize );
			
			x = CurveXToWindow( point.GetHandle1().x );
			y = CurveYToWindow( point.GetHandle1().y );
			dc.fillEllipse( x - pDotOffset, y - pDotOffset, pDotSize, pDotSize );
			
			x = CurveXToWindow( point.GetHandle2().x );
			y = CurveYToWindow( point.GetHandle2().y );
			dc.fillEllipse( x - pDotOffset, y - pDotOffset, pDotSize, pDotSize );
		}
		
	}else{ // constant and linear
		dc.setForeground( clrDot );
		for( p=0; p<pointCount; p++ ){
			if( p != selectedPoint ){
				const decCurveBezierPoint &point = curve.GetPointAt( p );
				
				x = CurveXToWindow( point.GetPoint().x );
				y = CurveYToWindow( point.GetPoint().y );
				dc.fillEllipse( x - pDotOffset, y - pDotOffset, pDotSize, pDotSize );
			}
		}
		
		if( selectedPoint != -1 ){
			dc.setForeground( clrDotSel );
			
			const decCurveBezierPoint &point = curve.GetPointAt( selectedPoint );
			
			x = CurveXToWindow( point.GetPoint().x );
			y = CurveYToWindow( point.GetPoint().y );
			dc.fillEllipse( x - pDotOffset, y - pDotOffset, pDotSize, pDotSize );
		}
	}
	
	/*
	decCurveBezierEvaluator evaluator( &curve );
	dc.setForeground( clrDotSel );
	for( p=0; p<5000; p++ ){
		float fx = ( float )p / 5000.0f;
		float fy = evaluator.EvaluateAt( fx );
		x = pGridStartX + ( int )( fx * pGridWidth );
		y = pGridStartY - ( int )( fy * pGridHeight );
		dc.fillEllipse( x - 1, y - 1, 3, 3 );
	}
	*/
}

void igdeNativeFoxViewCurveBezierView::ClampHandles( const decVector2 &p1, decVector2 &h1,
decVector2 &h2, const decVector2 &p2 ) const{
	// clamp the handle x coordinates to the range
	if( h1.x < p1.x ){
		h1.x = p1.x;
	}
	if( h2.x > p2.x ){
		h2.x = p2.x;
	}
	
	// adjust handles so they do not overlap in the x direction
	const float lens = p2.x - p1.x;
	const float len1 = h1.x - p1.x;
	const float len2 = p2.x - h2.x;
	
	if( len1 + len2 > lens ){
		const float blend1 = lens / ( len1 + len2 );
		const float blend2 = 1.0f - blend1;
		
		h1.x = h1.x * blend1 + p1.x * blend2;
		h1.y = h1.y * blend1 + p1.y * blend2;
		
		h2.x = h2.x * blend1 + p2.x * blend2;
		h2.y = h2.y * blend1 + p2.y * blend2;
	}
}

void igdeNativeFoxViewCurveBezierView::InternalDrawCurve( FXDC &dc, const decVector2 &p1,
const decVector2 &p2, const decVector2 &p3, const decVector2 &p4 ){
	decVector2 diff( p2 - p1 );
	float length = diff * diff;
	diff = p3 - p2;
	length += diff * diff;
	diff = p4 - p3;
	length += diff * diff;
	
	if( length < 60.0f ){
		dc.drawLine( ( int )p1.x, ( int )p1.y, ( int )p2.x, ( int )p2.y );
		dc.drawLine( ( int )p2.x, ( int )p2.y, ( int )p3.x, ( int )p3.y );
		dc.drawLine( ( int )p3.x, ( int )p3.y, ( int )p4.x, ( int )p4.y );
		
	}else{
		const decVector2 np1( ( p1 + p2 ) * 0.5f );
		const decVector2 np2( ( p2 + p3 ) * 0.5f );
		const decVector2 np3( ( p3 + p4 ) * 0.5f );
		const decVector2 np4( ( np1 + np2 ) * 0.5f );
		const decVector2 np5( ( np2 + np3 ) * 0.5f );
		const decVector2 np6( ( np4 + np5 ) * 0.5f );
		
		InternalDrawCurve( dc, p1, np1, np4, np6 );
		InternalDrawCurve( dc, np6, np5, np3, p4 );
	}
}


void igdeNativeFoxViewCurveBezierView::DisplayPopupView( const decPoint &position ){
	pOwner->ShowContextMenu( position );
}



FXint igdeNativeFoxViewCurveBezierView::getDefaultWidth(){
	return pOwner->GetDefaultSize().x;
}

FXint igdeNativeFoxViewCurveBezierView::getDefaultHeight(){
	return pOwner->GetDefaultSize().y;
}



int igdeNativeFoxViewCurveBezierView::ViewCurveBezierFlags( const igdeViewCurveBezier & ){
	return FRAME_SUNKEN;
}

igdeFont *igdeNativeFoxViewCurveBezierView::ViewCurveBezierFont( const igdeViewCurveBezier &powner,
const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	powner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::viewCurveBezierFontSizeAbsolute ) ){
		configuration.size = ( float )guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::viewCurveBezierFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::viewCurveBezierFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::viewCurveBezierFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = ( float )guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return powner.GetEnvironment().GetSharedFont( configuration );
}



// Events
///////////

long igdeNativeFoxViewCurveBezierView::onResize( FXObject*, FXSelector, void* ){
	pDirtyGridParams = true;
	update();
	return 1;
}

long igdeNativeFoxViewCurveBezierView::onPaint( FXObject*, FXSelector, void *pdata ){
	FXDCWindow dc( this, ( FXEvent* )pdata );
	
	UpdateParameters();
	
	DrawGrid( dc );
	if( pOwner->GetEnabled() ){
		DrawCurve( dc );
	}
	DrawRulers( dc );
	
	return 1;
}

long igdeNativeFoxViewCurveBezierView::onLeftMouseDown( FXObject*, FXSelector, void *pdata ){
	if( pDragMode != edmNone ){
		return 0;
	}
	
	const FXEvent &event = *( ( FXEvent* )pdata );
	
	UpdateParameters();
	
	// set drag parameters
	pDragOrg.x = pDragCur.x = event.win_x;
	pDragOrg.y = pDragCur.y = event.win_y;
	pDragShift = ( event.state & SHIFTMASK ) == SHIFTMASK;
	pDragControl = ( event.state & CONTROLMASK ) == CONTROLMASK;
	
	// clicked into the bottom left corner
	if( pDragOrg.x < pRulerSize.x && pDragOrg.y > height - pRulerSize.y ){
		
	// clicked into the left ruler
	}else if( pDragOrg.x < pRulerSize.x ){
		if( pDragShift ){
			pDragOldZoom.y = pGridZoom.y;
			pDragMode = edmScaleRulerY;
			
		}else{
			pDragOldCenter.y = pGridCenter.y;
			pDragMode = edmDragRulerY;
		}
		
	// clicked into the bottom ruler
	}else if( pDragOrg.y > height - pRulerSize.y ){
		if( pDragShift ){
			pDragOldZoom.x = pGridZoom.x;
			pDragMode = edmScaleRulerX;
			
		}else{
			pDragOldCenter.x = pGridCenter.x;
			pDragMode = edmDragRulerX;
		}
		
	// clicked into the curve
	}else{
		bool startDragPoint = false;
		bool startDragHandle1 = false;
		bool startDragHandle2 = false;
		
		// check which point is clicked upon
		const int selPoint = IndexOfPointAt( pDragOrg.x, pDragOrg.y, esmPoint );
		int selHandle1 = -1;
		int selHandle2 = -1;
		
		if( pOwner->GetCurve().GetInterpolationMode() == decCurveBezier::eimBezier ){
			selHandle1 = IndexOfPointAt( pDragOrg.x, pDragOrg.y, esmHandle1 );
			selHandle2 = IndexOfPointAt( pDragOrg.x, pDragOrg.y, esmHandle2 );
		}
		
		// if the shift and control key is pressed remove the selected point if any
		if( pDragShift && pDragControl ){
			if( selPoint != -1 ){
				pOwner->SetSelectedPoint( -1 );
				
				decCurveBezier curve( pOwner->GetCurve() );
				curve.RemovePointFrom( selPoint );
				pOwner->SetCurve( curve, false );
			}
			
		// if only the shift key is pressed add a new point if none is selected
		}else if( pDragShift ){
			if( selPoint == -1 ){
				float nx = WindowXToCurve( pDragOrg.x );
				float ny = WindowYToCurve( pDragOrg.y );
				
				/*
				if( nx < 0.0f ) nx = 0.0f;
				if( nx > 1.0f ) nx = 1.0f;
				if( ny < 0.0f ) ny = 0.0f;
				if( ny > 1.0f ) ny = 1.0f;
				*/
				
				decCurveBezier curve( pOwner->GetCurve() );
				const int index = curve.AddPoint( decCurveBezierPoint( decVector2( nx, ny ),
					decVector2( nx - 0.05f, ny ), decVector2( nx + 0.05f, ny ) ) );
				pOwner->SetCurve( curve, false );
				pOwner->SetSelectedPoint( index );
				
				startDragPoint = true;
				
				update();
			}
			
		// otherwise we can drag if there is a selected point
		}else{
			if( selHandle2 != -1 ){
				if( selHandle2 != pOwner->GetSelectedPoint() ){
					pOwner->SetSelectedPoint( selHandle2 );
				}
				
				startDragHandle2 = true;
				
			}else if( selHandle1 != -1 ){
				if( selHandle1 != pOwner->GetSelectedPoint() ){
					pOwner->SetSelectedPoint( selHandle1 );
				}
				
				startDragHandle1 = true;
				
			}else if( selPoint != -1 ){
				if( selPoint != pOwner->GetSelectedPoint() ){
					pOwner->SetSelectedPoint( selPoint );
				}
				
				startDragPoint = true;
			}
		}
		
		// start draging if required
		if( startDragPoint ){
			const decCurveBezier &curve = pOwner->GetCurve();
			const int selectedPoint = pOwner->GetSelectedPoint();
			const decCurveBezierPoint &point = curve.GetPointAt( selectedPoint );
			
			pDragOrgPoint = point;
			pDragPoint.x = CurveXToWindow( point.GetPoint().x );
			pDragPoint.y = CurveYToWindow( point.GetPoint().y );
			pDragHandle1Offset = point.GetHandle1() - point.GetPoint();
			pDragHandle2Offset = point.GetHandle2() - point.GetPoint();
			
			if( selectedPoint > 0 ){
				const decCurveBezierPoint &point2 = curve.GetPointAt( selectedPoint - 1 );
				
				pDragXLowerLimit = point2.GetPoint().x + CE_MIN_DIST;
				pUseXLowerLimit = true;
				
			}else{
				pUseXLowerLimit = false;
			}
			
			if( selectedPoint < curve.GetPointCount() - 1 ){
				const decCurveBezierPoint &point2 = curve.GetPointAt( selectedPoint + 1 );
				
				pDragXUpperLimit = point2.GetPoint().x - CE_MIN_DIST;
				pUseXUpperLimit = true;
				
			}else{
				pUseXUpperLimit = false;
			}
			
			pDragMode = edmDragCurvePoint;
			pStickyDragging = true;
			
		}else if( startDragHandle1 ){
			const decCurveBezierPoint &point = pOwner->GetCurve().GetPointAt( pOwner->GetSelectedPoint() );
			
			pDragOrgPoint = point;
			pDragPoint.x = CurveXToWindow( point.GetHandle1().x );
			pDragPoint.y = CurveYToWindow( point.GetHandle1().y );
			
			pDragXUpperLimit = point.GetPoint().x;
			pUseXLowerLimit = false;
			pUseXUpperLimit = true;
			
			pDragMode = edmDragCurveHandle1;
			pStickyDragging = true;
			
		}else if( startDragHandle2 ){
			const decCurveBezierPoint &point = pOwner->GetCurve().GetPointAt( pOwner->GetSelectedPoint() );
			
			pDragOrgPoint = point;
			pDragPoint.x = CurveXToWindow( point.GetHandle2().x );
			pDragPoint.y = CurveYToWindow( point.GetHandle2().y );
			
			pDragXLowerLimit = point.GetPoint().x;
			pUseXLowerLimit = true;
			pUseXUpperLimit = false;
			
			pDragMode = edmDragCurveHandle2;
			pStickyDragging = true;
		}
		
		update();
		return 1;
	}
	
	return 1;
}

long igdeNativeFoxViewCurveBezierView::onMouseMove( FXObject*, FXSelector, void *pdata ){
	if( pDragMode == edmNone ){
		return 1;
	}
	
	const FXEvent &event = *( ( FXEvent* )pdata );
	bool shift = ( event.state & SHIFTMASK ) == SHIFTMASK;
	bool control = ( event.state & CONTROLMASK ) == CONTROLMASK;
	int dragDiffX = event.win_x - pDragOrg.x;
	int dragDiffY = event.win_y - pDragOrg.y;
	int x = event.win_x;
	int y = event.win_y;
	
	pDragCur.x = x;
	pDragCur.y = y;
	
	UpdateParameters();
	
	switch( pDragMode ){
	case edmDragCurvePoint:
	case edmDragCurveHandle1:
	case edmDragCurveHandle2:
		// check for really dragging
		if( pStickyDragging ){
			if( abs( pDragCur.x - pDragOrg.x ) > pStickyDragThreshold || abs( pDragCur.y - pDragOrg.y ) > pStickyDragThreshold ){
				pStickyDragging = false;
			}
		}
		
		// drag the point around if not delayed
		if( ! pStickyDragging ){
			decVector2 point;
			
			point.x = WindowXToCurve( pDragCur.x );
			point.y = WindowYToCurve( pDragCur.y );
			
			if( pOwner->GetClamp() && pDragMode == edmDragCurvePoint ){
				point.SetClamped( pOwner->GetClampMin(), pOwner->GetClampMax() );
			}
			
			if( pUseXLowerLimit ){
				point.x = decMath::max( point.x, pDragXLowerLimit );
			}
			if( pUseXUpperLimit ){
				point.x = decMath::min( point.x, pDragXUpperLimit );
			}
			
			if( pDragMode == edmDragCurvePoint ){
				decCurveBezier curve( pOwner->GetCurve() );
				curve.RemovePointFrom( pOwner->GetSelectedPoint() );
				const int index = curve.AddPoint( decCurveBezierPoint( point,
					point + pDragHandle1Offset, point + pDragHandle2Offset ) );
				
				pOwner->SetCurve( curve, true );
				pOwner->SetSelectedPoint( index );
				
			}else if( pDragMode == edmDragCurveHandle1 ){
				const decVector2 &cpp = pDragOrgPoint.GetPoint();
				const decVector2 &cph2 = pDragOrgPoint.GetHandle2();
				
				decCurveBezier curve( pOwner->GetCurve() );
				curve.RemovePointFrom( pOwner->GetSelectedPoint() );
				int index;
				
				if( shift ){
					const float len1 = ( point - cpp ).Length();
					
					if( len1 == 0.0 ){
						index = curve.AddPoint( decCurveBezierPoint( cpp, point, cph2 ) );
						
					}else{
						const float len2 = ( cph2 - cpp ).Length();
						const float blend2 = len2 / len1;
						const float blend1 = 1.0f + blend2;
						
						index = curve.AddPoint( decCurveBezierPoint( cpp, point, cpp * blend1 - point * blend2 ) );
					}
					
				}else if( control ){
					index = curve.AddPoint( decCurveBezierPoint( cpp, point, cpp + cpp - point ) );
					
				}else{
					index = curve.AddPoint( decCurveBezierPoint( cpp, point, cph2 ) );
				}
				
				pOwner->SetCurve( curve, true );
				pOwner->SetSelectedPoint( index );
				
			}else{ // edmDragCurveHandle2
				const decVector2 &cpp = pDragOrgPoint.GetPoint();
				const decVector2 &cph1 = pDragOrgPoint.GetHandle1();
				
				decCurveBezier curve( pOwner->GetCurve() );
				curve.RemovePointFrom( pOwner->GetSelectedPoint() );
				int index;
				
				if( shift ){
					const float len1 = ( point - cpp ).Length();
					
					if( len1 == 0.0 ){
						index = curve.AddPoint( decCurveBezierPoint( cpp, cph1, point ) );
						
					}else{
						const float len2 = ( cph1 - cpp ).Length();
						const float blend2 = len2 / len1;
						const float blend1 = 1.0f + blend2;
						
						index = curve.AddPoint( decCurveBezierPoint( cpp, cpp * blend1 - point * blend2, point ) );
					}
					
				}else if( control ){
					index = curve.AddPoint( decCurveBezierPoint( cpp, cpp + cpp - point, point ) );
					
				}else{
					index = curve.AddPoint( decCurveBezierPoint( cpp, cph1, point ) );
				}
				
				pOwner->SetCurve( curve, true );
				pOwner->SetSelectedPoint( index );
			}
			
			if( target ){
				target->tryHandle( this, FXSEL( SEL_CHANGED, message ), NULL );
			}
			
			update();
			return 1;
		}
		break;
		
	case edmDragView:
		pGridCenter.x = pDragOldCenter.x - ( float )dragDiffX / pGridScale.x;
		pGridCenter.y = pDragOldCenter.y + ( float )dragDiffY / pGridScale.y;
		pDirtyGridParams = true;
		update();
		return 1;
		
	case edmScaleView:
		SetGridZoom( pDragOldZoom + decVector2( ( float )dragDiffX, ( float )dragDiffY ) / 100.0f );
		update();
		return 1;
		
	case edmDragRulerX:
		pGridCenter.x = pDragOldCenter.x - ( float )dragDiffX / pGridScale.x;
		pDirtyGridParams = true;
		update();
		return 1;
		
	case edmScaleRulerX:
		SetGridZoom( decVector2( pDragOldZoom.x + ( float )dragDiffX / 100.0f, pGridZoom.y ) );
		update();
		return 1;
		
	case edmDragRulerY:
		pGridCenter.y = pDragOldCenter.y + ( float )dragDiffY / pGridScale.y;
		pDirtyGridParams = true;
		update();
		return 1;
		
	case edmScaleRulerY:
		SetGridZoom( decVector2( pGridZoom.x, pDragOldZoom.y + ( float )dragDiffY / 100.0f ) );
		update();
		return 1;
		
	default:
		break;
	}
	
	return 1;
}

// TODO call NotifyCurveChanged() on MouseRelease

long igdeNativeFoxViewCurveBezierView::onMouseWheel( FXObject*, FXSelector, void* ){
	return 1;
}

long igdeNativeFoxViewCurveBezierView::onLeftMouseUp( FXObject*, FXSelector, void* ){
	switch( pDragMode ){
	case edmDragCurvePoint:
	case edmDragCurveHandle1:
	case edmDragCurveHandle2:
		pDragMode = edmNone;
		if( pStickyDragging ){
			break;
		}
		
		if( target ){
			target->tryHandle( this, FXSEL( SEL_COMMAND, message ), NULL );
		}
		
		// we have to send a curve change notification since the last mouse move causes
		// SetCurve() to be called using changing = true. if we call now SetCurve() again
		// the curve value will be identical and SetCurve() will not send a notification.
		// correctly we would have to use a variable to keep track if any changing has
		// been done or not. using pStickyDragging is good enough for the time being
		pOwner->NotifyCurveChanged();
		break;
		
	case edmDragRulerX:
	case edmScaleRulerX:
		pDragMode = edmNone;
		break;
		
	case edmDragRulerY:
	case edmScaleRulerY:
		pDragMode = edmNone;
		break;
		
	default:
		break;
	}
	
	return 1;
}

long igdeNativeFoxViewCurveBezierView::onRightMouseDown( FXObject*, FXSelector, void *pdata ){
	if( pDragMode == edmNone ){
		const FXEvent &event = *( ( FXEvent* )pdata );
		int x = event.win_x;
		int y = event.win_y;
		
		// clicked into the bottom left corner
		if( x < pRulerSize.x && y > height - pRulerSize.y ){
			
		// clicked into the left ruler
		}else if( x < pRulerSize.x ){
			
		// clicked into the bottom ruler
		}else if( y > height - pRulerSize.y ){
			
		// clicked into the curve
		}else{
			DisplayPopupView( decPoint( x, y ) );
		}
	}
	
	return 1;
}

long igdeNativeFoxViewCurveBezierView::onRightMouseUp( FXObject*, FXSelector, void* ){
	return 1;
}

long igdeNativeFoxViewCurveBezierView::onMiddleMouseDown( FXObject*, FXSelector, void *pdata ){
	if( pDragMode == edmNone ){
		const FXEvent &event = *( ( FXEvent* )pdata );
		
		UpdateParameters();
		
		pDragOrg.x = pDragCur.x = event.win_x;
		pDragOrg.y = pDragCur.y = event.win_y;
		pDragShift = ( event.state & SHIFTMASK ) == SHIFTMASK;
		pDragControl = ( event.state & CONTROLMASK ) == CONTROLMASK;
		
		if( pDragShift ){
			pDragOldZoom.x = pGridZoom.x;
			pDragOldZoom.y = pGridZoom.y;
			pDragMode = edmScaleView;
			
		}else{
			pDragOldCenter.x = pGridCenter.x;
			pDragOldCenter.y = pGridCenter.y;
			pDragMode = edmDragView;
		}
	}
	
	return 1;
}

long igdeNativeFoxViewCurveBezierView::onMiddleMouseUp( FXObject*, FXSelector, void* ){
	switch( pDragMode ){
	case edmDragView:
	case edmScaleView:
		pDragMode = edmNone;
		break;
		
	default:
		break;
	}
	return 1;
}



// Protected Functions
////////////////////////

void igdeNativeFoxViewCurveBezierView::UpdateParameters(){
	if( pDirtyGridParams ){
		int wwidth = getWidth();
		int hheight = getHeight();
		int realWidth = wwidth - pRulerSize.x - 10;
		int realHeight = hheight - pRulerSize.y - 10;
		int minUnitPixelsX = 30;
		int minUnitPixelsY = 15;
		float unitSize;
		
		pWindowCenter.x = ( wwidth + pRulerSize.x ) / 2;
		pWindowCenter.y = ( hheight - pRulerSize.y ) / 2;
		
		if( realWidth < 10 ){
			realWidth = 10;
		}
		if( realHeight < 10 ){
			realHeight = 10;
		}
		
		pGridScale.x = ( ( float )realWidth / ( pGridMax.x - pGridMin.x ) ) * pGridZoom.x;
		pGridScale.y = ( ( float )realHeight / ( pGridMax.y - pGridMin.y ) ) * pGridZoom.y;
		
		unitSize = ( float )minUnitPixelsX / pGridScale.x;
		for( pGridUnitMode.x=0; pGridUnitMode.x<SCALE_MODE_COUNT; pGridUnitMode.x++ ){
			if( unitSize < vScaleModes[ pGridUnitMode.x ].unitSize ) break;
		}
		
		unitSize = ( float )minUnitPixelsY / pGridScale.y;
		for( pGridUnitMode.y=0; pGridUnitMode.y<SCALE_MODE_COUNT; pGridUnitMode.y++ ){
			if( unitSize < vScaleModes[ pGridUnitMode.y ].unitSize ) break;
		}
		//printf( "umx=%i('%s',%g) umy=%i('%s',%g)\n", pGridUnitMode.x, vScaleModes[pGridUnitMode.x].rulerFormatString, vScaleModes[pGridUnitMode.x].unitSize, pGridUnitMode.y, vScaleModes[pGridUnitMode.y].rulerFormatString, vScaleModes[pGridUnitMode.y].unitSize );
		/*
		pGridUnitsX = ceilf( log2f( ( float )minUnitPixels / pGridScale.x ) );
		pGridUnitsX *= pGridUnitsX;
		if( pGridUnitsX < 0.0625f ){
			pGridUnitsX = 0.0625f;
 		}
		pGridUnitsY = ceilf( log2f( ( float )minUnitPixels / pGridScale.y ) );
		pGridUnitsY *= pGridUnitsY;
		if( pGridUnitsY < 0.0625f ){
			pGridUnitsY = 0.0625f;
		}
		*/
		//printf( "ux=%g(%g) uy=%g(%g)\n", ( float )minUnitPixels / pGridScale.x, pGridUnitsX, ( float )minUnitPixels / pGridScale.y, pGridUnitsY );
		
		// blender:
		// 1 units => 2 units => 5 units => 10 units
		// smallest 0.001, largest depending on location (100, 2000)
		
		pDirtyGridParams = false;
	}
}



// Resizeable
///////////////

FXDEFMAP( igdeNativeFoxViewCurveBezier ) igdeNativeFoxViewCurveBezierMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxViewCurveBezier::ID_RESIZER,
		igdeNativeFoxViewCurveBezier::onResizerDrag )
};

FXIMPLEMENT( igdeNativeFoxViewCurveBezier, FXVerticalFrame,
	igdeNativeFoxViewCurveBezierMap, ARRAYNUMBER( igdeNativeFoxViewCurveBezierMap ) )


igdeNativeFoxViewCurveBezier::igdeNativeFoxViewCurveBezier(){ }

igdeNativeFoxViewCurveBezier::igdeNativeFoxViewCurveBezier(
	igdeViewCurveBezier &powner, FXComposite *pparent,
	const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme ) :
FXVerticalFrame( pparent, layoutFlags.flags, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ),
pView( new igdeNativeFoxViewCurveBezierView( powner, this, LAYOUT_FILL
	| igdeNativeFoxViewCurveBezierView::ViewCurveBezierFlags( powner ), guitheme ) ),
pResizer( NULL )
{
	if( layoutFlags.canResizeVertical || ( layoutFlags.flags & LAYOUT_FILL_Y ) == 0 ){
		pResizer = new igdeNativeFoxResizer( this, this, ID_RESIZER );
	}
}

igdeNativeFoxViewCurveBezier::~igdeNativeFoxViewCurveBezier(){
}

igdeNativeFoxViewCurveBezier *igdeNativeFoxViewCurveBezier::CreateNativeWidget( igdeViewCurveBezier &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const nativeParent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! nativeParent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxViewCurveBezier( powner, nativeParent,
		igdeUIFoxHelper::GetChildLayoutFlagsAll( &powner ), *powner.GetGuiTheme() );
}

void igdeNativeFoxViewCurveBezier::PostCreateNativeWidget(){
	const FXComposite &nativeParent = *( ( FXComposite* )pView->GetOwner().GetParent()->GetNativeContainer() );
	if( nativeParent.id() ){
		create();
	}
}

void igdeNativeFoxViewCurveBezier::DestroyNativeWidget(){
	delete this;
}



long igdeNativeFoxViewCurveBezier::onResizerDrag( FXObject*, FXSelector, void *pdata ){
	const int distance = igdeNativeFoxResizer::SelCommandDraggedDistance( pdata );
	igdeViewCurveBezier &powner = pView->GetOwner();
	decPoint defaultSize( powner.GetDefaultSize() );
	defaultSize.y = decMath::max( 50, defaultSize.y + distance );
	powner.SetDefaultSize( defaultSize );
	return 0;
}

#endif
