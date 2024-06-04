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

#include <stdio.h>
#include <stdlib.h>

#include "igdeViewCurveBezier.h"
#include "igdeViewCurveBezierListener.h"
#include "igdeDialogCurveBezierClamp.h"
#include "igdeDialogCurveBezierCoord.h"
#include "../igdeUIHelper.h"
#include "../dialog/igdeDialog.h"
#include "../dialog/igdeDialogReference.h"
#include "../menu/igdeMenuCascade.h"
#include "../menu/igdeMenuCascadeReference.h"
#include "../native/toolkit.h"

#include <dragengine/common/exceptions.h>



// class igdeViewCurveBezier::cActionResetView
////////////////////////////////////////////////

igdeViewCurveBezier::cActionResetView::cActionResetView( igdeViewCurveBezier &view ) :
igdeAction( "Reset View", NULL, "Reset view to default values" ),
pView( view ){
}

void igdeViewCurveBezier::cActionResetView::OnAction(){
	pView.ResetView();
}



// class igdeViewCurveBezier::cActionFitToCurve
/////////////////////////////////////////////////

igdeViewCurveBezier::cActionFitToCurve::cActionFitToCurve( igdeViewCurveBezier &view ) :
igdeAction( "Fit View to Curve", NULL, "Reset view parameters to fit curve into view area" ),
pView( view ){
}

void igdeViewCurveBezier::cActionFitToCurve::OnAction(){
	pView.FitViewToCurve();
}



// class igdeViewCurveBezier::cActionEditSelectedPoint
////////////////////////////////////////////////////////

#define CE_MIN_DIST		0.001f

igdeViewCurveBezier::cActionEditSelectedPoint::cActionEditSelectedPoint( igdeViewCurveBezier &view ) :
igdeAction( "Edit Selected Point...", NULL, "Show dialog to edit coordinates of selected point" ),
pView( view ){
}

void igdeViewCurveBezier::cActionEditSelectedPoint::OnAction(){
	int selectedPoint = pView.GetSelectedPoint();
	if( selectedPoint == -1 ){
		return;
	}
	
	igdeDialogReference dialog;
	dialog.TakeOver( new igdeDialogCurveBezierCoord( pView.GetEnvironment(),
		pView.GetCurve().GetPointAt( selectedPoint ) ) );
	if( ! dialog->Run( &pView ) ){
		return;
	}
	
	// clamp point to valid values
	decCurveBezierPoint curvePoint( ( ( igdeDialogCurveBezierCoord& )( igdeDialog& )dialog ).GetPoint() );
	decVector2 point( curvePoint.GetPoint() );
	decVector2 handle1( curvePoint.GetHandle1() );
	decVector2 handle2( curvePoint.GetHandle2() );
	
	decCurveBezier curve( pView.GetCurve() );
	
	float clampedPointX = point.x;
	if( selectedPoint > 0 ){
		clampedPointX = decMath::max( clampedPointX,
			curve.GetPointAt( selectedPoint - 1 ).GetPoint().x + CE_MIN_DIST );
	}
	if( selectedPoint < curve.GetPointCount() - 1 ){
		clampedPointX = decMath::min( clampedPointX,
			curve.GetPointAt( selectedPoint + 1 ).GetPoint().x - CE_MIN_DIST );
	}
	
	const float diffPointX = clampedPointX - point.x;
	point.x = clampedPointX;
	
	handle1.x = decMath::min( handle1.x + diffPointX, point.x );
	handle2.x = decMath::max( handle2.x + diffPointX, point.x );
	
	curvePoint.SetPoint( point, handle1, handle2 );
	
	// update point
	curve.RemovePointFrom( selectedPoint );
	selectedPoint = curve.AddPoint( curvePoint );
	
	pView.SetCurve( curve, false );
	pView.SetSelectedPoint( selectedPoint );
}

void igdeViewCurveBezier::cActionEditSelectedPoint::Update(){
	SetEnabled( pView.GetSelectedPoint() != -1 );
}



// class igdeViewCurveBezier::cActionSetInterpolationMode
///////////////////////////////////////////////////////////

igdeViewCurveBezier::cActionSetInterpolationMode::cActionSetInterpolationMode(
    igdeViewCurveBezier &view, decCurveBezier::eInterpolationModes mode, const char *text,
igdeIcon *icon, const char *description, deInputEvent::eKeyCodes mnemonic ) :
igdeAction( text, icon, description, mnemonic ),
pView( view ), pMode( mode ){
}

