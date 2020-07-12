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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "igdeNativeFoxNVNode.h"
#include "igdeNativeFoxNVSlot.h"
#include "igdeNativeFoxNVBoard.h"
#include "../../../igdeCommonDialogs.h"
#include "../../../nodeview/igdeNVBoard.h"
#include "../../../nodeview/igdeNVNode.h"
#include "../../../resources/igdeIcon.h"
#include "../../../resources/igdeFont.h"
#include "../../../theme/igdeGuiTheme.h"
#include "../../../theme/propertyNames.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



FXDEFMAP( igdeNativeFoxNVNode ) cNativeIgdeNVNodeMap[] = {
	FXMAPFUNC( SEL_LEFTBUTTONPRESS, igdeNativeFoxNVNode::ID_LAB_TITLE, igdeNativeFoxNVNode::onTitleLeftMouseDown ),
	FXMAPFUNC( SEL_LEFTBUTTONPRESS, igdeNativeFoxNVNode::ID_FRA_TITLE, igdeNativeFoxNVNode::onTitleLeftMouseDown ),
	FXMAPFUNC( SEL_LEFTBUTTONRELEASE, igdeNativeFoxNVNode::ID_LAB_TITLE, igdeNativeFoxNVNode::onTitleLeftMouseUp ),
	FXMAPFUNC( SEL_LEFTBUTTONRELEASE, igdeNativeFoxNVNode::ID_FRA_TITLE, igdeNativeFoxNVNode::onTitleLeftMouseUp ),
	FXMAPFUNC( SEL_RIGHTBUTTONPRESS, igdeNativeFoxNVNode::ID_LAB_TITLE, igdeNativeFoxNVNode::onRightMousePress ),
	FXMAPFUNC( SEL_RIGHTBUTTONPRESS, igdeNativeFoxNVNode::ID_FRA_TITLE, igdeNativeFoxNVNode::onRightMousePress ),
	FXMAPFUNC( SEL_RIGHTBUTTONRELEASE, igdeNativeFoxNVNode::ID_LAB_TITLE, igdeNativeFoxNVNode::onRightMouseRelease ),
	FXMAPFUNC( SEL_RIGHTBUTTONRELEASE, igdeNativeFoxNVNode::ID_FRA_TITLE, igdeNativeFoxNVNode::onRightMouseRelease ),
	FXMAPFUNC( SEL_MOTION, igdeNativeFoxNVNode::ID_LAB_TITLE, igdeNativeFoxNVNode::onTitleMouseMove ),
	FXMAPFUNC( SEL_MOTION, igdeNativeFoxNVNode::ID_FRA_TITLE, igdeNativeFoxNVNode::onTitleMouseMove ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxNVNode::onChildLayoutFlags )
};


FXIMPLEMENT( igdeNativeFoxNVNode, FXVerticalFrame,
	cNativeIgdeNVNodeMap, ARRAYNUMBER( cNativeIgdeNVNodeMap ) )


// class igdeNativeFoxNVNode
///////////////////////////////

igdeNativeFoxNVNode::igdeNativeFoxNVNode(){ }

igdeNativeFoxNVNode::igdeNativeFoxNVNode( igdeNVNode &owner, FXComposite *parent, const igdeGuiTheme &guitheme ) :
FXVerticalFrame( parent, NVNodeFlags( owner ), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &owner ),
pFont( NVNodeFont( owner, guitheme ) ),
pLabTitle( NULL ),
pFrameTitle( NULL ),
pFrameSlots( NULL ),

pTitleIsDraging( false ),

