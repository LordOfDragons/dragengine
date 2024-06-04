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
#include <stdint.h>
#include <string.h>

#include "igdeNativeFoxNVBoard.h"
#include "igdeNativeFoxNVNode.h"
#include "igdeNativeFoxNVSlot.h"
#include "../../../igdeCommonDialogs.h"
#include "../../../nodeview/igdeNVBoard.h"
#include "../../../nodeview/igdeNVLink.h"
#include "../../../nodeview/igdeNVSlot.h"
#include "../../../nodeview/igdeNVNode.h"
#include "../../../theme/igdeGuiTheme.h"
#include "../../../theme/propertyNames.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



FXDEFMAP( igdeNativeFoxNVBoard ) cNativeIgdeNVBoardMap[] = {
	FXMAPFUNC( SEL_PAINT, 0, igdeNativeFoxNVBoard::onPaint ),
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxNVBoard::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxNVBoard::onChildLayoutFlags ),
	FXMAPFUNC( SEL_LEFTBUTTONPRESS, 0, igdeNativeFoxNVBoard::onLeftMousePress ),
	FXMAPFUNC( SEL_LEFTBUTTONRELEASE, 0, igdeNativeFoxNVBoard::onLeftMouseRelease ),
	FXMAPFUNC( SEL_MOTION, 0, igdeNativeFoxNVBoard::onMouseMoved ),
	FXMAPFUNC( SEL_RIGHTBUTTONPRESS, 0, igdeNativeFoxNVBoard::onRightMousePress ),
	FXMAPFUNC( SEL_RIGHTBUTTONRELEASE, 0, igdeNativeFoxNVBoard::onRightMouseRelease )
};


FXIMPLEMENT( igdeNativeFoxNVBoard, FXPacker,
	cNativeIgdeNVBoardMap, ARRAYNUMBER( cNativeIgdeNVBoardMap ) )


// class igdeNativeFoxNVBoard
///////////////////////////////

igdeNativeFoxNVBoard::igdeNativeFoxNVBoard(){ }

igdeNativeFoxNVBoard::igdeNativeFoxNVBoard( igdeNVBoard &powner, FXComposite *pparent, const igdeGuiTheme & ) :
FXPacker( pparent, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &powner ),
pDoubleBuffer( NULL ),
pCreateLinkSource( NULL ),
pCreateLinkTarget( NULL ),
pIsDragBoard( false )
{
	enable();
	
	UpdateColors();
}

igdeNativeFoxNVBoard::~igdeNativeFoxNVBoard(){
	if( pDoubleBuffer ){
		delete pDoubleBuffer;
	}
}

igdeNativeFoxNVBoard *igdeNativeFoxNVBoard::CreateNativeWidget( igdeNVBoard &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxNVBoard( powner, pparent, *powner.GetGuiTheme() );
}

void igdeNativeFoxNVBoard::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxNVBoard::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

FXbool igdeNativeFoxNVBoard::canFocus() const{
	return false;
}



void igdeNativeFoxNVBoard::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxNVBoard::UpdateColors(){
	setBackColor( igdeUIFoxHelper::ConvertColor( pOwner->GetBgColor() ) );
}

void igdeNativeFoxNVBoard::UpdateNodes(){
	update();
}

void igdeNativeFoxNVBoard::UpdateLinks(){
	update();
}

void igdeNativeFoxNVBoard::UpdateOffset(){
	update();
}

decPoint igdeNativeFoxNVBoard::GetSize(){
	return decPoint( getWidth(), getWidth() );
}



void igdeNativeFoxNVBoard::BeginCreateLink( igdeNativeFoxNVSlot *source ){
	SetHoverLink( NULL );
	pCreateLinkSource = source;
	pCreateLinkTarget = NULL;
	pCreateLinkPosition = source->GetCenterBoard();
	update();
}

void igdeNativeFoxNVBoard::SetCreateLinkPosition( const decPoint &position ){
	pCreateLinkPosition = position;
	update();
}

void igdeNativeFoxNVBoard::SetCreateLinkTarget( igdeNativeFoxNVSlot *ttarget ){
	pCreateLinkTarget = ttarget;
	update();
}

