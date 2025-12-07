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

#include "igdeNativeFoxSlider.h"
#include "foxIcons.h"
#include "../../igdeSlider.h"
#include "../../igdeContainer.h"
#include "../../igdeCommonDialogs.h"
#include "../../event/igdeAction.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../resources/igdeIcon.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Events
///////////

FXDEFMAP(igdeNativeFoxSlider) igdeNativeFoxSliderMap[] = {
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxSlider::ID_SELF, igdeNativeFoxSlider::onCommand),
	FXMAPFUNC(SEL_CHANGED, igdeNativeFoxSlider::ID_SELF, igdeNativeFoxSlider::onChanged)
};



// Class igdeNativeFoxSlider
//////////////////////////////

FXIMPLEMENT(igdeNativeFoxSlider, FXSlider, igdeNativeFoxSliderMap, ARRAYNUMBER(igdeNativeFoxSliderMap))

// Constructor, destructor
////////////////////////////

igdeNativeFoxSlider::igdeNativeFoxSlider(){}

igdeNativeFoxSlider::igdeNativeFoxSlider(igdeSlider &powner, FXComposite *pparent, int layoutFlags) :
FXSlider(pparent, this, ID_SELF, layoutFlags | SliderFlags(powner)),
pOwner(&powner)
{
	UpdateScale();
	UpdateRange();
	UpdateValue();
	if(! powner.GetEnabled()){
		disable();
	}
	
	setTipText(powner.GetDescription().GetString());
	setHelpText(powner.GetDescription().GetString());
}

igdeNativeFoxSlider::~igdeNativeFoxSlider(){
}

igdeNativeFoxSlider *igdeNativeFoxSlider::CreateNativeWidget(igdeSlider &powner){
	if(! powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(! pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxSlider(powner, pparent, igdeUIFoxHelper::GetChildLayoutFlags(&powner));
}

void igdeNativeFoxSlider::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxSlider::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxSlider::Focus(){
	setFocus();
}

void igdeNativeFoxSlider::UpdateScale(){
	pScale = powf(10.0f, (float)pOwner->GetPrecision());
	pInvScale = 1.0f / pScale;
}

void igdeNativeFoxSlider::UpdateRange(){
	setRange((FXint)(pOwner->GetLower() * pScale + 0.5f),
		(FXint)(pOwner->GetUpper() * pScale + 0.5f));
	setTickDelta((FXint)(pOwner->GetTickSpacing() * pScale + 0.5f));
}

void igdeNativeFoxSlider::UpdateValue(){
	setValue((FXint)(pOwner->GetValue() * pScale + 0.5f));
}

void igdeNativeFoxSlider::UpdateEnabled(){
	if(pOwner->GetEnabled()){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxSlider::UpdateDescription(){
	const char * const description = pOwner->GetDescription();
	setTipText(description);
	setHelpText(description);
}



int igdeNativeFoxSlider::SliderFlags(const igdeSlider &powner){
	switch(powner.GetOrientation()){
	case igdeSlider::eoHorizontal:
		return SLIDER_HORIZONTAL | SLIDER_ARROW_DOWN | SLIDER_TICKS_BOTTOM;
		
	case igdeSlider::eoVertical:
		return SLIDER_VERTICAL | SLIDER_ARROW_RIGHT | SLIDER_TICKS_RIGHT;
		
	default:
		return 0;
	}
}



// Events
///////////

long igdeNativeFoxSlider::onCommand(FXObject*, FXSelector, void*){
	if(! pOwner->GetEnabled()){
		return 0;
	}
	
	// we have to send this notification always even if nothing changed. this is because
	// a SEL_CHANGED can be send right before the SEL_COMMAND and then the value would
	// be already set and the SEL_COMMAND would be ignore. ignoring the SEL_COMMAND though
	// breaks listeners only listening for the "changed" event.
	// 
	// we can re-enable this code if we start tracking ourself if the value changed by a
	// previous SEL_CHANGED. but this would need first a true gain to be obvious
	const float value = (float)getValue() * pInvScale;
	/*if( fabsf( pOwner->GetValue() - value ) < FLOAT_SAFE_EPSILON ){
		return 0;
	}*/
	
	try{
		pOwner->SetValue(value);
		pOwner->NotifyValueChanged();
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(pOwner, e);
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxSlider::onChanged(FXObject*, FXSelector, void*){
	if(! pOwner->GetEnabled()){
		return 0;
	}
	
	const float value = (float)getValue() * pInvScale;
	if(fabsf(pOwner->GetValue() - value) < FLOAT_SAFE_EPSILON){
		return 0;
	}
	
	try{
		pOwner->SetValue(value);
		pOwner->NotifyValueChanging();
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(pOwner, e);
		return 0;
	}
	
	return 1;
}

#endif
