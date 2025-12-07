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

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "ceViewConversation.h"
#include "ceWindowMain.h"
#include "ceCanvasRuleOfThirdsAid.h"
#include "ceViewConversationListener.h"
#include "../configuration/ceConfiguration.h"
#include "../conversation/ceConversation.h"
#include "../conversation/action/ceCAPlayerChoice.h"
#include "../conversation/action/ceCAPlayerChoiceOption.h"
#include "../conversation/textbox/ceTextBox.h"
#include "../conversation/playback/cePlayback.h"
#include "../conversation/playback/cePlaybackActionStack.h"
#include "../conversation/playback/variable/cePlaybackVariableList.h"
#include "../conversation/playback/cePlaybackActionStackEntry.h"
#include "../conversation/playerChoiceBox/cePCBOption.h"
#include "../conversation/playerChoiceBox/cePlayerChoiceBox.h"
#include "../conversation/infobox/ceConversationInfoBox.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>
#include <deigde/gui/event/igdeMouseKeyListener.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>



// Actions
////////////

namespace {

class cCameraMouseListener : public igdeMouseCameraListener {
	ceViewConversation &pView;
	
public:
	cCameraMouseListener(ceViewConversation &view) : pView(view){}
	
	virtual void OnCameraChanged(){
		if(pView.GetConversation()){
			pView.GetConversation()->NotifyCameraChanged();
		}
	}
};

class cPlaybackListener : public igdeMouseKeyListener {
	ceViewConversation &pView;
	
public:
	cPlaybackListener(ceViewConversation &view) : pView(view){}
	
	virtual void OnButtonPress(igdeWidget*, int, const decPoint &position, int){
		if(!pView.GetConversation()){
			return;
		}
		
		if(AdvancePlayerTriggeredAction()){
			return;
		}
		
		cePlayback &playback = *pView.GetConversation()->GetPlayback();
		cePlayerChoiceBox &pcbox = pView.GetConversation()->GetPlayerChoiceBox();
		if(pcbox.GetOptionList().GetCount() > 0){
			if(pcbox.GetSelectedOption() != -1){
				pcbox.SelectOptionAt(position.x, position.y);
			}
			AcceptPlayerOption();
			return;
		}
		
		// otherwise fast forward speaking
		playback.FastForwardSpeaking();
	}
	
	virtual void OnMouseMoved(igdeWidget*, const decPoint &position, int){
		if(!pView.GetConversation()){
			return;
		}
		
		cePlayerChoiceBox &pcbox = pView.GetConversation()->GetPlayerChoiceBox();
		if(pcbox.GetSelectedOption() != -1){
			pcbox.SelectOptionAt(position.x, position.y);
		}
	}
	
	virtual void OnKeyPress(igdeWidget*, deInputEvent::eKeyCodes keyCode, int){
		if(!pView.GetConversation()){
			return;
		}
		
		switch(keyCode){
		case deInputEvent::ekcSpace:
			if(AdvancePlayerTriggeredAction()){
				return;
			}
			if(pView.GetConversation()->GetPlayerChoiceBox().GetOptionList().GetCount() > 0){
				return;
			}
			pView.GetConversation()->GetPlayback()->FastForwardSpeaking();
			break;
			
		default:
			break;
		}
	}
	
	virtual void OnMouseWheeled(igdeWidget*, const decPoint &, const decPoint &change, int){
		if(!pView.GetConversation()){
			return;
		}
		
		cePlayerChoiceBox &pcbox = pView.GetConversation()->GetPlayerChoiceBox();
		if(pcbox.GetSelectedOption() == -1){
			return;
		}
		
		if(change.y > 0){
			pcbox.SetSelectedOption(decMath::normalize(
				pcbox.GetSelectedOption() - 1, 0, pcbox.GetOptionList().GetCount() - 1));
			
		}else if(change.y < 0){
			pcbox.SetSelectedOption(decMath::normalize(
				pcbox.GetSelectedOption() + 1, 0, pcbox.GetOptionList().GetCount() - 1));
		}
	}
	