void igdeNativeFoxNVBoard::FinishCreateLink(){
	if( pCreateLinkSource && pCreateLinkTarget
	&& pOwner->CanLink( &pCreateLinkSource->GetOwner(), &pCreateLinkTarget->GetOwner() ) ){
		igdeNVLink * const link = pOwner->AddLink( &pCreateLinkSource->GetOwner(), &pCreateLinkTarget->GetOwner() );
		pCreateLinkSource = NULL;
		pCreateLinkTarget = NULL;
		update();
		
		pOwner->NotifyLinkAdded( link );
		
	}else{
		pCreateLinkSource = NULL;
		pCreateLinkTarget = NULL;
		update();
	}
}

igdeNVLink *igdeNativeFoxNVBoard::ClosestLinkNear( const decPoint &position, float range ) const{
	const int count = pOwner->GetLinkCount();
	igdeNVLink *bestLink = NULL;
	float bestDistance = 0.0f;
	int i;
	
	for( i=0; i<count; i++ ){
		igdeNVLink * const link = pOwner->GetLinkAt( i );
		
		const decPoint positionSource( link->GetSource()->GetConnectorBoard() );
		const decPoint positionTarget( link->GetTarget()->GetConnectorBoard() );
		
		const decPoint p1( positionSource );
		const decPoint p4( positionTarget );
		const int offset = abs( p4.x - p1.x ) >> 1;
		const decPoint p2( p1.x + offset, p1.y );
		const decPoint p3( p4.x - offset, p4.y );
		
		const float distance = PointBezierDistance( position, p1, p2, p3, p4 );
		if( distance > range ){
			continue;
		}
		
		if( ! bestLink || distance <= bestDistance ){
			bestLink = link;
			bestDistance = distance;
		}
	}
	
	return bestLink;
}

void igdeNativeFoxNVBoard::SetHoverLink( igdeNVLink *link ){
	if( pHoverLink == link ){
		return;
	}
	
	pHoverLink = link;
	update();
}



// Events
///////////

long igdeNativeFoxNVBoard::onPaint( FXObject*, FXSelector, void *pdata ){
	if( pDoubleBuffer ){
		{
		FXDCWindow dcdb( pDoubleBuffer );
		dcdb.setForeground( getBackColor() );
		dcdb.fillRectangle( 0, 0, getWidth(), getHeight() );
		
		DrawLinks( dcdb );
		DrawCreateLink( dcdb );
		}
		
		FXDCWindow dcw( this, ( FXEvent* )pdata );
		dcw.drawImage( pDoubleBuffer, 0, 0 );
		
	}else{
		FXDCWindow dcw( this, ( FXEvent* )pdata );
		dcw.setForeground( getBackColor() );
		dcw.fillRectangle( 0, 0, getWidth(), getHeight() );
		
		DrawLinks( dcw );
		DrawCreateLink( dcw );
	}
	return 1;
}

long igdeNativeFoxNVBoard::onResize( FXObject*, FXSelector, void* ){
	if( pDoubleBuffer ){
		pDoubleBuffer->resize( getWidth(), getHeight() );
		
	}else{
		pDoubleBuffer = new FXImage( getApp(), NULL, 0, getWidth(), getHeight() );
		pDoubleBuffer->create();
	}
	
	pOwner->NotifyNodesOffsetChanged();
	pOwner->NotifySizeChanged();
	return 1;
}

long igdeNativeFoxNVBoard::onChildLayoutFlags( FXObject*, FXSelector, void *pdata ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )pdata );
	clflags.flags = 0;
	clflags.canResizeHorizontal = true;
	clflags.canResizeVertical = true;
	return 1;
}

long igdeNativeFoxNVBoard::onLeftMousePress( FXObject*, FXSelector, void *pdata ){
	if( pIsDragBoard ){
		return 1;
	}
	
	const FXEvent &event = *( ( FXEvent* )pdata );
	const bool shift = ( event.state & SHIFTMASK ) == SHIFTMASK;
	const bool control = ( event.state & CONTROLMASK ) == CONTROLMASK;
	
	if( shift && ! control ){
		SetHoverLink( NULL );
		pDragBoard.Set( event.win_x, event.win_y );
		pIsDragBoard = true;
		setDragCursor( getApp()->getDefaultCursor( DEF_MOVE_CURSOR ) );
	}
	
	return 1;
}

