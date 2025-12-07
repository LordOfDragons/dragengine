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

#include "igdeNativeFoxNVSlot.h"
#include "igdeNativeFoxNVSocket.h"
#include "igdeNativeFoxNVBoard.h"
#include "../../../igdeCommonDialogs.h"
#include "../../../nodeview/igdeNVSlot.h"
#include "../../../nodeview/igdeNVBoard.h"
#include "../../../nodeview/igdeNVNode.h"
#include "../../../resources/igdeIcon.h"
#include "../../../resources/igdeFont.h"
#include "../../../theme/igdeGuiTheme.h"
#include "../../../theme/propertyNames.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



FXDEFMAP(igdeNativeFoxNVSlot) cNativeIgdeNVSlotMap[] = {
	FXMAPFUNC(SEL_PAINT, 0, igdeNativeFoxNVSlot::onPaint),
	FXMAPFUNC(SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxNVSlot::onChildLayoutFlags),
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxNVSlot::ID_SOCKET, igdeNativeFoxNVSlot::onSocketCommand),
	FXMAPFUNC(SEL_LEFTBUTTONPRESS, igdeNativeFoxNVSlot::ID_SOCKET, igdeNativeFoxNVSlot::onSocketLeftMouseDown),
	FXMAPFUNC(SEL_MOTION, igdeNativeFoxNVSlot::ID_SOCKET, igdeNativeFoxNVSlot::onSocketMouseMoved),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE, igdeNativeFoxNVSlot::ID_SOCKET, igdeNativeFoxNVSlot::onSocketLeftMouseUp),
	FXMAPFUNC(SEL_DND_ENTER, igdeNativeFoxNVSlot::ID_SOCKET, igdeNativeFoxNVSlot::onSocketDndEnter),
	FXMAPFUNC(SEL_DND_LEAVE, igdeNativeFoxNVSlot::ID_SOCKET, igdeNativeFoxNVSlot::onSocketDndLeave),
	FXMAPFUNC(SEL_DND_DROP, igdeNativeFoxNVSlot::ID_SOCKET, igdeNativeFoxNVSlot::onSocketDndDrop),
	FXMAPFUNC(SEL_DND_MOTION, igdeNativeFoxNVSlot::ID_SOCKET, igdeNativeFoxNVSlot::onSocketDndMotion)
};


FXIMPLEMENT(igdeNativeFoxNVSlot, FXHorizontalFrame,
	cNativeIgdeNVSlotMap, ARRAYNUMBER(cNativeIgdeNVSlotMap))


// class igdeNativeFoxNVSlot
///////////////////////////////

igdeNativeFoxNVSlot::igdeNativeFoxNVSlot(){}

igdeNativeFoxNVSlot::igdeNativeFoxNVSlot(igdeNVSlot &powner, FXComposite *pparent,
	const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme) :
FXHorizontalFrame(pparent, layoutFlags.flags | NVSlotFlags(powner), 0, 0, 0, 0,
	NVSlotPadLeft(guitheme) + (powner.GetIsInput() ? 0 : 20),
	NVSlotPadRight(guitheme) + (powner.GetIsInput() ? 20 : 0),
	NVSlotPadTop(guitheme), NVSlotPadBottom(guitheme)),
pOwner(&powner),
pFont(NVSlotFont(powner, guitheme)),
pSocket(NULL),
pLabel(NULL),
pDragTypeSocket(pparent->getApp()->registerDragType("application/deigde_nodesystem_socket")),
pIsDnd(false)
{
	pSocket = new igdeNativeFoxNVSocket(this, this, ID_SOCKET, LAYOUT_FILL_Y | (powner.GetIsInput()
		? LAYOUT_SIDE_LEFT | LAYOUT_LEFT : LAYOUT_SIDE_RIGHT | LAYOUT_RIGHT), guitheme);
	
	pLabel = new FXLabel(this, powner.GetText().GetString(), NULL, LAYOUT_FILL_Y | (powner.GetIsInput()
			? JUSTIFY_LEFT | JUSTIFY_CENTER_Y | ICON_BEFORE_TEXT | LAYOUT_SIDE_RIGHT | LAYOUT_LEFT
			: JUSTIFY_RIGHT | JUSTIFY_CENTER_Y | ICON_AFTER_TEXT | LAYOUT_SIDE_LEFT | LAYOUT_RIGHT
), 0, 0, 0, 0, 0, 0, 0, 0);
	pLabel->setFont((FXFont*)pFont->GetNativeFont());
	pLabel->setTarget(this);
	pLabel->setSelector(ID_LABEL);
	
	UpdateText();
	UpdateDescription();
	UpdateEnabled();
	UpdateColor();
	UpdateLinkedState();
}

