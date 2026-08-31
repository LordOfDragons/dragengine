/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeosMenuCascade.h"
#include "../../../menu/igdeMenuCascade.h"
#include "../../../igdeWidget.h"
#include <interface/Menu.h>
#include <interface/MenuItem.h>
#include <interface/PopUpMenu.h>
#include <interface/Window.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>


// Helper struct to hold both menu and menu item
struct sBeosMenuCascadeData{
	BMenu *menu;
	BMenuItem *menuItem;
	
	sBeosMenuCascadeData() : menu(nullptr), menuItem(nullptr){}
	~sBeosMenuCascadeData(){
		// menu is owned by menuItem, menuItem is owned by parent menu
	}
};


// Class igdeNativeBeosMenuCascade
////////////////////////////////////

void* igdeNativeBeosMenuCascade::CreateNativeWidget(igdeMenuCascade &owner){
	sBeosMenuCascadeData *data = new sBeosMenuCascadeData;
	
	data->menu = new BMenu(owner.GetTitle().GetString());
	data->menuItem = new BMenuItem(data->menu);
	
	if(owner.GetParent()){
		BMenu * const parentMenu = (BMenu*)owner.GetParent()->GetNativeContainer();
		if(parentMenu){
			parentMenu->AddItem(data->menuItem);
		}
	}
	
	return data;
}

void igdeNativeBeosMenuCascade::PostCreateNativeWidget(igdeMenuCascade &, void *){
}

void igdeNativeBeosMenuCascade::DestroyNativeWidget(igdeMenuCascade &, void *native){
	sBeosMenuCascadeData * const data = (sBeosMenuCascadeData*)native;
	if(data){
		if(data->menuItem && data->menuItem->Menu()){
			data->menuItem->Menu()->RemoveItem(data->menuItem);
			delete data->menuItem;
		}
		delete data;
	}
}

void* igdeNativeBeosMenuCascade::GetNativeContainer(igdeMenuCascade &, void *native){
	sBeosMenuCascadeData * const data = (sBeosMenuCascadeData*)native;
	return data ? data->menu : nullptr;
}

void igdeNativeBeosMenuCascade::UpdateTitle(igdeMenuCascade &owner, void *native){
	sBeosMenuCascadeData * const data = (sBeosMenuCascadeData*)native;
	if(data && data->menuItem){
		data->menuItem->SetLabel(owner.GetTitle().GetString());
	}
}

void igdeNativeBeosMenuCascade::UpdateDescription(igdeMenuCascade &, void *){
}

void igdeNativeBeosMenuCascade::UpdateHotKey(igdeMenuCascade &, void *){
}

void igdeNativeBeosMenuCascade::UpdateIcon(igdeMenuCascade &, void *){
}

void igdeNativeBeosMenuCascade::UpdateEnabled(igdeMenuCascade &owner, void *native){
	sBeosMenuCascadeData * const data = (sBeosMenuCascadeData*)native;
	if(data && data->menuItem){
		data->menuItem->SetEnabled(owner.GetEnabled());
	}
}

void* igdeNativeBeosMenuCascade::CreateNativePopup(igdeMenuCascade &owner, igdeWidget &){
	BPopUpMenu * const popup = new BPopUpMenu(owner.GetTitle().GetString(), false, false);
	return popup;
}

void igdeNativeBeosMenuCascade::PostCreateNativePopup(igdeMenuCascade &, void *){
}

void igdeNativeBeosMenuCascade::ShowPopupWindow(igdeMenuCascade &owner, igdeWidget &widgetOwner,
const decPoint &position){
	BView * const view = (BView*)widgetOwner.GetNativeWidget();
	if(!view || !view->Window()){
		return;
	}
	
	BPoint screenPos((float)position.x, (float)position.y);
	view->ConvertToScreen(&screenPos);
	
	BPopUpMenu * const popup = new BPopUpMenu(owner.GetTitle().GetString(), false, false);
	
	const int count = owner.GetChildCount();
	for(int i = 0; i < count; i++){
		// Add items from owner's children
	}
	
	popup->Go(screenPos, true, false);
	delete popup;
}

void igdeNativeBeosMenuCascade::DestroyNativePopup(igdeMenuCascade &, void *native){
	delete (BPopUpMenu*)native;
}

#endif
