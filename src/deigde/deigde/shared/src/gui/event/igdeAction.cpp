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

#include "igdeAction.h"
#include "igdeActionListener.h"

#include <dragengine/common/exceptions.h>



// Class igdeAction
/////////////////////

// Constructor, destructor
////////////////////////////

igdeAction::igdeAction() :
pMnemonic(deInputEvent::ekcUndefined),
pEnabled(true),
pSelected(false),
pDefault(false){
}

igdeAction::igdeAction(const char *text, const char *description) :
pText(text),
pDescription(description),
pMnemonic(deInputEvent::ekcUndefined),
pEnabled(true),
pSelected(false),
pDefault(false){
}

igdeAction::igdeAction(const char *text, const char *description, const igdeHotKey &hotKey) :
pText(text),
pDescription(description),
pHotKey(hotKey),
pMnemonic(deInputEvent::ekcUndefined),
pEnabled(true),
pSelected(false),
pDefault(false){
}

igdeAction::igdeAction(const char *text, const char *description, deInputEvent::eKeyCodes mnemonic) :
pText(text),
pDescription(description),
pMnemonic(mnemonic),
pEnabled(true),
pSelected(false),
pDefault(false){
}

igdeAction::igdeAction(const char *text, const char *description,
	deInputEvent::eKeyCodes mnemonic, const igdeHotKey &hotKey) :
pText(text),
pDescription(description),
pHotKey(hotKey),
pMnemonic(mnemonic),
pEnabled(true),
pSelected(false),
pDefault(false){
}

igdeAction::igdeAction(const char *text, igdeIcon *icon, const char *description) :
pText(text),
pDescription(description),
pMnemonic(deInputEvent::ekcUndefined),
pIcon(icon),
pEnabled(true),
pSelected(false),
pDefault(false){
}

igdeAction::igdeAction(const char *text, igdeIcon *icon, const char *description,
	const igdeHotKey &hotKey) :
pText(text),
pDescription(description),
pHotKey(hotKey),
pMnemonic(deInputEvent::ekcUndefined),
pIcon(icon),
pEnabled(true),
pSelected(false),
pDefault(false){
}

igdeAction::igdeAction(const char *text, igdeIcon *icon, const char *description,
	deInputEvent::eKeyCodes mnemonic) :
pText(text),
pDescription(description),
pMnemonic(mnemonic),
pIcon(icon),
pEnabled(true),
pSelected(false),
pDefault(false){
}

igdeAction::igdeAction(const char *text, igdeIcon *icon, const char *description,
	deInputEvent::eKeyCodes mnemonic, const igdeHotKey &hotKey) :
pText(text),
pDescription(description),
pHotKey(hotKey),
pMnemonic(mnemonic),
pIcon(icon),
pEnabled(true),
pSelected(false),
pDefault(false){
}

igdeAction::~igdeAction(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeActionListener *l){
		l->OnDestroyed(this);
	});
}



// Management
///////////////

void igdeAction::SetText(const char *text){
	if(pText == text){
		return;
	}
	
	pText = text;
	NotifyParametersChanged();
}

void igdeAction::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	NotifyParametersChanged();
}

void igdeAction::SetHotKey(const igdeHotKey &hotKey){
	if(pHotKey == hotKey){
		return;
	}
	
	pHotKey = hotKey;
	NotifyParametersChanged();
}

void igdeAction::SetMnemonic(deInputEvent::eKeyCodes mnemonic){
	if(pMnemonic == mnemonic){
		return;
	}
	
	pMnemonic = mnemonic;
	NotifyParametersChanged();
}

void igdeAction::SetIcon(igdeIcon *icon){
	if(pIcon == icon){
		return;
	}
	
	pIcon = icon;
	NotifyParametersChanged();
}

void igdeAction::SetEnabled(bool enabled){
	if(pEnabled == enabled){
		return;
	}
	
	pEnabled = enabled;
	NotifyParametersChanged();
}

void igdeAction::SetSelected(bool selected){
	if(pSelected == selected){
		return;
	}
	
	pSelected = selected;
	NotifyParametersChanged();
}

void igdeAction::SetDefault(bool isdefault){
	if(pDefault == isdefault){
		return;
	}
	
	pDefault = isdefault;
	NotifyParametersChanged();
}



void igdeAction::AddListener(igdeActionListener *listener){
	DEASSERT_NOTNULL(listener)
	pListeners.Add(listener);
}

void igdeAction::RemoveListener(igdeActionListener *listener){
	pListeners.Remove(listener);
}

void igdeAction::NotifyParametersChanged(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeActionListener *l){
		l->OnParameterChanged(this);
	});
}



void igdeAction::Update(){
}