long igdeNativeFoxNVBoard::onLeftMouseRelease( FXObject*, FXSelector, void* ){
	if( pIsDragBoard ){
		setDragCursor( getApp()->getDefaultCursor( DEF_ARROW_CURSOR ) );
		pIsDragBoard = false;
	}
	
	return 1;
}

long igdeNativeFoxNVBoard::onMouseMoved( FXObject*, FXSelector, void *pdata ){
	const FXEvent &event = *( ( FXEvent* )pdata );
	const decPoint position( event.win_x, event.win_y );
	
	if( pIsDragBoard ){
		const decPoint distance( position - pDragBoard );
		pDragBoard = position;
		
		pOwner->SetOffset( pOwner->GetOffset() + distance );
		pOwner->NotifyOffsetChanged();
		
	}else{
		SetHoverLink( pOwner->ClosestLinkNear( position ) );
	}
	
	return 1;
}

long igdeNativeFoxNVBoard::onRightMousePress( FXObject*, FXSelector, void *pdata ){
	if( pIsDragBoard ){
		return 1;
	}
	
	const FXEvent &event = *( ( FXEvent* )pdata );
	const decPoint position( event.win_x, event.win_y );
	SetHoverLink( pOwner->ClosestLinkNear( position ) );
	pOwner->ShowContextMenu( position );
	return 1;
}

long igdeNativeFoxNVBoard::onRightMouseRelease( FXObject*, FXSelector, void* ){
	return 1;
}



void igdeNativeFoxNVBoard::DrawLinks( FXDC &dc ) const{
	const FXColor colorLink = FXRGB( 0, 0, 128 );
	const FXColor colorHover = FXRGB( 128, 128, 255 );
	const int count = pOwner->GetLinkCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeNVLink * const link = pOwner->GetLinkAt( i );
		
		const decPoint positionSource( link->GetSource()->GetConnectorBoard() );
		const decPoint positionTarget( link->GetTarget()->GetConnectorBoard() );
		
		const decPoint p1( positionSource );
		const decPoint p4( positionTarget );
		const int offset = abs( p4.x - p1.x ) >> 1;
		const decPoint p2( p1.x + offset, p1.y );
		const decPoint p3( p4.x - offset, p4.y );
		
		const FXColor color = pHoverLink == link ? colorHover : colorLink;
		
		DrawBezier( dc, color, p1, p2, p3, p4 );
	}
}

void igdeNativeFoxNVBoard::DrawCreateLink( FXDC &dc ) const{
	if( ! pCreateLinkSource ){
		return;
	}
	
	decPoint positionTarget;
	FXColor colorLink;
	
	if( pCreateLinkTarget ){
		positionTarget = pCreateLinkTarget->GetConnectorBoard();
		
		if( pOwner->CanLink( &pCreateLinkSource->GetOwner(), &pCreateLinkTarget->GetOwner() ) ){
			colorLink = FXRGB( 0, 192, 0 );
			
		}else{
			colorLink = FXRGB( 255, 0, 0 );
		}
		
	}else{
		positionTarget = pCreateLinkPosition;
		colorLink = FXRGB( 192, 192, 0 );
	}
	
	const decPoint positionSource( pCreateLinkSource->GetConnectorBoard() );
	
	const decPoint p1( positionSource );
	const decPoint p4( positionTarget );
	const int offset = abs( p4.x - p1.x ) >> 1;
	const decPoint p2( p1.x + offset, p1.y );
	const decPoint p3( p4.x - offset, p4.y );
	
	DrawBezier( dc, colorLink, p1, p2, p3, p4 );
}

void igdeNativeFoxNVBoard::DrawBezier( FXDC &dc, FXColor color, const decPoint &p1,
const decPoint &p2, const decPoint &p3, const decPoint &p4 ) const{
	//const FXColor color2 = igdeUIFoxHelper::BrighterColor( color, 64 );
	const FXColor color2 = igdeUIFoxHelper::BlendColor( color, FXRGB( 255, 255, 255 ), 0.5f );
	
	dc.setForeground( color2 );
	DrawBezier( dc, p1, p2, p3, p4 );
	
	dc.setForeground( color );
	const decPoint o1( 0, -1 );
	DrawBezier( dc, p1 + o1, p2 + o1, p3 + o1, p4 + o1 );
	
	const decPoint o2( 0, 1 );
	DrawBezier( dc, p1 + o2, p2 + o2, p3 + o2, p4 + o2 );
}