pIsLDraging( false ),
pIsRDraging( false ),
pDragShift( false ),
pDragControl( false )
{
	pFrameTitle = new FXHorizontalFrame( this, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	pFrameTitle->setTarget( this );
	pFrameTitle->setSelector( ID_FRA_TITLE );
	pFrameTitle->enable();
	
	pLabTitle = new FXLabel( pFrameTitle, owner.GetTitle().GetString(), 0,
		LAYOUT_FILL_X | LAYOUT_FILL_Y | JUSTIFY_CENTER_X | JUSTIFY_CENTER_Y );
	pLabTitle->setFont( (FXFont*)pFont->GetNativeFont() );
	pLabTitle->setTarget( this );
	pLabTitle->setSelector( ID_LAB_TITLE );
	
	pFrameSlots = new FXVerticalFrame( this, LAYOUT_FILL_X | LAYOUT_FILL_Y,
		0, 0, 0, 0,
		NVNodePadLeft( guitheme ), NVNodePadRight( guitheme ),
		NVNodePadTop( guitheme ), NVNodePadBottom( guitheme ),
		0, 0 );
	pFrameSlots->setTarget( this );
	pFrameSlots->setSelector( ID_FRA_SLOTS );
	
	enable();
	
	UpdateTitle();
	UpdateDescription();
	UpdateEnabled();
	UpdateColors();
	UpdatePosition();
}

igdeNativeFoxNVNode::~igdeNativeFoxNVNode(){
}

igdeNativeFoxNVNode *igdeNativeFoxNVNode::CreateNativeWidget( igdeNVNode &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxNVNode( owner, parent, *owner.GetGuiTheme() );
}

void igdeNativeFoxNVNode::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
		create();
	}
}

void igdeNativeFoxNVNode::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

FXbool igdeNativeFoxNVNode::canFocus() const{
	return false;
}



void igdeNativeFoxNVNode::UpdateTitle(){
	pLabTitle->setText( pOwner->GetTitle().GetString() );
}

void igdeNativeFoxNVNode::UpdateDescription(){
	pLabTitle->setTipText( pOwner->GetDescription().GetString() );
	pLabTitle->setHelpText( pOwner->GetDescription().GetString() );
}

void igdeNativeFoxNVNode::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxNVNode::UpdateActive(){
	UpdateColors();
}

void igdeNativeFoxNVNode::UpdateColors(){
	const bool isActive = pOwner->GetActive();
	const FXColor bgColor = igdeUIFoxHelper::ConvertColor( pOwner->GetBgColor() );
	const FXColor borderColor = igdeUIFoxHelper::ConvertColor( pOwner->GetBorderColor() );
	const FXColor titleBgColor = igdeUIFoxHelper::ConvertColor( isActive
		? pOwner->GetActiveTitleBgColor() : pOwner->GetInactiveTitleBgColor() );
	
	setBackColor( bgColor );
	setBorderColor( borderColor );
	
	pFrameTitle->setBackColor( titleBgColor );
	
	pLabTitle->setTextColor( FXRGB( 0, 0, 0 ) );
	pLabTitle->setBackColor( titleBgColor );
	
	pFrameSlots->setBackColor( bgColor );
}

void igdeNativeFoxNVNode::UpdatePosition(){
	decPoint position( pOwner->GetPosition() );
	if( pOwner->GetOwnerBoard() ){
		position += pOwner->GetOwnerBoard()->GetSize() / 2 + pOwner->GetOwnerBoard()->GetOffset();
	}
	move( position.x, position.y );
	
	if( getParent() ){
		getParent()->update();
	}
}

void igdeNativeFoxNVNode::FitSizeToContent(){
	const int width = FXVerticalFrame::getDefaultWidth() + 20;
	const int height = FXVerticalFrame::getDefaultHeight();
	resize( width, height );
	recalc();
}

decPoint igdeNativeFoxNVNode::GetSize(){
	return decPoint( getWidth(), getWidth() );
}



int igdeNativeFoxNVNode::NVNodeFlags( const igdeNVNode & ){
	return LAYOUT_FIX_X | LAYOUT_FIX_Y | LAYOUT_FIX_WIDTH | LAYOUT_FIX_HEIGHT | FRAME_RAISED;
}

igdeFont *igdeNativeFoxNVNode::NVNodeFont( const igdeNVNode &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::nodeViewNodeFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::nodeViewNodeFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::nodeViewNodeFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::nodeViewNodeFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}

int igdeNativeFoxNVNode::NVNodePadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::nodeViewNodePaddingLeft, 0 );
}

int igdeNativeFoxNVNode::NVNodePadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::nodeViewNodePaddingRight, 0 );
}

int igdeNativeFoxNVNode::NVNodePadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::nodeViewNodePaddingTop, 2 );
}

int igdeNativeFoxNVNode::NVNodePadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::nodeViewNodePaddingBottom, 5 );
}



// Events
///////////