void igdeViewCurveBezier::cActionSetInterpolationMode::OnAction(){
	if( pView.GetCurve().GetInterpolationMode() == pMode ){
		return;
	}
	
	decCurveBezier curve( pView.GetCurve() );
	curve.SetInterpolationMode( pMode );
	pView.SetCurve( curve, false );
}

void igdeViewCurveBezier::cActionSetInterpolationMode::Update(){
	SetSelected( pView.GetCurve().GetInterpolationMode() == pMode );
}



// class igdeViewCurveBezier::cActionCopyCurve
////////////////////////////////////////////////

static decCurveBezier vIgdeViewCurveBezierCopyCurve;

igdeViewCurveBezier::cActionCopyCurve::cActionCopyCurve( igdeViewCurveBezier &view ) :
igdeAction( "Copy Curve", view.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
"Copy curve to clipboard" ),
pView( view ){
}

void igdeViewCurveBezier::cActionCopyCurve::OnAction(){
	   vIgdeViewCurveBezierCopyCurve = pView.GetCurve();
}



// class igdeViewCurveBezier::cActionPasteCurve
/////////////////////////////////////////////////

igdeViewCurveBezier::cActionPasteCurve::cActionPasteCurve( igdeViewCurveBezier &view ) :
igdeAction( "Paste Curve", view.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
"Paste curve from clipboard" ),
pView( view ){
}

void igdeViewCurveBezier::cActionPasteCurve::OnAction(){
	pView.SetCurve( vIgdeViewCurveBezierCopyCurve, false );
	pView.NotifyCurveSelectPoint();
}



// class igdeViewCurveBezier::cActionClearCurve
/////////////////////////////////////////////////

igdeViewCurveBezier::cActionClearCurve::cActionClearCurve( igdeViewCurveBezier &view ) :
igdeAction( "Clear Curve", NULL, "Remove all points from the curve" ),
pView( view ){
}

void igdeViewCurveBezier::cActionClearCurve::OnAction(){
	if( pView.GetCurve().GetPointCount() > 0 ){
		pView.ClearCurve();
	}
}



// class igdeViewCurveBezier::cActionSetDefaultConstant
/////////////////////////////////////////////////////////

igdeViewCurveBezier::cActionSetDefaultConstant::cActionSetDefaultConstant( igdeViewCurveBezier &view ) :
igdeAction( "Set to Default Constant", nullptr, "Set curve to constant interpolation switching from 0 to 1 at 0.5" ),
pView( view ){
}

void igdeViewCurveBezier::cActionSetDefaultConstant::OnAction(){
	pView.SetDefaultConstant();
}



// class igdeViewCurveBezier::cActionSetDefaultLinear
///////////////////////////////////////////////////////

igdeViewCurveBezier::cActionSetDefaultLinear::cActionSetDefaultLinear( igdeViewCurveBezier &view ) :
igdeAction( "Set to Default Linear", NULL, "Set curve to linear interpolation from (0,0) to (1,1)" ),
pView( view ){
}

void igdeViewCurveBezier::cActionSetDefaultLinear::OnAction(){
	pView.SetDefaultLinear();
}



// class igdeViewCurveBezier::cActionSetDefaultBezier
///////////////////////////////////////////////////////

igdeViewCurveBezier::cActionSetDefaultBezier::cActionSetDefaultBezier( igdeViewCurveBezier &view ) :
igdeAction( "Set to Default Bezier", NULL, "Set curve to bezier interpolation from (0,0) to (1,1)" ),
pView( view ){
}

void igdeViewCurveBezier::cActionSetDefaultBezier::OnAction(){
	pView.SetDefaultBezier();
}



// class igdeViewCurveBezier::cActionInvertCurveX
///////////////////////////////////////////////////

igdeViewCurveBezier::cActionInvertCurveX::cActionInvertCurveX( igdeViewCurveBezier &view ) :
igdeAction( "Invert curve along X axis", NULL, "Invert curve along X axis" ),
pView( view ){
}

void igdeViewCurveBezier::cActionInvertCurveX::OnAction(){
	pView.InvertCurveX();
}



// class igdeViewCurveBezier::cActionInvertCurveY
///////////////////////////////////////////////////

igdeViewCurveBezier::cActionInvertCurveY::cActionInvertCurveY( igdeViewCurveBezier &view ) :
igdeAction( "Invert curve along Y axis", NULL, "Invert curve along Y axis" ),
pView( view ){
}

void igdeViewCurveBezier::cActionInvertCurveY::OnAction(){
	pView.InvertCurveY();
}



// class igdeViewCurveBezier::cActionEditClamp
////////////////////////////////////////////////