void igdeNativeFoxNVBoard::DrawBezier( FXDC &dc, const decPoint &p1, const decPoint &p2,
const decPoint &p3, const decPoint &p4 ) const{
	const decVector2 bp1( ( float )p1.x - 0.5f, ( float )p1.y - 0.5f );
	const decVector2 bp2( ( float )p2.x - 0.5f, ( float )p2.y - 0.5f );
	const decVector2 bp3( ( float )p3.x - 0.5f, ( float )p3.y - 0.5f );
	const decVector2 bp4( ( float )p4.x - 0.5f, ( float )p4.y - 0.5f );
	
	InternalDrawBezier( dc, bp1, bp2, bp3, bp4 );
}

void igdeNativeFoxNVBoard::InternalDrawBezier( FXDC &dc, const decVector2 &p1,
const decVector2 &p2, const decVector2 &p3, const decVector2 &p4 ) const{
	decVector2 diff = p2 - p1;
	float length = diff * diff;
	diff = p3 - p2;
	length += diff * diff;
	diff = p4 - p3;
	length += diff * diff;
	
	if( length < 60.0f ){
		dc.drawLine( ( int )p1.x, ( int )p1.y, ( int )p2.x, ( int )p2.y );
		dc.drawLine( ( int )p2.x, ( int )p2.y, ( int )p3.x, ( int )p3.y );
		dc.drawLine( ( int )p3.x, ( int )p3.y, ( int )p4.x, ( int )p4.y );
		return;
	}
	
	const decVector2 np1 = ( p1 + p2 ) * 0.5f;
	const decVector2 np2 = ( p2 + p3 ) * 0.5f;
	const decVector2 np3 = ( p3 + p4 ) * 0.5f;
	const decVector2 np4 = ( np1 + np2 ) * 0.5f;
	const decVector2 np5 = ( np2 + np3 ) * 0.5f;
	const decVector2 np6 = ( np4 + np5 ) * 0.5f;
	
	InternalDrawBezier( dc, p1, np1, np4, np6 );
	InternalDrawBezier( dc, np6, np5, np3, p4 );
}

bool igdeNativeFoxNVBoard::LineIntersectsBezier( const decPoint &lp1, const decPoint &lp2,
const decPoint &bp1, const decPoint &bp2, const decPoint &bp3, const decPoint &bp4 ) const{
	const decVector2 nlp1( ( float )lp1.x, ( float )lp1.y );
	const decVector2 nlp2( ( float )lp2.x, ( float )lp2.y );
	const decVector2 nbp1( ( float )bp1.x, ( float )bp1.y );
	const decVector2 nbp2( ( float )bp2.x, ( float )bp2.y );
	const decVector2 nbp3( ( float )bp3.x, ( float )bp3.y );
	const decVector2 nbp4( ( float )bp4.x, ( float )bp4.y );
	
	decVector2 ldir( nlp2 - nlp1 );
	float llen = ldir.Length();
	ldir /= llen;
	
	return InternalLineIntersectsBezier( ldir, llen, nlp1, nbp1, nbp2, nbp3, nbp4 );
}

