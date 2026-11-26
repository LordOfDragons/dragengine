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

#include "igdeInfoBubbleToast.h"
#include "igdeEditorWindow.h"
#include "igdeViewRenderWindow.h"
#include "resources/igdeFont.h"
#include "../environment/igdeEnvironment.h"
#include "../engine/igdeEngineController.h"
#include "../localization/igdeTranslationManager.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>


// Class igdeInfoBubbleToast::EnvironmentListener
///////////////////////////////////////////////////

igdeInfoBubbleToast::EnvironmentListener::EnvironmentListener(igdeInfoBubbleToast *atoast) :
toast(atoast){
}

igdeInfoBubbleToast::EnvironmentListener::~EnvironmentListener() = default;

void igdeInfoBubbleToast::EnvironmentListener::OnFrameUpdate(float elapsed){
	if(toast){
		toast->pUpdateToast(elapsed);
	}
}


// Class igdeInfoBubbleToast
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeInfoBubbleToast::igdeInfoBubbleToast(igdeViewRenderWindow &view, float order) :
igdeInfoBubble(view, order),
pFadeInTime(0.1f),
pFadeOutTime(0.25f),
pVisibleTimePerCharacter(0.1f),
pMaxVisibleTime(6.0f),
pVisibleTime(0.0f),
pElapsed(0.0f),
pToastX(0.5f),
pToastY(0.85f),
pToastTransparency(0.85f),
pState(State::hidden)
{
	igdeFont::sConfiguration fc;
	GetView().GetEnvironment().GetApplicationFont(fc);
	fc.size *= 2.0f;
	SetFont(igdeFont::Ref(GetView().GetEnvironment().GetSharedFont(fc)));
	SetTransparency(0.0f);
}

igdeInfoBubbleToast::~igdeInfoBubbleToast(){
	if(pEnvironmentListener){
		GetView().GetEnvironment().RemoveListener(pEnvironmentListener);
		pEnvironmentListener->toast = nullptr;
	}
}


// Management
///////////////

void igdeInfoBubbleToast::SetFadeInTime(float time){
	pFadeInTime = decMath::max(time, 0.0f);
}

void igdeInfoBubbleToast::SetFadeOutTime(float time){
	pFadeOutTime = decMath::max(time, 0.0f);
}

void igdeInfoBubbleToast::SetVisibleTimePerCharacter(float time){
	pVisibleTimePerCharacter = decMath::max(time, 0.0f);
}

void igdeInfoBubbleToast::SetMaxVisibleTime(float time){
	pMaxVisibleTime = decMath::max(time, 0.0f);
}

void igdeInfoBubbleToast::SetToastX(float x){
	pToastX = decMath::clamp(x, 0.0f, 1.0f);
}

void igdeInfoBubbleToast::SetToastY(float y){
	pToastY = decMath::clamp(y, 0.0f, 1.0f);
}

void igdeInfoBubbleToast::SetToastTransparency(float transparency){
	pToastTransparency = decMath::clamp(transparency, 0.0f, 1.0f);
}


void igdeInfoBubbleToast::ShowToast(const decString &text){
	SetText(GetView().GetEnvironment().GetTranslationManager().TranslateIf(text).ToUTF8());
	pVisibleTime = CalculateVisibleTime(GetText());
	
	switch(pState){
	case State::hidden:
		pState = State::fadeIn;
		pElapsed = 0.0f;
		SetTransparency(0.0f);
		ShowCentered(pToastX, pToastY);
		break;
		
	case State::fadeIn:
		break;
		
	case State::visible:
		pElapsed = 0.0f;
		break;
		
	case State::fadeOut:
		pState = State::fadeIn;
		pElapsed = decMath::linearStep(pElapsed, 0.0f, pFadeOutTime, pFadeInTime, 0.0f);
		break;
	}
	
	if(!pEnvironmentListener){
		pEnvironmentListener = deTObjectReference<EnvironmentListener>::New(this);
		GetView().GetEnvironment().AddListener(pEnvironmentListener);
	}
}

void igdeInfoBubbleToast::HideToast(){
	pState = State::hidden;
	pElapsed = 0.0f;
	SetTransparency(0.0f);
	Hide();
}


float igdeInfoBubbleToast::CalculateVisibleTime(const decString &text) const{
	return decMath::min(pVisibleTimePerCharacter * (float)text.GetLength(), pMaxVisibleTime);
}


// Protected Functions
////////////////////////

void igdeInfoBubbleToast::pUpdateToast(float elapsed){
	if(pState == State::hidden){
		return;
	}
	
	if(pState == State::fadeIn){
		pElapsed += elapsed;
		SetTransparency(decMath::linearStep(pElapsed, 0.0f, pFadeInTime, 0.0f, pToastTransparency));
		if(pElapsed >= pFadeInTime){
			pState = State::visible;
			pElapsed -= pFadeInTime;
		}
	}
	
	if(pState == State::visible){
		pElapsed += elapsed;
		SetTransparency(pToastTransparency);
		if(pElapsed >= pVisibleTime){
			pState = State::fadeOut;
			pElapsed -= pVisibleTime;
		}
	}
	
	if(pState == State::fadeOut){
		pElapsed += elapsed;
		SetTransparency(decMath::linearStep(pElapsed, 0.0f, pFadeOutTime, pToastTransparency, 0.0f));
		if(pElapsed >= pFadeOutTime){
			pState = State::hidden;
			pElapsed = 0.0f;
			Hide();
		}
	}
}
