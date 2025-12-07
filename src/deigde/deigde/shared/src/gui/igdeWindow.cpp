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

#include "igdeApplication.h"
#include "igdeWindow.h"
#include "igdeCommonDialogs.h"
#include "igdeContainer.h"
#include "igdeWidget.h"
#include "native/toolkit.h"
#include "resources/igdeIcon.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeWindow
/////////////////////

// Constructor, destructor
////////////////////////////

igdeWindow::igdeWindow(igdeEnvironment &environment, const char *title, igdeIcon *icon, bool canResize) :
igdeContainer(environment),
pTitle(title),
pIcon(icon),
pCanResize(canResize),
pPosition(igdeApplication::app().DisplayScaled(decPoint(10, 50))),
pSize(igdeApplication::app().DisplayScaled(decPoint(400, 300))),
pEnabled(true)
{
	SetVisible(false);
}

igdeWindow::~igdeWindow(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeWindow::SetTitle(const char *title){
	if(pTitle == title){
		return;
	}
	
	pTitle = title;
	OnTitleChanged();
}

void igdeWindow::SetIcon(igdeIcon* icon){
	if(pIcon == icon){
		return;
	}
	
	pIcon = icon;
	OnIconChanged();
}

void igdeWindow::SetSize(const decPoint &size){
	if(size == pSize){
		return;
	}
	
	pSize = size;
	OnSizeChanged();
}

void igdeWindow::SetEnabled(bool enabled){
	if(enabled == pEnabled){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeWindow::SetPosition(const decPoint &position){
	if(position == pPosition){
		return;
	}
	
	pPosition = position;
	OnPositionChanged();
}

void igdeWindow::RaiseAndActivate(){
	OnRaiseAndActivate();
}



void igdeWindow::AddChild(igdeWidget *child){
	if(GetChildCount() > 0){
		DETHROW(deeInvalidParam);
	}
	igdeContainer::AddChild(child);
}



bool igdeWindow::CloseWindow(){
	return true;
}

void igdeWindow::Close(){
	if(GetNativeWidget()){
		DestroyNativeWidget();
	}
}

igdeWindow *igdeWindow::GetParentWindow(){
	return this;
}



// IGDE Internal Use Only
///////////////////////////

void igdeWindow::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	if(GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	igdeNativeWindow * const native = igdeNativeWindow::CreateNativeWidget(*this);
	SetNativeWidget(native);
	CreateChildWidgetNativeWidgets();
	native->PostCreateNativeWidget();
}

void igdeWindow::DestroyNativeWidget(){
	if(! GetNativeWidget()){
		return;
	}
	
	((igdeNativeWindow*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeWindow::OnTitleChanged(){
	if(GetNativeWidget()){
		((igdeNativeWindow*)GetNativeWidget())->UpdateTitle();
	}
}

void igdeWindow::OnIconChanged(){
	if(GetNativeWidget()){
		((igdeNativeWindow*)GetNativeWidget())->UpdateIcon();
	}
}

void igdeWindow::OnSizeChanged(){
	if(GetNativeWidget()){
		((igdeNativeWindow*)GetNativeWidget())->UpdateSize();
	}
}

void igdeWindow::OnPositionChanged(){
	if(GetNativeWidget()){
		((igdeNativeWindow*)GetNativeWidget())->UpdatePosition();
	}
}

void igdeWindow::OnVisibleChanged(){
	if(GetVisible()){
		CreateNativeWidget();
	}
	igdeWidget::OnVisibleChanged();
}

void igdeWindow::OnEnabledChanged(){
	if(GetNativeWidget()){
		((igdeNativeWindow*)GetNativeWidget())->UpdateEnabled();
	}
}

void igdeWindow::OnRaiseAndActivate(){
	if(GetNativeWidget()){
		((igdeNativeWindow*)GetNativeWidget())->RaiseAndActivate();
	}
}
