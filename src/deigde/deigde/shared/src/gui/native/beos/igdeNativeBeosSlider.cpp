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

#include "igdeNativeBeosSlider.h"
#include "../../igdeSlider.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/exceptions.h>


igdeNativeBeosSlider::igdeNativeBeosSlider(igdeSlider &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BSlider(BRect(0, 0, 200, 20), "slider", "", nullptr, (int32)owner.GetLower(),
	(int32)owner.GetUpper(), B_BLOCK_THUMB),
pOwner(&owner),
pFont(nullptr),
pIgnoreValueChange(false)
{
	SetValue((int32)owner.GetValue());
	SetTickSpacing((int32)owner.GetTickSpacing());
	
	if(owner.GetOrientation() == igdeSlider::eoVertical){
		SetOrientation(B_VERTICAL);
		
	}else{
		SetOrientation(B_HORIZONTAL);
	}
	
	// Apply theme font to slider
	int fontSize = 12; // Default
	try{
		const auto &fontProp = guitheme.GetProperty(igdeGuiThemePropertyNames::buttonFontSize);
		if(!fontProp.IsEmpty()){
			fontSize = (int)fontProp.ToFloat();
		}
		
	}catch(...){
		// Use default if property not available
	}
	
	BFont font(be_plain_font);
	font.SetSize(fontSize);
	SetFont(&font);
	
	if(!owner.GetEnabled()){
		SetEnabled(false);
	}
}

igdeNativeBeosSlider::~igdeNativeBeosSlider() = default;


igdeNativeBeosSlider* igdeNativeBeosSlider::CreateNativeWidget(igdeSlider &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosSlider(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosSlider::PostCreateNativeWidget(){
}

void igdeNativeBeosSlider::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}

void igdeNativeBeosSlider::Focus(){
	MakeFocus(true);
}

void igdeNativeBeosSlider::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosSlider::UpdateScale(){
	SetTickSpacing((int32)pOwner->GetTickSpacing());
}

void igdeNativeBeosSlider::UpdateRange(){
	SetLimits((int32)pOwner->GetLower(), (int32)pOwner->GetUpper());
}

void igdeNativeBeosSlider::UpdateValue(){
	if(!pIgnoreValueChange){
		pIgnoreValueChange = true;
		SetValue((int32)pOwner->GetValue());
		pIgnoreValueChange = false;
	}
}

void igdeNativeBeosSlider::UpdateEnabled(){
	SetEnabled(pOwner->GetEnabled());
}

void igdeNativeBeosSlider::UpdateDescription(){
	// Tooltip handling - deferred
}

void igdeNativeBeosSlider::MessageReceived(BMessage *msg){
	BSlider::MessageReceived(msg);
	
	try{
		if(msg->what == B_VALUE_CHANGED && !pIgnoreValueChange){
			pIgnoreValueChange = true;
			pOwner->SetValue((float)Value());
			pIgnoreValueChange = false;
			
			// Notify listeners
			pOwner->NotifyValueChanged();
		}
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE Slider", e);
	}
}

bool igdeNativeBeosSlider::IsFocusable() const{
	return pOwner->GetEnabled();
}

#endif