	bool AdvancePlayerTriggeredAction(){
		if(!pView.GetConversation()){
			return false;
		}
		
		cePlayback &playback = *pView.GetConversation()->GetPlayback();
		cePlaybackActionStack &stack = playback.GetMainActionStack();
		
		// action user has to advance himself
		ceConversationAction * const action = stack.HasNextAction() ? stack.GetTop().GetNextAction() : NULL;
		if(action){
			switch(action->GetType()){
			case ceConversationAction::eatActorCommand:
			case ceConversationAction::eatGameCommand:
			case ceConversationAction::eatTrigger:
			case ceConversationAction::eatActorAdd:
			case ceConversationAction::eatActorRemove:
			case ceConversationAction::eatCoordSystemAdd:
			case ceConversationAction::eatCoordSystemRemove:
				playback.GetConversation().GetInfoBox().Clear();
				playback.AdvanceToNextAction();
				return true;
				
			default:
				break;
			}
		}
		
		return false;
	}
	
	void AcceptPlayerOption(){
		cePlayerChoiceBox &pcbox = pView.GetConversation()->GetPlayerChoiceBox();
		if(pcbox.GetSelectedOption() == -1){
			return;
		}
		
		const int selectedOption = pcbox.GetSelectedOption();
		ceCAPlayerChoiceOption *option = NULL;
		ceCAPlayerChoice *action = NULL;
		
		if(selectedOption != -1){
			const cePCBOption &pcbOption = *pcbox.GetOptionList().GetAt(selectedOption);
			action = pcbOption.GetAction();
			option = pcbOption.GetActionOption();
		}
		
		pcbox.Clear();
		
		cePlayback &playback = *pView.GetConversation()->GetPlayback();
		if(action && option){
			if(!action->GetVariableName().IsEmpty()){
				playback.GetVariables().Set(action->GetVariableName(), selectedOption);
				playback.GetConversation().NotifyPlaybackVarListChanged();
			}
			
			// return to original stack position to continue
			playback.CancelLoopingLayer(pcbox.GetPlaybackStackDepth());
			
			// push option actions to stack or advance to the next action if missing
			if(option->GetActions().GetCount() > 0){
				playback.GetMainActionStack()->Push(NULL, action, &option->GetActions(), 0);
				
			}else{
				playback.AdvanceToNextAction();
			}
			
		}else{
			playback.AdvanceToNextAction();
		}
	}
};

}



// Class ceViewConversation
/////////////////////////////

// Constructor, destructor
////////////////////////////

ceViewConversation::ceViewConversation(ceWindowMain &windowMain) :
igdeViewRenderWindow(windowMain.GetEnvironment()),
pWindowMain(windowMain),
pListener(NULL),
pConversation(NULL),
pRuleOfThirdsAid(NULL)
{
	pListener = new ceViewConversationListener(*this);
	
	pCameraMouseListener.TakeOver(new cCameraMouseListener(*this));
	AddListener(pCameraMouseListener);
	
	pPlaybackListener.TakeOver(new cPlaybackListener(*this));
	AddListener(pPlaybackListener);
}

ceViewConversation::~ceViewConversation(){
	SetConversation(NULL);
	
	if(pRuleOfThirdsAid){
		delete pRuleOfThirdsAid;
	}
}



// Management
///////////////

void ceViewConversation::ResetView(){
}

void ceViewConversation::SetConversation(ceConversation *conversation){
	if(conversation == pConversation){
		return;
	}
	
	if(pRuleOfThirdsAid){
		pRuleOfThirdsAid->SetVisible(true);
	}
	pCameraMouseListener->SetCamera(NULL);
	SetRenderWorld(NULL);
	
	if(pConversation){
		if(GetRenderWindow()){
			RemoveCanvas(pConversation->GetInfoBox().GetCanvasView());
			RemoveCanvas(pConversation->GetPlayerChoiceBox().GetCanvasView());
			RemoveCanvas(pConversation->GetTextBox()->GetCanvasView());
		}
		pConversation->RemoveListener(pListener);
	}
	
	pConversation = conversation;
	
	if(conversation){
		conversation->AddReference();
		conversation->AddListener(pListener);
		
		if(GetRenderWindow()){
			AddCanvas(conversation->GetTextBox()->GetCanvasView());
			AddCanvas(conversation->GetPlayerChoiceBox().GetCanvasView());
			AddCanvas(conversation->GetInfoBox().GetCanvasView());
			conversation->GetTextBox()->UpdateCanvas();
			if(pRuleOfThirdsAid){
				pRuleOfThirdsAid->SetVisible(conversation->GetShowRuleOfThirdsAid());
			}
			
			SetRenderWorld(conversation->GetCamera()->GetEngineCamera());
		}
		
		pCameraMouseListener->SetCamera(conversation->GetCameraFree());
	}
}