igdeViewCurveBezier::cActionEditClamp::cActionEditClamp( igdeViewCurveBezier &view ) :
igdeAction( "Edit Clamp Parameters...", NULL, "Show dialog to edit clamp parameters for the view" ),
pView( view ){
}

void igdeViewCurveBezier::cActionEditClamp::OnAction(){
	igdeDialogReference dialog;
	dialog.TakeOver( new igdeDialogCurveBezierClamp( pView.GetEnvironment(),
		pView.GetClamp(), pView.GetClampMin(), pView.GetClampMax() ) );
	if( ! dialog->Run( &pView ) ){
		return;
	}
	
	igdeDialogCurveBezierClamp &cbc = ( igdeDialogCurveBezierClamp& )( igdeDialog& )dialog;
	pView.SetClamp( cbc.GetClamp() );
	pView.SetClampMin( cbc.GetClampMin() );
	pView.SetClampMax( cbc.GetClampMax() );
}



// class igdeViewCurveBezier::cActionAutoHandles
//////////////////////////////////////////////////

igdeViewCurveBezier::cActionAutoHandles::cActionAutoHandles( igdeViewCurveBezier &view ) :
igdeAction( "Auto Handles", nullptr, "Set default handles for interpolation type" ),
pView( view ){
}

void igdeViewCurveBezier::cActionAutoHandles::OnAction(){
	pView.SetAutoHandles();
}



// Class igdeViewCurveBezier
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeViewCurveBezier::igdeViewCurveBezier( igdeEnvironment &environment ) :
igdeWidget( environment ),
pEnabled( true ),
pSelPoint( -1 ),
pClamp( false ),
pClampMin( 0.0f, 0.0f ),
pClampMax( 1.0f, 1.0f ),
pDefaultSize( 200, 150 )
{
	SetDefaultBezier();
}

igdeViewCurveBezier::~igdeViewCurveBezier(){
	igdeViewCurveBezier::DestroyNativeWidget();
}



// Management
///////////////

void igdeViewCurveBezier::SetDefaultSize( const decPoint &size ){
	if( size == pDefaultSize ){
		return;
	}
	
	pDefaultSize = size;
	OnDefaultSizeChanged();
}

void igdeViewCurveBezier::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}



void igdeViewCurveBezier::SetCurve( const decCurveBezier &curve, bool changing ){
	if( pCurve == curve ){
		return;
	}
	
	if( pSelPoint >= curve.GetPointCount() ){
		SetSelectedPoint( pSelPoint = -1 );
	}
	
	pCurve = curve;
	
	OnCurveChanged();
	
	if( changing ){
		NotifyCurveChanging();
		
	}else{
		NotifyCurveChanged();
	}
}

void igdeViewCurveBezier::ClearCurve(){
	SetCurve( decCurveBezier() );
	ResetView();
}

void igdeViewCurveBezier::InvertCurveX(){
	decCurveBezier curve;
	curve.SetInterpolationMode( pCurve.GetInterpolationMode() );
	
	const int count = pCurve.GetPointCount();
	const float l = pClampMin.x;
	const float r = pClampMax.x;
	int i;
	
	for( i=count-1; i>=0; i-- ){
		const decCurveBezierPoint &point = pCurve.GetPointAt( i );
		const decVector2 &p = point.GetPoint();
		const decVector2 &h1 = point.GetHandle1();
		const decVector2 &h2 = point.GetHandle2();
		
		curve.AddPoint( decCurveBezierPoint( \
			decVector2( decMath::linearStep( p.x, l, r, r, l ), p.y ),
			decVector2( decMath::linearStep( h2.x, l, r, r, l ), h2.y ),
			decVector2( decMath::linearStep( h1.x, l, r, r, l ), h1.y ) ) );
	}
	SetCurve( curve );
}

void igdeViewCurveBezier::InvertCurveY(){
	decCurveBezier curve;
	curve.SetInterpolationMode( pCurve.GetInterpolationMode() );
	
	const int count = pCurve.GetPointCount();
	const float l = pClampMin.y;
	const float u = pClampMax.y;
	int i;
	
	for( i=0; i<count; i++ ){
		const decCurveBezierPoint &point = pCurve.GetPointAt( i );
		const decVector2 &p = point.GetPoint();
		const decVector2 &h1 = point.GetHandle1();
		const decVector2 &h2 = point.GetHandle2();
		
		curve.AddPoint( decCurveBezierPoint( \
			decVector2( p.x, decMath::linearStep( p.y, l, u, u, l ) ),
			decVector2( h1.x, decMath::linearStep( h1.y, l, u, u, l ) ),
			decVector2( h2.x, decMath::linearStep( h2.y, l, u, u, l ) ) ) );
	}
	SetCurve( curve );
}