igdeNativeFoxNVSlot::~igdeNativeFoxNVSlot(){
}

igdeNativeFoxNVSlot *igdeNativeFoxNVSlot::CreateNativeWidget(igdeNVSlot &powner){
	if(! powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(! pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxNVSlot(powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlagsAll(&powner), *powner.GetGuiTheme());
}

void igdeNativeFoxNVSlot::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxNVSlot::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

FXbool igdeNativeFoxNVSlot::canFocus() const{
	return true;
}



void igdeNativeFoxNVSlot::UpdateText(){
	pLabel->setText(pOwner->GetText().GetString());
}

void igdeNativeFoxNVSlot::UpdateDescription(){
	pLabel->setTipText(pOwner->GetDescription().GetString());
	pLabel->setHelpText(pOwner->GetDescription().GetString());
}

void igdeNativeFoxNVSlot::UpdateEnabled(){
	if(pOwner->GetEnabled()){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxNVSlot::UpdateColor(){
	const FXColor color = igdeUIFoxHelper::ConvertColor(pOwner->GetColor());
	pLabel->setTextColor(color);
	pSocket->SetColorFillChecked(color);
	
// 	const FXColor bgcolor = igdeUIFoxHelper::ConvertColor( decColor( 150.0f / 255.0f, 150.0f / 255.0f, 150.0f / 255.0f ) );
// 	pSocket->SetColorFillChecked( bgcolor );
}

void igdeNativeFoxNVSlot::UpdateLinkedState(){
	pSocket->SetChecked(pOwner->GetLinkCount() > 0);
}



decPoint igdeNativeFoxNVSlot::GetCenter() const{
	decPoint center(pSocket->GetCenter());
	pSocket->translateCoordinatesTo(center.x, center.y, this, center.x, center.y);
	return center;
}

decPoint igdeNativeFoxNVSlot::GetCenterNode() const{
	decPoint center(GetCenter());
	
	if(! pOwner->GetOwnerNode() || ! pOwner->GetOwnerNode()->GetNativeWidget()){
		return center;
	}
	
	translateCoordinatesTo(center.x, center.y,
		(FXWindow*)pOwner->GetOwnerNode()->GetNativeWidget(),
		center.x, center.y);
	
	return center;
}

decPoint igdeNativeFoxNVSlot::GetCenterBoard() const{
	decPoint center(GetCenter());
	
	if(! pOwner->GetOwnerNode() || ! pOwner->GetOwnerNode()->GetOwnerBoard()
	|| ! pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeContainer()){
		return center;
	}
	
	translateCoordinatesTo(center.x, center.y,
		(FXWindow*)pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeContainer(),
		center.x, center.y);
	
	return center;
}

decPoint igdeNativeFoxNVSlot::GetConnector() const{
	decPoint position(pOwner->GetIsInput() ? pSocket->GetBorderLeft() : pSocket->GetBorderRight());
	pSocket->translateCoordinatesTo(position.x, position.y, this, position.x, position.y);
	return position;
}

decPoint igdeNativeFoxNVSlot::GetConnectorNode() const{
	decPoint position(GetConnector());
	
	if(! pOwner->GetOwnerNode() || ! pOwner->GetOwnerNode()->GetNativeWidget()){
		return position;
	}
	
	translateCoordinatesTo(position.x, position.y,
		(FXWindow*)pOwner->GetOwnerNode()->GetNativeWidget(),
		position.x, position.y);
	
	return position;
}

decPoint igdeNativeFoxNVSlot::GetConnectorBoard() const{
	decPoint position(GetConnector());
	
	if(! pOwner->GetOwnerNode() || ! pOwner->GetOwnerNode()->GetOwnerBoard()
	|| ! pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeContainer()){
		return position;
	}
	
	translateCoordinatesTo(position.x, position.y,
		(FXWindow*)pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeContainer(),
		position.x, position.y);
	
	return position;
}



int igdeNativeFoxNVSlot::NVSlotFlags(const igdeNVSlot &){
	return 0;
}

igdeFont *igdeNativeFoxNVSlot::NVSlotFont(const igdeNVSlot &powner, const igdeGuiTheme &guitheme){
	igdeFont::sConfiguration configuration;
	powner.GetEnvironment().GetApplicationFont(configuration);
	
	if(guitheme.HasProperty(igdeGuiThemePropertyNames::nodeViewSlotFontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::nodeViewSlotFontSizeAbsolute, 0);
		
	}else if(guitheme.HasProperty(igdeGuiThemePropertyNames::nodeViewSlotFontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::nodeViewSlotFontSize, 1.0f);
		
	}else if(guitheme.HasProperty(igdeGuiThemePropertyNames::fontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0);
		
	}else if(guitheme.HasProperty(igdeGuiThemePropertyNames::fontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f);
	}
	
	return powner.GetEnvironment().GetSharedFont(configuration);
}

int igdeNativeFoxNVSlot::NVSlotPadLeft(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::nodeViewSlotPaddingLeft, DEFAULT_PAD);
}

int igdeNativeFoxNVSlot::NVSlotPadRight(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::nodeViewSlotPaddingRight, DEFAULT_PAD);
}

int igdeNativeFoxNVSlot::NVSlotPadTop(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::nodeViewSlotPaddingTop, DEFAULT_PAD);
}

int igdeNativeFoxNVSlot::NVSlotPadBottom(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::nodeViewSlotPaddingBottom, DEFAULT_PAD);
}