long igdeNativeFoxNVNode::onTitleLeftMouseDown( FXObject *sender, FXSelector, void *data ){
	if( pTitleIsDraging ){
		return 1;
	}
	
	if( pOwner->GetOwnerBoard() ){
		pOwner->GetOwnerBoard()->SetActiveNode( pOwner );
	}
	
	const FXEvent &event = *( ( FXEvent* )data );
	FXWindow * const widget = ( FXWindow* )sender;
	
	FXint px, py;
	translateCoordinatesFrom( px, py, widget, event.win_x, event.win_y );
	pTitleDragOffset.x = px;
	pTitleDragOffset.y = py;
	
	widget->grab();
	pTitleIsDraging = true;
	pOwner->NotifyDragBegin();
	return 1;
}

long igdeNativeFoxNVNode::onTitleMouseMove( FXObject *sender, FXSelector selector, void *data ){
	if( ! pTitleIsDraging ){
		return 1;
	}
	
	const FXEvent &event = *( ( FXEvent* )data );
	const decPoint dragCur( event.win_x + getX(), event.win_y + getY() );
	
	decPoint position( dragCur - pTitleDragOffset );
	if( pOwner->GetOwnerBoard() ){
		position -= pOwner->GetOwnerBoard()->GetSize() / 2 + pOwner->GetOwnerBoard()->GetOffset();
	}
	pOwner->SetPosition( position );
	pOwner->NotifyDraging();
	
	if( pOwner->GetOwnerBoard() && pOwner->GetOwnerBoard()->GetNativeWidget() ){
		( ( FXWindow* )pOwner->GetOwnerBoard()->GetNativeWidget() )->update();
	}
	
	/*
	float pixelToUnis = 1.0f / 100.0f; // UnitsToPixel=100
	FXint mouseInParentX, mouseInParentY;
	
	getParent()->translateCoordinatesFrom( mouseInParentX, mouseInParentY,
		getApp()->getRootWindow(), event.root_x, event.root_y );
	
	const decVector2 nposition(
		( float )( mouseInParentX - pMouseToNodeOffset.x - ( getParent()->getWidth() >> 1 ) ) * pixelToUnis,
		( float )( mouseInParentY - pMouseToNodeOffset.y - ( getParent()->getHeight() >> 1 ) ) * pixelToUnis );
	
	pUndoMoveRule->SetNewPosition( nposition - pWindowVegetation->GetVLayer()->GetViewCenter() );
	pUndoMoveRule->RedoAction();
	*/
	
	return 1;
}

long igdeNativeFoxNVNode::onTitleLeftMouseUp( FXObject *sender, FXSelector selector, void *data ){
	if( ! pTitleIsDraging ){
		return 1;
	}
	
	FXWindow * const widget = ( FXWindow* )sender;
	
	pTitleIsDraging = false;
	widget->ungrab();
	pOwner->NotifyDragEnd();
	
	return 1;
}

long igdeNativeFoxNVNode::onRightMousePress( FXObject*, FXSelector, void *data ){
	if( pTitleIsDraging ){
		return 1;
	}
	
	const FXEvent &event = *( ( FXEvent* )data );
	pOwner->ShowContextMenu( decPoint( event.win_x, event.win_y ) );
	return 1;
}

long igdeNativeFoxNVNode::onRightMouseRelease( FXObject*, FXSelector, void* ){
	return 1;
}

long igdeNativeFoxNVNode::onChildLayoutFlags( FXObject*, FXSelector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_FILL_X;
	clflags.canResizeHorizontal = false;
	clflags.canResizeVertical = false;
	return 1;
}