void ceViewConversation::GetViewportWithRatio(decBoundary &viewport) const{
	const decPoint size(GetRenderAreaSize());
	
	if(size.x == 0 || size.y == 0 || !pConversation){
		viewport.x1 = 0;
		viewport.y1 = 0;
		viewport.x2 = 1;
		viewport.y2 = 1;
		return;
	}
	
	const float ratio = (float)size.x / (float)size.y;
	const float viewRatio = pConversation->GetScreenRatio();
	
	if(viewRatio > ratio){
		const int viewHeight = (int)((float)size.x / viewRatio);
		
		viewport.x1 = 0;
		viewport.y1 = (size.y - viewHeight) / 2;
		viewport.x2 = size.x;
		viewport.y2 = viewport.y1 + viewHeight;
		
	}else{
		const int viewWidth = (int)((float)size.y * viewRatio);
		
		viewport.x1 = (size.x - viewWidth) / 2;
		viewport.y1 = 0;
		viewport.x2 = viewport.x1 + viewWidth;
		viewport.y2 = size.y;
	}
}



void ceViewConversation::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if(pConversation){
		AddCanvas(pConversation->GetTextBox()->GetCanvasView());
		AddCanvas(pConversation->GetPlayerChoiceBox().GetCanvasView());
		AddCanvas(pConversation->GetInfoBox().GetCanvasView());
		pConversation->GetTextBox()->UpdateCanvas();
	}
	
	if(!pRuleOfThirdsAid){
		pRuleOfThirdsAid = new ceCanvasRuleOfThirdsAid(pWindowMain.GetEnvironment());
		AddCanvas(pRuleOfThirdsAid->GetCanvasView());
	}
	
	pCanvasBackground.TakeOver(pWindowMain.GetEngine()->GetCanvasManager()->CreateCanvasPaint());
	pCanvasBackground->SetShapeType(deCanvasPaint::estRectangle);
	pCanvasBackground->SetFillColor(decColor(0.0f, 0.0f, 0.0f));
	pCanvasBackground->SetLineColor(pCanvasBackground->GetFillColor());
	pCanvasBackground->SetThickness(0.0f);
	pCanvasBackground->SetOrder(-1.0f); // move it behind render window which is usually 0
	AddCanvas(pCanvasBackground);
	
	if(pConversation){
		SetRenderWorld(pConversation->GetCamera()->GetEngineCamera());
	}
}

void ceViewConversation::OnResize(){
	igdeViewRenderWindow::OnResize();
	
	if(pRuleOfThirdsAid){
		pRuleOfThirdsAid->Resize();
	}
	
	if(pConversation){
		pConversation->GetTextBox()->UpdateCanvas();
		pConversation->GetPlayerChoiceBox().UpdateCanvas();
		pConversation->GetInfoBox().UpdateCanvas();
	}
	
	if(pCanvasBackground){
		pCanvasBackground->SetSize(pCanvasBackground->GetParentView()->GetSize());
	}
	
	// adjust screen format preview
	deCanvasRenderWorld * const canvasRender = GetCanvasRenderWorld();
	if(canvasRender){
		decBoundary viewport;
		GetViewportWithRatio(viewport);
		canvasRender->SetPosition(decPoint(viewport.x1, viewport.y1));
		canvasRender->SetSize(decPoint(viewport.x2 - viewport.x1, viewport.y2 - viewport.y1));
	}
}

void ceViewConversation::OnFrameUpdate(float elapsed){
	igdeViewRenderWindow::OnFrameUpdate(elapsed);
	
	if(pConversation){
		pConversation->Update(elapsed);
	}
}