long igdeNativeFoxNVSlot::onChildLayoutFlags(FXObject*, FXSelector, void *pdata){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *((igdeUIFoxHelper::sChildLayoutFlags*)pdata);
	clflags.flags = pOwner->GetIsInput()
		? LAYOUT_SIDE_RIGHT | LAYOUT_LEFT | LAYOUT_FILL_X | LAYOUT_FILL_Y
		: LAYOUT_SIDE_LEFT | LAYOUT_RIGHT | LAYOUT_FILL_X | LAYOUT_FILL_Y;
	clflags.canResizeHorizontal = false;
	clflags.canResizeVertical = false;
	return 1;
}



// Events
///////////

long igdeNativeFoxNVSlot::onPaint(FXObject *sender, FXSelector selector, void *pdata){
	setBackColor(getParent()->getBackColor());
	pLabel->setBackColor(getBackColor());
	return FXHorizontalFrame::onPaint(sender, selector, pdata);
}

long igdeNativeFoxNVSlot::onSocketCommand(FXObject*, FXSelector, void*){
	UpdateLinkedState();  // restore state in case somebody manages to change it
	return 1;
}

long igdeNativeFoxNVSlot::onSocketLeftMouseDown(FXObject*, FXSelector, void*){
	if(pIsDnd){
		return 1;
	}
	
	if(! pOwner->GetOwnerNode() || ! pOwner->GetOwnerNode()->GetOwnerBoard()
	|| ! pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeWidget()){
		return 1;
	}
	
	pSocket->grab();
	pIsDnd = pSocket->beginDrag(&pDragTypeSocket, 1);
	if(! pIsDnd){
		pSocket->ungrab();
		return 1;
	}
	
	igdeNativeFoxNVBoard &nativeBoard = *((igdeNativeFoxNVBoard*)
		pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeWidget());
	nativeBoard.BeginCreateLink(this);
	
	return 1;
}

long igdeNativeFoxNVSlot::onSocketMouseMoved(FXObject*, FXSelector, void *pdata){
	if(! pIsDnd){
		return 1;
	}
	
	if(! pOwner->GetOwnerNode() || ! pOwner->GetOwnerNode()->GetOwnerBoard()
	|| ! pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeWidget()){
		return 1;
	}
	
	igdeNativeFoxNVBoard &nativeBoard = *((igdeNativeFoxNVBoard*)
		pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeWidget());
	const FXEvent &event = *((FXEvent*)pdata);
	
	decPoint position;
	nativeBoard.translateCoordinatesFrom(position.x, position.y, pSocket, event.win_x, event.win_y);
	nativeBoard.SetCreateLinkPosition(position);
	
	pSocket->handleDrag(event.root_x, event.root_y, DRAG_LINK);
	
	/*
	switch(pSocket->didAccept()){
	case DRAG_LINK:
		pSocket->setDragCursor(getApp()->getDefaultCursor(DEF_DNDLINK_CURSOR));
		break;
		
	default:
		pSocket->setDragCursor(getApp()->getDefaultCursor(DEF_DNDSTOP_CURSOR));
		break;
	}
	*/
	
	return 1;
}

long igdeNativeFoxNVSlot::onSocketLeftMouseUp(FXObject*, FXSelector, void*){
	if(! pIsDnd){
		return 1;
	}
	
	pSocket->endDrag(didAccept() != DRAG_REJECT);
// 	pSocket->setDragCursor( pSocket->getDefaultCursor() );
	pSocket->ungrab();
	pIsDnd = false;
	
	igdeNativeFoxNVBoard &nativeBoard = *((igdeNativeFoxNVBoard*)
		pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeWidget());
	nativeBoard.FinishCreateLink();  // calling this twice is safe
	
	return 1;
}