void igdeViewCurveBezier::SetDefaultConstant(){
	const decVector2 center( ( pClampMin + pClampMax ) * 0.5f );
	decCurveBezier curve;
	curve.AddPoint( decCurveBezierPoint( pClampMin ) );
	curve.AddPoint( decCurveBezierPoint( decVector2( center.x, pClampMax.y ) ) );
	curve.SetInterpolationMode( decCurveBezier::eimConstant );
	SetCurve( curve );
	
	ResetView();
}

void igdeViewCurveBezier::SetDefaultLinear(){
	const decVector2 offsetHandle( ( pClampMax.x - pClampMin.x ) * 0.25f, 0.0f );
	decCurveBezier curve;
	curve.AddPoint( decCurveBezierPoint( pClampMin, pClampMin - offsetHandle, pClampMin + offsetHandle ) );
	curve.AddPoint( decCurveBezierPoint( pClampMax, pClampMax - offsetHandle, pClampMax + offsetHandle ) );
	curve.SetInterpolationMode( decCurveBezier::eimLinear );
	SetCurve( curve );
	
	FitViewToCurve();
}

void igdeViewCurveBezier::SetDefaultBezier(){
	const decVector2 offsetHandle( ( pClampMax.x - pClampMin.x ) * 0.25f, 0.0f );
	decCurveBezier curve;
	curve.AddPoint( decCurveBezierPoint( pClampMin, pClampMin - offsetHandle, pClampMin + offsetHandle ) );
	curve.AddPoint( decCurveBezierPoint( pClampMax, pClampMax - offsetHandle, pClampMax + offsetHandle ) );
	curve.SetInterpolationMode( decCurveBezier::eimBezier );
	SetCurve( curve );
	
	FitViewToCurve();
}

void igdeViewCurveBezier::SetAutoHandles(){
	const int count = pCurve.GetPointCount();
	const float f = 0.25f;
	decCurveBezier curve;
	int i;
	
	if( pCurve.GetInterpolationMode() == decCurveBezier::eimBezier ){
		if( count == 1 ){
			const decVector2 &pt = pCurve.GetPointAt( 0 ).GetPoint();
			const decVector2 offset( f, 0.0f );
			curve.AddPoint( decCurveBezierPoint( pt, pt - offset, pt + offset ) );
			
		}else if( count > 1 ){
			for( i=0; i<count; i++ ){
				const decVector2 &pt = pCurve.GetPointAt( i ).GetPoint();
				
				if( i == 0 ){
					decVector2 h1( pt.Mix( pCurve.GetPointAt( i + 1 ).GetPoint(), f ) );
					h1.y = pt.y;
					const decVector2 h2( pt - ( h1 - pt ) );
					curve.AddPoint( decCurveBezierPoint( pt, h2, h1 ) );
					
				}else if( i == count - 1 ){
					decVector2 h1( pt.Mix( pCurve.GetPointAt( i - 1 ).GetPoint(), f ) );
					h1.y = pt.y;
					const decVector2 h2( pt + ( pt - h1 ) );
					curve.AddPoint( decCurveBezierPoint( pt, h1, h2 ) );
					
				}else{
					decVector2 h1( pt.Mix( pCurve.GetPointAt( i - 1 ).GetPoint(), f ) );
					decVector2 h2( pt.Mix( pCurve.GetPointAt( i + 1 ).GetPoint(), f ) );
					h1.y = pt.y;
					h2.y = pt.y;
					curve.AddPoint( decCurveBezierPoint( pt, h1, h2 ) );
				}
			}
		}
		
	}else{
		for( i=0; i<count; i++ ){
			const decVector2 &pt = pCurve.GetPointAt( i ).GetPoint();
			curve.AddPoint( decCurveBezierPoint( pt, pt, pt ) );
		}
	}
	
	SetCurve( curve );
}



void igdeViewCurveBezier::SetSelectedPoint( int index ){
	if( index < -1 || index >= pCurve.GetPointCount() ){
		DETHROW( deeInvalidParam );
	}
	
	if( index == pSelPoint ){
		return;
	}
	
	pSelPoint = index;
	OnSelectedPointChanged();
	
	NotifyCurveSelectPoint();
}



void igdeViewCurveBezier::SetClamp( bool clamp ){
	if( clamp == pClamp ){
		return;
	}
	
	pClamp = clamp;
	OnClampChanged();
}

