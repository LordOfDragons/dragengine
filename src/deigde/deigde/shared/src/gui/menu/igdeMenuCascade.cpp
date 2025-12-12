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
#include <string.h>

#include "igdeMenuCascade.h"
#include "../igdeCommonDialogs.h"
#include "../igdeWindow.h"
#include "../igdeWidget.h"
#include "../native/toolkit.h"
#include "../resources/igdeIcon.h"

#include "../../engine/igdeEngineController.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gameproject/igdeGameProject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeMenuCascade
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeMenuCascade::igdeMenuCascade(igdeEnvironment &environment) :
igdeContainer(environment),
pMnemonic(deInputEvent::ekcUndefined),
pEnabled(true){
}

igdeMenuCascade::igdeMenuCascade(igdeEnvironment &environment, const char *text) :
igdeContainer(environment),
pText(text),
pMnemonic(deInputEvent::ekcUndefined),
pEnabled(true){
}

igdeMenuCascade::igdeMenuCascade(igdeEnvironment &environment, const char *text,
	igdeIcon *icon) :
igdeContainer(environment),
pText(text),
pMnemonic(deInputEvent::ekcUndefined),
pIcon(icon),
pEnabled(true){
}

igdeMenuCascade::igdeMenuCascade(igdeEnvironment &environment, const char *text,
	deInputEvent::eKeyCodes mnemonic) :
igdeContainer(environment),
pText(text),
pMnemonic(mnemonic),
pEnabled(true){
}

igdeMenuCascade::igdeMenuCascade(igdeEnvironment &environment, const char *text,
	igdeIcon *icon, const char *description) :
igdeContainer(environment),
pText(text),
pDescription(description),
pMnemonic(deInputEvent::ekcUndefined),
pIcon(icon),
pEnabled(true){
}

igdeMenuCascade::igdeMenuCascade(igdeEnvironment &environment, const char *text,
	igdeIcon *icon, const char *description, const igdeHotKey &hotKey) :
igdeContainer(environment),
pText(text),
pDescription(description),
pHotKey(hotKey),
pMnemonic(deInputEvent::ekcUndefined),
pIcon(icon),
pEnabled(true){
}

igdeMenuCascade::igdeMenuCascade(igdeEnvironment &environment, const char *text,
	igdeIcon *icon, const char *description, deInputEvent::eKeyCodes mnemonic) :
igdeContainer(environment),
pText(text),
pDescription(description),
pMnemonic(mnemonic),
pIcon(icon),
pEnabled(true){
}

igdeMenuCascade::igdeMenuCascade(igdeEnvironment &environment, const char *text,
	igdeIcon *icon, const char *description, deInputEvent::eKeyCodes mnemonic,
	const igdeHotKey &hotKey) :
igdeContainer(environment),
pText(text),
pDescription(description),
pHotKey(hotKey),
pMnemonic(mnemonic),
pIcon(icon),
pEnabled(true){
}

igdeMenuCascade::~igdeMenuCascade(){
	igdeMenuCascade::DestroyNativeWidget();
}



// Management
///////////////

void igdeMenuCascade::SetText(const char *text){
	if(pText == text){
		return;
	}
	
	pText = text;
	OnTextChanged();
}

void igdeMenuCascade::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeMenuCascade::SetHotKey(const igdeHotKey &hotKey){
	if(pHotKey == hotKey){
		return;
	}
	
	pHotKey = hotKey;
	OnHotKeyChanged();
}

void igdeMenuCascade::SetMnemonic(deInputEvent::eKeyCodes mnemonic){
	if(pMnemonic == mnemonic){
		return;
	}
	
	pMnemonic = mnemonic;
	OnMnemonicChanged();
}

void igdeMenuCascade::SetIcon(igdeIcon *icon){
	if(pIcon == icon){
		return;
	}
	
	pIcon = icon;
	OnIconChanged();
}

void igdeMenuCascade::SetEnabled(bool enabled){
	if(pEnabled == enabled){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}



void igdeMenuCascade::Popup(igdeWidget &owner){
	if(owner.GetNativeWidget()){
		PopupAt(owner, igdeNativeWidget::GetCursorPosition(owner));
	}
}

void igdeMenuCascade::Popup(igdeWidget &owner, const decPoint &position){
	PopupAt(owner, owner.WidgetToScreen(position));
}

void igdeMenuCascade::PopupBottom(igdeWidget &owner){
	if(!owner.GetNativeWidget()){
		return;
	}
	
	PopupAt(owner, owner.WidgetToScreen(decPoint(0, igdeNativeWidget::GetSize(owner).y)));
}



void igdeMenuCascade::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	void * const native = igdeNativeMenuCascade::CreateNativeWidget(*this);
	SetNativeWidget(native);
	igdeNativeMenuCascade::PostCreateNativeWidget(*this, native);
	
	CreateChildWidgetNativeWidgets();
}

void igdeMenuCascade::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	igdeNativeMenuCascade::DestroyNativeWidget(*this, GetNativeWidget());
	DropNativeWidget();
}

void *igdeMenuCascade::GetNativeContainer() const{
	if(!GetNativeWidget()){
		return nullptr;
	}
	return igdeNativeMenuCascade::GetNativeContainer(*this, GetNativeWidget());
}



void igdeMenuCascade::OnTextChanged(){
	if(GetNativeWidget()){
		igdeNativeMenuCascade::UpdateTitle(*this, GetNativeWidget());
	}
}

void igdeMenuCascade::OnDescriptionChanged(){
	if(GetNativeWidget()){
		igdeNativeMenuCascade::UpdateDescription(*this, GetNativeWidget());
	}
}

void igdeMenuCascade::OnHotKeyChanged(){
	if(!GetNativeWidget()){
		return;
	}
}

void igdeMenuCascade::OnMnemonicChanged(){
	if(!GetNativeWidget()){
		return;
	}
}

void igdeMenuCascade::OnIconChanged(){
	if(GetNativeWidget()){
		igdeNativeMenuCascade::UpdateIcon(*this, GetNativeWidget());
	}
}

void igdeMenuCascade::OnEnabledChanged(){
	if(GetNativeWidget()){
		igdeNativeMenuCascade::UpdateEnabled(*this, GetNativeWidget());
	}
}

void igdeMenuCascade::PopupAt(igdeWidget &owner, const decPoint &position){
	// this is ugly, I know. the problem is that we can not create a popup widget as a window
	// out in the blue. we sort of use an "injection hack" to get this working. we create for
	// the menu a native widget of just FXMenuPane type and assign it to the menu as if
	// CreateNativeWidget has been called on the menu. then we assign the native widgets
	// also to this class knowning very well we are not allowed to delete them. we can not
	// simply create igdeNativeMenuCascadeCascade and use the getMenu() of it since
	// toolkits then fail
	if(GetNativeWidget()){
		DETHROW(deeInvalidParam);
	}
	
	void * const nativePopup = igdeNativeMenuCascade::CreateNativePopup(*this, owner);
	try{
		SetNativeWidget(nativePopup);
		CreateChildWidgetNativeWidgets();
		igdeNativeMenuCascade::PostCreateNativePopup(*this, nativePopup);
		
		igdeNativeMenuCascade::ShowPopupWindow(*this, owner, position);
		
		DropNativeWidget();
		igdeNativeMenuCascade::DestroyNativePopup(*this, nativePopup);
		
	}catch(const deException &){
		DropNativeWidget();
		igdeNativeMenuCascade::DestroyNativePopup(*this, nativePopup);
		throw;
	}
}