long igdeNativeFoxNVSlot::onSocketDndEnter(FXObject*, FXSelector, void*){
	return 1;
}

long igdeNativeFoxNVSlot::onSocketDndLeave(FXObject*, FXSelector, void*){
	if(pIsDnd){
		return 1;
	}
	
	if(! pOwner->GetOwnerNode() || ! pOwner->GetOwnerNode()->GetOwnerBoard()
	|| ! pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeWidget()){
		return 1;
	}
	
	igdeNativeFoxNVBoard &nativeBoard = *((igdeNativeFoxNVBoard*)
		pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeWidget());
	nativeBoard.SetCreateLinkTarget(NULL);
	pSocket->acceptDrop(DRAG_REJECT);
	return 1;
}

long igdeNativeFoxNVSlot::onSocketDndDrop(FXObject*, FXSelector, void*){
	if(pIsDnd){
		return 1;
	}
	
	if(! pOwner->GetOwnerNode() || ! pOwner->GetOwnerNode()->GetOwnerBoard()
	|| ! pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeWidget()){
		return 1;
	}
	
	igdeNativeFoxNVBoard &nativeBoard = *((igdeNativeFoxNVBoard*)
		pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeWidget());
	nativeBoard.FinishCreateLink();
	return 1;
}

long igdeNativeFoxNVSlot::onSocketDndMotion(FXObject*, FXSelector, void *pdata){
	if(pIsDnd){
		return 1;
	}
	
	if(! pOwner->GetOwnerNode() || ! pOwner->GetOwnerNode()->GetOwnerBoard()
	|| ! pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeWidget()){
		pSocket->acceptDrop(DRAG_REJECT);
		return 1;
	}
	
	igdeNativeFoxNVBoard &nativeBoard = *((igdeNativeFoxNVBoard*)
		pOwner->GetOwnerNode()->GetOwnerBoard()->GetNativeWidget());
	const FXEvent &event = *((FXEvent*)pdata);
	
	if(offeredDNDType(FROM_DRAGNDROP, pDragTypeSocket)
	&& pSocket->IsInsideSocket(decPoint(event.win_x, event.win_y))){
		nativeBoard.SetCreateLinkTarget(this);
		
		if(nativeBoard.GetOwner().CanLink(&nativeBoard.GetCreateLinkSource()->GetOwner(), pOwner)){
			pSocket->acceptDrop(DRAG_LINK);
			
		}else{
			pSocket->acceptDrop(DRAG_REJECT);
		}
		
	}else{
		nativeBoard.SetCreateLinkTarget(NULL);
		pSocket->acceptDrop(DRAG_REJECT);
	}
	
	return 1;
}



/*

long meWVNodeSlot::onLeftMouseDown(FXObject*, FXSelector, void*){
	meWindowVegetation *windowVegetation = pParentNode->GetWindowVegetation();
	FXEvent *event = (FXEvent*)data;
	FXint vlayerRelX, vlayerRelY;
	int x = event->win_x;
	int y = event->win_y;
	
	if(! pIsInput){
		translateCoordinatesTo(vlayerRelX, vlayerRelY, windowVegetation, x, y);
		windowVegetation->AddLinkStartingWith(vlayerRelX, vlayerRelY, this);
	}
	
	return 1;
}

long meWVNodeSlot::onMouseMove(FXObject*, FXSelector, void*){
//	FXEvent *event = ( FXEvent* )data;
//	bool shift = ( event->state & SHIFTMASK ) == SHIFTMASK;
//	bool control = ( event->state & CONTROLMASK ) == CONTROLMASK;
//	int x = event->win_x;
//	int y = event->win_y;
	
	return 1;
}

long meWVNodeSlot::onLeftMouseUp(FXObject*, FXSelector, void*){
	return 1;
}

long meWVNodeSlot::onRightMouseDown(FXObject*, FXSelector, void*){
//	FXEvent *event = ( FXEvent* )data;
//	bool shift = ( event->state & SHIFTMASK ) == SHIFTMASK;
//	bool control = ( event->state & CONTROLMASK ) == CONTROLMASK;
//	int x = event->win_x;
//	int y = event->win_y;
	
	return 1;
}

long meWVNodeSlot::onRightMouseUp(FXObject*, FXSelector, void*){
	return 1;
}

*/

#endif