bool igdeNativeFoxNVBoard::InternalLineIntersectsBezier( const decVector2 &ldir, float llen,
const decVector2 &lpos, const decVector2 &bp1, const decVector2 &bp2, const decVector2 &bp3,
const decVector2 &bp4 ) const{
	const decVector2 lnor( ldir.y, -ldir.x );
	const float ldot = lnor * lpos;
	const bool side1 = lnor * bp1 > ldot;
	const bool side2 = lnor * bp2 > ldot;
	const bool side3 = lnor * bp3 > ldot;
	const bool side4 = lnor * bp4 > ldot;
	
	if( side1 == side2 && side2 == side3 && side3 == side4 ){
		return false;
	}
	
	decVector2 diff = bp2 - bp1;
	float length = diff * diff;
	diff = bp3 - bp2;
	length += diff * diff;
	diff = bp4 - bp3;
	length += diff * diff;
	
	if( length < 60.0f ){
		float denom, loc1, loc2, tlen, tloc;
		decVector2 tp1, tp2, tp3;
		
		if( side1 != side2 ){
			tp1 = bp1;
			tp2 = bp2;
			
		}else if( side2 != side3 ){
			tp1 = bp2;
			tp2 = bp3;
			
		}else{
			tp1 = bp3;
			tp2 = bp4;
		}
		
		denom = lnor * ( tp2 - tp1 );
		if( fabsf( denom ) < 0.001f ){
			if( fabsf( lnor * tp1 - ldot ) < 0.5f ){
				loc1 = ldir * ( tp1 - lpos );
				loc2 = ldir * ( tp2 - lpos );
				return ! ( ( loc1 < 0.5f && loc2 < 0.5f ) || ( loc1 > llen + 0.5f && loc2 > llen + 0.5f ) );
			}
			
		}else{
			tlen = ( tp2 - tp1 ).Length();
			tp3 = tp1 + ( tp2 - tp1 ) * ( ( ldot - lnor * tp1 ) / denom );
			tloc = ( tp3 - tp1 ).Length();
			
			if( tloc > -0.5f && tloc < tlen + 0.5f ){
				loc1 = ldir * ( tp3 - lpos );
				return loc1 > -0.5f && loc1 < llen + 0.5f;
			}
		}
		
	}else{
		const decVector2 nbp1 = ( bp1 + bp2 ) * 0.5f;
		const decVector2 nbp2 = ( bp2 + bp3 ) * 0.5f;
		const decVector2 nbp3 = ( bp3 + bp4 ) * 0.5f;
		const decVector2 nbp4 = ( nbp1 + nbp2 ) * 0.5f;
		const decVector2 nbp5 = ( nbp2 + nbp3 ) * 0.5f;
		const decVector2 nbp6 = ( nbp4 + nbp5 ) * 0.5f;
		
		if( InternalLineIntersectsBezier( ldir, llen, lpos, bp1, nbp1, nbp4, nbp6 ) ){
			return true;
		}
		if( InternalLineIntersectsBezier( ldir, llen, lpos, nbp6, nbp5, nbp3, bp4 ) ){
			return true;
		}
	}
	
	return false;
}

float igdeNativeFoxNVBoard::PointBezierDistance( const decPoint &p, const decPoint &bp1,
const decPoint &bp2, const decPoint &bp3, const decPoint &bp4 ) const{
	return PointBezierDistance( decVector2( p ), decVector2( bp1 ),
		decVector2( bp2 ), decVector2( bp3 ), decVector2( bp4 ) );
}

static float fPointLineDistance( const decVector2 &bp, const decVector2 &bp1, const decVector2 &bp2 ){
	decVector2 normal( bp2 - bp1 );
	const float length = normal.Length();
	if( length <= 1.0 ){
		return ( bp - bp1 ).Length();
	}
	
	normal /= length;
	const float distance = normal * ( bp - bp1 );
	if( distance < -0.5 || distance > length + 0.5 ){
		return INFINITY;
	}
	
	const decVector2 closest( bp1 + normal * distance );
	return ( bp - closest ).Length();
}

float igdeNativeFoxNVBoard::PointBezierDistance( const decVector2 &bp, const decVector2 &bp1,
const decVector2 &bp2, const decVector2 &bp3, const decVector2 &bp4 ) const{
	decVector2 diff( bp2 - bp1 );
	float length = diff * diff;
	diff = bp3 - bp2;
	length += diff * diff;
	diff = bp4 - bp3;
	length += diff * diff;
	
	if( length < 60.0f ){
		return decMath::min( fPointLineDistance( bp, bp1, bp2 ),
			fPointLineDistance( bp, bp2, bp3 ), fPointLineDistance( bp, bp3, bp4 ) );
		
	}else{
		const decVector2 nbp1( ( bp1 + bp2 ) * 0.5f );
		const decVector2 nbp2( ( bp2 + bp3 ) * 0.5f );
		const decVector2 nbp3( ( bp3 + bp4 ) * 0.5f );
		const decVector2 nbp4( ( nbp1 + nbp2 ) * 0.5f );
		const decVector2 nbp5( ( nbp2 + nbp3 ) * 0.5f );
		const decVector2 nbp6( ( nbp4 + nbp5 ) * 0.5f );
		
		return decMath::min( PointBezierDistance( bp, bp1, nbp1, nbp4, nbp6 ),
			PointBezierDistance( bp, nbp6, nbp5, nbp3, bp4 ) );
	}
}

#endif