void igdeViewCurveBezier::SetClampMin( const decVector2 &clamp ){
	if( clamp.IsEqualTo( pClampMin ) ){
		return;
	}
	
	pClampMin = clamp;
	pClampMax.SetLargest( clamp );
	OnClampChanged();
}

void igdeViewCurveBezier::SetClampMax( const decVector2 &clamp ){
	if( clamp.IsEqualTo( pClampMax ) ){
		return;
	}
	
	pClampMax = clamp;
	pClampMin.SetSmallest( clamp );
	OnClampChanged();
}



void igdeViewCurveBezier::ResetView(){
	OnResetView();
}

void igdeViewCurveBezier::FitViewToCurve(){
	OnFitToCurve();
}



void igdeViewCurveBezier::ShowContextMenu( const decPoint &position ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeMenuCascadeReference menu;
	menu.TakeOver( new igdeMenuCascade( GetEnvironment() ) );
	
	// view supported menu entries
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, new cActionResetView( *this ), true );
	helper.MenuCommand( menu, new cActionFitToCurve( *this ), true );
	
	helper.MenuSeparator( menu );
	helper.MenuOption( menu, new cActionSetInterpolationMode( *this, decCurveBezier::eimConstant,
		"Constant Interpolation", NULL, "Set curve to constant interpolation" ), true );
	helper.MenuOption( menu, new cActionSetInterpolationMode( *this, decCurveBezier::eimLinear,
		"Linear Interpolation", NULL, "Set curve to linear interpolation" ), true );
	helper.MenuOption( menu, new cActionSetInterpolationMode( *this, decCurveBezier::eimBezier,
		"Bezier Interpolation", NULL, "Set curve to bezier interpolation" ), true );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, new cActionCopyCurve( *this ), true );
	helper.MenuCommand( menu, new cActionPasteCurve( *this ), true );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, new cActionClearCurve( *this ), true );
	helper.MenuCommand( menu, new cActionSetDefaultConstant( *this ), true );
	helper.MenuCommand( menu, new cActionSetDefaultLinear( *this ), true );
	helper.MenuCommand( menu, new cActionSetDefaultBezier( *this ), true );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, new cActionInvertCurveX( *this ), true );
	helper.MenuCommand( menu, new cActionInvertCurveY( *this ), true );
	helper.MenuCommand( menu, new cActionAutoHandles( *this ), true );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, new cActionEditClamp( *this ), true );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, new cActionEditSelectedPoint( *this ), true );
	
	// listener supported menu entries
	const int count = pListeners.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeViewCurveBezierListener* )pListeners.GetAt( i ) )->AddContextMenuEntries( this, menu );
	}
	
	if( menu->GetChildCount() > 0 ){
		menu->Popup( *this, position );
	}
}



void igdeViewCurveBezier::AddListener( igdeViewCurveBezierListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeViewCurveBezier::RemoveListener( igdeViewCurveBezierListener *listener ){
	pListeners.Remove( listener );
}

void igdeViewCurveBezier::NotifyCurveChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeViewCurveBezierListener* )listeners.GetAt( i ) )->OnCurveChanged( this );
	}
}

void igdeViewCurveBezier::NotifyCurveChanging(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeViewCurveBezierListener* )listeners.GetAt( i ) )->OnCurveChanging( this );
	}
}

void igdeViewCurveBezier::NotifyCurveSelectPoint(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeViewCurveBezierListener* )listeners.GetAt( i ) )->OnSelectedPointChanged( this );
	}
}



void igdeViewCurveBezier::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeViewCurveBezier * const native = igdeNativeViewCurveBezier::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeViewCurveBezier::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeViewCurveBezier* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}

void igdeViewCurveBezier::OnDefaultSizeChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeViewCurveBezier* )GetNativeWidget() )->GetView().UpdateDefaultSize();
}

void igdeViewCurveBezier::OnCurveChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeViewCurveBezier* )GetNativeWidget() )->GetView().UpdateCurve();
}

void igdeViewCurveBezier::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeViewCurveBezier* )GetNativeWidget() )->GetView().UpdateEnabled();
}

void igdeViewCurveBezier::OnSelectedPointChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeViewCurveBezier* )GetNativeWidget() )->GetView().UpdateSelectedPoint();
}

void igdeViewCurveBezier::OnClampChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeViewCurveBezier* )GetNativeWidget() )->GetView().UpdateClamp();
}

void igdeViewCurveBezier::OnResetView(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeViewCurveBezier* )GetNativeWidget() )->GetView().ResetView();
}

void igdeViewCurveBezier::OnFitToCurve(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeViewCurveBezier* )GetNativeWidget() )->GetView().FitViewToCurve();
}