/*
void meWVNode::UpdateWindowShape(){
	if( pDirtyWindowShape ){
		// make sure the window shape exists and is created. should this fail for
		// some reason we simply ignore the shape which causes garbage to show
		// around the node but it would not prevent usage of the editor
		if( ! pWindowShape ){
			pWindowShape = new FXBitmap( getApp() );
			if( pWindowShape ){
				pWindowShape->create();
				setShape( pWindowShape );
			}
		}
		
		// if the window shape exists update it by drawing the node frame ontop of it
		if( pWindowShape ){
			clearShape();
			
			pWindowShape->resize( getWidth(), getHeight() );
			
			FXDCWindow dc( pWindowShape );
			{ // just to make sure the dc is killed before we attempt to render the bitmap
			int width = getWidth();
			int height = getHeight();
			int cornerRadius = 10;
			int slotCircleRadius = 4;
			int nodeLeft = slotCircleRadius;
			int nodeTop = slotCircleRadius;
			int nodeRight = width - 1 - slotCircleRadius;
			int nodeBottom = height - 1 - slotCircleRadius;
			int nodeWidth = nodeRight - nodeLeft + 1;
			int nodeHeight = nodeBottom - nodeTop + 1;
			
			dc.setForeground( FXRGB( 0, 0, 0 ) );
			dc.fillRectangle( 0, 0, width, height );
			dc.setForeground( FXRGB( 255, 255, 255 ) );
			dc.fillRoundRectangle( nodeLeft, nodeTop, nodeWidth, nodeHeight, cornerRadius, cornerRadius );
			dc.drawRoundRectangle( nodeLeft, nodeTop, nodeWidth, nodeHeight, cornerRadius, cornerRadius );
			}
			pWindowShape->render();
			
			setShape( pWindowShape );
		}
		
		// no more dirty
		pDirtyWindowShape = false;
	}
}
*/

/*
long meWVNode::onPaint( FXObject *sender, FXSelector selector, void *data ){
	meHTVegetationLayer *vlayer = pWindowVegetation->GetVLayer();
	FXEvent *event = ( FXEvent* )data;
	FXDCWindow dc( this, event );
	
	//UpdateWindowShape();
	
	int width = getWidth();
	int height = getHeight();
//	int cornerRadius = 10;
//	int slotCircleRadius = 4;
//	int slotCircleSize = slotCircleRadius << 1;
	int nodeLeft = 0; //slotCircleRadius;
	int nodeTop = 0; //slotCircleRadius;
	int nodeRight = width - 1; // - slotCircleRadius;
	int nodeBottom = height - 1; // - slotCircleRadius;
	int nodeWidth = nodeRight - nodeLeft + 1;
	int nodeHeight = nodeBottom - nodeTop + 1;
	FXColor colorText = FXRGB( 0, 0, 0 );
	FXColor colorBg = FXRGB( 150, 150, 150 );
	FXColor colorBorder = FXRGB( 60, 60, 60 );
	int textWidth;
	//int textHeight;
//	int textPosX;
	
//	FXString text;
	
	//dc.clipChildren( false );
	
	dc.setFont( pFont );
	
	if( vlayer->GetActiveRule() == pRule ){
		dc.setForeground( GetActiveTitleBgColor() );
	}else{
		dc.setForeground( GetInactiveTitleBgColor() );
	}
	dc.fillRectangle( nodeLeft, nodeTop, nodeWidth - 1, nodeTop + pTitleBarHeight );
	textWidth = pFont->getTextWidth( pTitle );
	//textHeight = pFont->getTextHeight( pTitle );
	dc.setForeground( colorText );
	dc.drawText( nodeLeft + ( ( nodeWidth - 12 - textWidth ) >> 1 ), nodeTop + pTitleBarHeight - 3, pTitle );
	
	if( pShowParameters ){
		dc.setForeground( FXRGB( 40, 90, 40 ) );
	}else{
		dc.setForeground( FXRGB( 80, 180, 80 ) );
	}
	dc.fillRectangle( nodeRight - 10, nodeTop + 2, nodeRight - 2, nodeTop + 10 );
	
	dc.setForeground( colorBg );
	//dc.fillRoundRectangle( nodeLeft, nodeTop, nodeWidth, nodeHeight, cornerRadius, cornerRadius );
	dc.fillRectangle( nodeLeft, nodeTop + pTitleBarHeight, nodeWidth - 1, nodeHeight - 1 );
	
	dc.setForeground( colorBorder );
	dc.drawLine( nodeLeft, nodeTop + pTitleBarHeight, nodeRight, nodeTop + pTitleBarHeight );
	//dc.drawRoundRectangle( nodeLeft, nodeTop, nodeWidth, nodeHeight, cornerRadius, cornerRadius );
	dc.drawRectangle( nodeLeft, nodeTop, nodeWidth - 1, nodeHeight - 1 );
//	dc.drawLine( nodeLeft + slotCircleSize, nodeTop, nodeLeft + slotCircleSize, nodeHeight );
//	dc.drawLine( nodeRight - slotCircleSize, nodeTop, nodeRight - slotCircleSize, nodeHeight );
	
	return 1;
}
*/
