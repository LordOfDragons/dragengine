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
#include <stdio.h>
#include <stdlib.h>

#include "ceWDSLane.h"
#include "../ceWindowDopeSheet.h"
#include "../../ceWindowMain.h"
#include "../../dialog/ceDialogEditStrip.h"
#include "../../dialog/ceDialogEditStringWithList.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAActorSpeak.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/actor/speechAnimation/ceSAWord.h"
#include "../../../conversation/actor/speechAnimation/ceSpeechAnimation.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/target/ceTarget.h"
#include "../../../conversation/strip/ceStrip.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../undosys/action/actorSpeak/strip/ceUCAASpeakStripSetDuration.h"
#include "../../../undosys/action/actorSpeak/strip/ceUCAASpeakStripSetPause.h"
#include "../../../undosys/action/actorSpeak/strip/ceUCAASpeakStripsScale.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeMouseDragListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/theme/igdeGuiTheme.h>
#include <deigde/gui/theme/propertyNames.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>



// Actions
////////////

namespace {

class cListenerResetDuration : public ceDialogEditStrip::Listener{
	ceWDSLane &pLane;
	
public:
	typedef deTObjectReference<cListenerResetDuration> Ref;
	
	cListenerResetDuration(ceWDSLane &lane) : pLane(lane){}
	
	virtual float DefaultDuration (const decString &id){
		return pLane.DefaultDuration(id);
	}
};

class cActionStripAdd : public igdeAction{
public:
	typedef deTObjectReference<cActionStripAdd> Ref;
	
private:
	ceWDSLane &pLane;
	const int pIndex;
	
public:
	cActionStripAdd(ceWDSLane &lane, int index) : igdeAction("Add...",
		lane.GetWindow().GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add element"), pLane(lane), pIndex(index){}
	
	void OnAction() override{
		if(!pLane.GetWindow().GetActionASpeak()){
			return;
		}
		
		decString title, text;
		title.Format("Add %s", pLane.GetLabel().GetString());
		text.Format("%s: ", pLane.GetLabel().GetString());
		const ceDialogEditStrip::Ref dialog(ceDialogEditStrip::Ref::New(
			pLane.GetWindow().GetEnvironment(), title, text));
		
		decStringList idList;
		pLane.FillIDList(idList);
		dialog->SetIDList(idList);
		dialog->SetListener(cListenerResetDuration::Ref::New(pLane));
		dialog->ResetDuration();
		dialog->SetAutoResetDuration(true);
		
		if(!dialog->Run(&pLane.GetWindow().GetWindowMain())){
			return;
		}
		
		pLane.GetWindow().GetConversation()->GetUndoSystem()->Add(
			pLane.UndoStripAdd(dialog->CreateStrip(), decMath::max(pIndex, 0)));
	}
	
	void Update() override{
		SetEnabled(pLane.GetWindow().GetActionASpeak());
	}
};

class cActionStripRemove : public igdeAction{
public:
	typedef deTObjectReference<cActionStripRemove> Ref;
	
private:
	ceWDSLane &pLane;
	ceStrip *pStrip;
	
public:
	cActionStripRemove(ceWDSLane &lane, ceStrip *strip) : igdeAction("Remove",
		lane.GetWindow().GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove element"), pLane(lane), pStrip(strip){}
	
	void OnAction() override{
		if(!pStrip){
			return;
		}
		
		igdeUndo::Ref undo(pLane.UndoStripRemove(pStrip));
		pLane.GetWindow().GetConversation()->GetUndoSystem()->Add(undo);
	}
	
	void Update() override{
		SetEnabled(pStrip);
	}
};

class cActionStripRemoveAll : public igdeAction{
public:
	typedef deTObjectReference<cActionStripRemoveAll> Ref;
	
private:
	ceWDSLane &pLane;
	
public:
	cActionStripRemoveAll(ceWDSLane &lane) : igdeAction("Remove All",
		lane.GetWindow().GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove all elements"), pLane(lane){}
	
	void OnAction() override{
		if(!pLane.GetWindow().GetActionASpeak()){
			return;
		}
		
		igdeUndo::Ref undo(pLane.UndoStripRemoveAll());
		pLane.GetWindow().GetConversation()->GetUndoSystem()->Add(undo);
	}
	
	void Update() override{
		SetEnabled(pLane.GetWindow().GetActionASpeak());
	}
};

class cActionStripMoveLeft : public igdeAction{
public:
	typedef deTObjectReference<cActionStripMoveLeft> Ref;
	
private:
	ceWDSLane &pLane;
	int pIndex;
	
public:
	cActionStripMoveLeft(ceWDSLane &lane, int index) : igdeAction("Move Left",
		lane.GetWindow().GetEnvironment().GetStockIcon(igdeEnvironment::esiLeft),
		"Switch element with element on the left"), pLane(lane), pIndex(index){}
	
	void OnAction() override{
		if(!pLane.GetWindow().GetActionASpeak() || pIndex < 1){
			return;
		}
		
		igdeUndo::Ref undo(pLane.UndoStripMove(pLane.GetStripList().GetAt(pIndex), pIndex - 1));
		pLane.GetWindow().GetConversation()->GetUndoSystem()->Add(undo);
	}
	
	void Update() override{
		SetEnabled(pLane.GetWindow().GetActionASpeak() && pIndex > 0);
	}
};

class cActionStripMoveRight : public igdeAction{
public:
	typedef deTObjectReference<cActionStripMoveRight> Ref;
	
private:
	ceWDSLane &pLane;
	int pIndex;
	
public:
	cActionStripMoveRight(ceWDSLane &lane, int index) : igdeAction("Move Right",
		lane.GetWindow().GetEnvironment().GetStockIcon(igdeEnvironment::esiRight),
		"Switch element with element on the right"), pLane(lane), pIndex(index){}
	
	void OnAction() override{
		if(!pLane.GetWindow().GetActionASpeak() || pIndex > pLane.GetStripList().GetCount() - 2){
			return;
		}
		
		igdeUndo::Ref undo(pLane.UndoStripMove(pLane.GetStripList().GetAt(pIndex), pIndex + 1));
		pLane.GetWindow().GetConversation()->GetUndoSystem()->Add(undo);
	}
	
	void Update() override{
		SetEnabled(pLane.GetWindow().GetActionASpeak() && pIndex >= 0
			&& pIndex < pLane.GetStripList().GetCount() - 1);
	}
};

class cActionStripEdit : public igdeAction{
public:
	typedef deTObjectReference<cActionStripEdit> Ref;
	
private:
	ceWDSLane &pLane;
	ceStrip *pStrip;
	
public:
	cActionStripEdit(ceWDSLane &lane, ceStrip *strip) : igdeAction("Edit...", nullptr,
		"Edit element"), pLane(lane), pStrip(strip){}
	
	void OnAction() override{
		if(pStrip){
			pLane.EditStrip(pStrip);
		}
	}
	
	void Update() override{
		SetEnabled(pStrip);
	}
};


class cMouseListener : public igdeMouseDragListener {
	enum eDragModes{
		edmNone,
		edmPause,
		edmDuration,
		edmScale
	};
	
	ceWDSLane &pLane;
	eDragModes pDragMode;
	int pDragHandle;
	float pOldEntryValue;
	int pScaleCount;
	float pScaleAnchor;
	float pScaleLength;
	igdeUndo::Ref pUndoDuration;
	igdeUndo::Ref pUndoPause;
	igdeUndo::Ref pUndoScale;
	
public:
	typedef deTObjectReference<cMouseListener> Ref;
	
	cMouseListener(ceWDSLane &lane) : pLane(lane), pDragMode(edmNone){}
	
	virtual bool OnDragBegin(){
		const ceCAActorSpeak * const action = pLane.GetWindow().GetActionASpeak();
		if(!action){
			return false;
		}
		
		const ceStrip::List &strips = pLane.GetStripList();
		pDragMode = edmNone;
		pUndoDuration = nullptr;
		pUndoPause = nullptr;
		pUndoScale = nullptr;
		pScaleCount = 0;
		
		pDragHandle = pLane.GetStripPauseAt(GetDragOrigin().x);
		if(pDragHandle != -1){
			pOldEntryValue = strips.GetAt(pDragHandle)->GetPause();
			if(pOldEntryValue > FLOAT_SAFE_EPSILON || GetShiftOrigin()){
				pDragMode = edmPause;
				pLane.SetSelection(pDragHandle, pDragHandle);
				return true;
			}
		}
		
		pDragHandle = pLane.GetStripDurationAt(GetDragOrigin().x);
		if(pDragHandle != -1){
			pOldEntryValue = strips.GetAt(pDragHandle)->GetDuration();
			pDragMode = edmDuration;
			pLane.SetSelection(pDragHandle, pDragHandle);
			return true;
		}
		
		pLane.SetSelection(-1, -1);
		return false;
	}
	
	virtual void OnDragUpdate(){
		const ceCAActorSpeak * const action = pLane.GetWindow().GetActionASpeak();
		if(!action){
			return;
		}
		
		const float time = pLane.GetWindow().GetTimeForX(GetDragPosition().x);
		
		if(pUndoScale){
			pUndoScale->Undo();
			pUndoScale.DynamicCast<ceUCAASpeakStripsScale>()->SetScaling(
				(time - pScaleAnchor) / pScaleLength);
			pUndoScale->Redo();
			return;
		}
		
		const ceStrip::List &strips = pLane.GetStripList();
		
		switch(pDragMode){
		case edmPause:{
			float newPause = time - pLane.GetStripStartFor(pDragHandle);
			if(newPause < 0.01f){
				newPause = 0.0f;
			}
			
			if(pUndoPause){
				pUndoPause.DynamicCast<ceUCAASpeakStripSetPause>()->SetNewPause(newPause);
				pUndoPause.DynamicCast<ceUCAASpeakStripSetPause>()->ProgressiveRedo();
				
			}else{
				pUndoPause = pLane.UndoStripSetPause(strips.GetAt(pDragHandle), newPause);
				pUndoPause->Redo();
			}
			}break;
			
		case edmDuration:{
			float newDuration = time - pLane.GetStripPauseFor(pDragHandle);
			if(newDuration < 0.01f){
				newDuration = 0.01f;
			}
			
			if(pUndoDuration){
				pUndoDuration.DynamicCast<ceUCAASpeakStripSetDuration>()->SetNewDuration(newDuration);
				pUndoDuration.DynamicCast<ceUCAASpeakStripSetDuration>()->ProgressiveRedo();
				
			}else{
				pUndoDuration = pLane.UndoStripSetDuration(strips.GetAt(pDragHandle), newDuration);
				pUndoDuration->Redo();
			}
			}break;
			
		default:
			break;
		}
	}
	
	virtual void OnDragFinish(bool){
		if(pUndoPause){
			pLane.GetWindow().GetConversation()->GetUndoSystem()->Add(pUndoPause, false);
			pUndoPause = nullptr;
		}
		
		if(pUndoDuration){
			pLane.GetWindow().GetConversation()->GetUndoSystem()->Add(pUndoDuration, false);
			pUndoDuration = nullptr;
		}
		
		if(pUndoScale){
			pLane.GetWindow().GetConversation()->GetUndoSystem()->Add(pUndoScale, false);
			pUndoScale = nullptr;
		}
		
		pDragMode = edmNone;
		pLane.SetSelection(-1, -1);
	}
	
	virtual void OnMouseWheeled(igdeWidget*, const decPoint &, const decPoint &change, int){
		switch(pDragMode){
		case edmDuration:
		case edmScale:
			break;
			
		default:
			return;
		}
		
		// adjust list of strips to scale if it changed
		if(change.y > 0){ // up
			if(pScaleCount == pDragHandle){
				return;
			}
			pScaleCount++;
			
		}else if(change.y < 0){ // down
			if(pScaleCount == 0){
				return;
			}
			pScaleCount--;
			
		}else{
			return;
		}
		
		// if there is an undo duration undo and delete it
		if(pUndoDuration){
			pUndoDuration->Undo();
			pUndoDuration = nullptr;
		}
		
		// if undo scaling does not exist yet create it if we had been in a duration drag mode
		if(!pUndoScale){
			if(pDragMode == edmDuration){
				pUndoScale = pLane.UndoScaleStrips();
				pDragMode = edmScale;
				
			}else{
				return;
			}
		}
		
		// update undo
		const ceStrip::List &list = pLane.GetStripList();
		ceStrip::List applyList;
		int i;
		
		pUndoScale->Undo();
		
		pScaleAnchor = 0.0f;
		for(i=0; i<pDragHandle - pScaleCount; i++){
			const ceStrip &strip = list.GetAt(i);
			pScaleAnchor += strip.GetPause() + strip.GetDuration();
		}
		
		pScaleLength = 0.0f;
		for(i=pDragHandle - pScaleCount; i<=pDragHandle; i++){
			ceStrip * const strip = list.GetAt(i);
			applyList.Add(strip);
			pScaleLength += strip->GetPause() + strip->GetDuration();
		}
		if(pScaleLength < FLOAT_SAFE_EPSILON){
			pScaleLength = 1.0f;
		}
		
		pUndoScale.DynamicCast<ceUCAASpeakStripsScale>()->SetStrips(applyList);
		pUndoScale.DynamicCast<ceUCAASpeakStripsScale>()->SetScaling(
			(pLane.GetWindow().GetTimeForX(GetDragPosition().x) - pScaleAnchor) / pScaleLength);
		
		pUndoScale->Redo();
		
		pLane.SetSelection(pDragHandle - pScaleCount, pDragHandle);
	}
	
	virtual void OnDoubleClicked(igdeWidget*, int button, const decPoint &position, int){
		if(button == deInputEvent::embcLeft){
			const int index = pLane.GetStripAt(position.x);
			if(index >= 0 && index < pLane.GetStripList().GetCount()){
				pLane.EditStrip(pLane.GetStripList().GetAt(index));
			}
		}
	}
	
	virtual void OnButtonPress(igdeWidget *widget, int button, const decPoint &position, int modifiers){
		if(button != deInputEvent::embcRight){
			igdeMouseDragListener::OnButtonPress(widget, button, position, modifiers);
			return;
		}
		
		igdeMenuCascade::Ref contextMenu(igdeMenuCascade::Ref::New(pLane.GetWindow().GetEnvironment()));
		
		pLane.OnContextMenu(contextMenu, position);
		
		contextMenu->Popup(pLane.GetWindow().GetDopeSheet(), position);
	}
};

}



// Class ceWDSLane::cStrip
////////////////////////////

ceWDSLane::cStrip::cStrip(){}
ceWDSLane::cStrip::~cStrip(){}



// Class ceWDSLane
////////////////////

// Constructor, destructor
////////////////////////////

ceWDSLane::ceWDSLane(ceWindowDopeSheet &dopeSheet, int index, const char *label, const char *description) :
pWindow(dopeSheet),
pIndex(index),
pLabel(label),
pDescription(description),

pBarHeight(8),
pSelectionFrom(-1),
pSelectionTo(-1)
{
	pMouseKeyListener = cMouseListener::Ref::New(*this);
}

ceWDSLane::~ceWDSLane(){
}



// Management
///////////////

int ceWDSLane::GetStripAt(int x) const{
	const int left = -pWindow.GetScrollTime();
	if(x < left){
		return -1;
	}
	
	float entryTime = 0.0f;
	return GetStripList().IndexOfMatching([&](const ceStrip &s){
		const int fromX = left + (int)(entryTime * pWindow.GetPixelPerSecond());
		entryTime += s.GetPause() + s.GetDuration();
		const int toX = left + (int)(entryTime * pWindow.GetPixelPerSecond());
		return x >= fromX && x < toX;
	});
}

int ceWDSLane::GetInsertStripAt(int x) const{
	const int index = GetStripAt(x);
	if(index != -1){
		return index;
		
	}else if(x < -pWindow.GetScrollTime()){
		return 0;
		
	}else{
		return GetStripList().GetCount();
	}
}

int ceWDSLane::GetStripPauseAt(int x) const{
	const int left = -pWindow.GetScrollTime();
	float entryTime = 0.0f;
	
	return GetStripList().IndexOfMatching([&](const ceStrip &s){
		entryTime += s.GetPause();
		const int ex = left + (int)(entryTime * pWindow.GetPixelPerSecond());
		
		if(x >= ex - 4 && x < ex + 4){
			return true;
			
		}else{
			entryTime += s.GetDuration();
			return false;
		}
	});
}

int ceWDSLane::GetStripDurationAt(int x) const{
	const int left = -pWindow.GetScrollTime();
	float entryTime = 0.0f;
	
	return GetStripList().IndexOfMatching([&](const ceStrip &s){
		entryTime += s.GetPause() + s.GetDuration();
		const int ex = left + (int)(entryTime * pWindow.GetPixelPerSecond());
		return x >= ex - 4 && x < ex + 4;
	});
}

float ceWDSLane::GetStripStartFor(int index) const{
	const ceStrip::List &list = GetStripList();
	if(index >= list.GetCount()){
		index = list.GetCount() - 1;
	}
	
	return list.Inject(0.0f, [](float t, const ceStrip &s){
		return t + s.GetPause() + s.GetDuration();
	}, 0, index);
}

float ceWDSLane::GetStripPauseFor(int index) const{
	const ceStrip::List &list = GetStripList();
	if(index >= list.GetCount()){
		index = list.GetCount() - 1;
	}
	
	return GetStripList().Inject(0.0f, [](float t, const ceStrip &s){
		return t + s.GetPause() + s.GetDuration();
	}, 0, index) + list.GetAt(index)->GetPause();
}

float ceWDSLane::GetMaximumLineTime() const{
	return GetStripList().Inject(0.0f, [](float t, const ceStrip &s){
		return t + s.GetPause() + s.GetDuration();
	});
}

void ceWDSLane::SetSelection(int from, int to){
	if(from == pSelectionFrom && to == pSelectionTo){
		return;
	}
	
	pSelectionFrom = from;
	pSelectionTo = to;
	
	UpdateCanvas();
}



void ceWDSLane::OnResize(){
	RebuildCanvas();
	UpdateCanvas();
}

void ceWDSLane::OnActionChanged(){
	RebuildCanvas();
	UpdateCanvas();
}

void ceWDSLane::OnTimeChanged(){
	UpdateCanvas();
}



void ceWDSLane::CreateCanvas(){
	igdeEnvironment &env = pWindow.GetEnvironment();
	const decColor colorBarFill(env.GetSystemColor(igdeEnvironment::escWidgetBackground));
	const decColor colorBarLine(0.0f, 0.0f, 0.0f); //env.GetSystemColor(igdeEnvironment::escWidgetShadow));
	const decColor colorBarSelectionFill(0.65f, 0.65f, 1.0f); //env.GetSystemColor(igdeEnvironment::escWidgetSelectedBackground));
	
	deCanvasManager &canvasManager = *pWindow.GetEngine()->GetCanvasManager();
	pCanvas = canvasManager.CreateCanvasView();
	
	pCanvasPanelSheet = canvasManager.CreateCanvasView();
	pCanvasPanelSheet->SetOrder((float)pCanvas->GetCanvasCount());
	pCanvas->AddCanvas(pCanvasPanelSheet);
	
	pCanvasBar = canvasManager.CreateCanvasPaint();
	pCanvasBar->SetFillColor(colorBarFill);
	pCanvasBar->SetLineColor(colorBarLine);
	pCanvasBar->SetThickness(1);
	pCanvasBar->SetOrder((float)pCanvasPanelSheet->GetCanvasCount());
	pCanvasPanelSheet->AddCanvas(pCanvasBar);
	
	pCanvasBarSelection = canvasManager.CreateCanvasPaint();
	pCanvasBarSelection->SetFillColor(colorBarSelectionFill);
	pCanvasBarSelection->SetThickness(0);
	pCanvasBarSelection->SetVisible(false);
	pCanvasBarSelection->SetOrder((float)pCanvasPanelSheet->GetCanvasCount());
	pCanvasPanelSheet->AddCanvas(pCanvasBarSelection);
	
	pCanvasHandles = canvasManager.CreateCanvasView();
	pCanvasHandles->SetOrder((float)pCanvasPanelSheet->GetCanvasCount());
	pCanvasPanelSheet->AddCanvas(pCanvasHandles);
}

void ceWDSLane::UpdateCanvas(){
	if(!pCanvas){
		return;
	}
	
	igdeEnvironment &env = pWindow.GetEnvironment();
	const decColor colorHandleBg(env.GetSystemColor(igdeEnvironment::escWidgetBackground));
	const decColor colorHandleFg(env.GetSystemColor(igdeEnvironment::escWidgetForeground));
	const decColor colorHandleSelectedBg(0.65f, 0.65f, 1.0f); //env.GetSystemColor(igdeEnvironment::escWidgetSelectedBackground));
	const decColor colorHandleSelectedFg(0.0f, 0.0f, 0.0f); //env.GetSystemColor(igdeEnvironment::escWidgetSelectedForeground));
	const decColor colorTextBg(0.75f, 0.75f, 0.75f);
	const decColor colorTextSelectedBg(colorHandleSelectedBg);
	
	const int center = pCanvas->GetSize().y / 2;
	const ceStrip::List &list = GetStripList();
	const int count = list.GetCount();
	int selectionTimeFrom = 0;
	int selectionTimeTo = 0;
	float time = 0.0f;
	int i, x;
	
	// handles. this also finds the selection parameters for the bar selection below
	for(i=0; i<count; i++){
		const ceStrip &listStrip = list.GetAt(i);
		const cStrip &strip = pStrips.GetAt(i);
		
		const int left = pWindow.GetXForTime(time);
		const int top = center - strip.handleDuration->GetSize().y / 2;
		
		const int left2 = pWindow.GetXForTime(time + listStrip.GetPause());
		x = left2;
		strip.handlePause->SetPosition(decPoint(x - strip.handlePause->GetSize().x / 2, top));
		time += listStrip.GetPause();
		
		x = pWindow.GetXForTime(time + listStrip.GetDuration());
		strip.handleDuration->SetPosition(decPoint(x - strip.handleDuration->GetSize().x / 2, top));
		time += listStrip.GetDuration();
		
		const int right = x;
		
		strip.stripId->SetText(listStrip.GetID());
		strip.stripId->SetSize(strip.stripId->GetFont()->TextSize(strip.stripId->GetText()));
		strip.stripId->SetPosition(decPoint((left2 + right) / 2, center) - strip.stripId->GetSize() / 2);
		
		strip.stripIdBg->SetPosition(strip.stripId->GetPosition() - decPoint(1, 1));
		strip.stripIdBg->SetSize(strip.stripId->GetSize() + decPoint(3, 2));
		
		if(i == pSelectionFrom){
			selectionTimeFrom = left;
		}
		if(i == pSelectionTo){
			selectionTimeTo = right;
		}
		
		// update colors
		if(i >= pSelectionFrom && i <= pSelectionTo){
			strip.handlePauseBg->SetFillColor(colorHandleSelectedBg);
			strip.handleDurationBg->SetFillColor(colorHandleSelectedBg);
			strip.stripId->SetColor(colorHandleSelectedFg);
			strip.stripIdBg->SetFillColor(colorTextSelectedBg);
			
		}else{
			strip.handlePauseBg->SetFillColor(colorHandleBg);
			strip.handleDurationBg->SetFillColor(colorHandleBg);
			strip.stripId->SetColor(colorHandleFg);
			strip.stripIdBg->SetFillColor(colorTextBg);
		}
	}
	
	// bar selection
	if(selectionTimeTo > selectionTimeFrom){
		pCanvasBarSelection->SetPosition(pCanvasBar->GetPosition() + decPoint(selectionTimeFrom, 1));
		pCanvasBarSelection->SetSize(decPoint(selectionTimeTo - selectionTimeFrom, pCanvasBar->GetSize().y - 2));
		pCanvasBarSelection->SetVisible(true);
		
	}else{
		pCanvasBarSelection->SetVisible(false);
	}
}

void ceWDSLane::RebuildCanvas(){
	if(!pCanvas){
		return;
	}
	
	decPoint sizeDopeSheet(pWindow.GetSizeDopeSheet());
	const int laneHeight = pWindow.GetLaneHeight();
	
	pCanvas->SetPosition(decPoint(0, pWindow.GetVAPreviewHeight() + laneHeight * pIndex));
	pCanvas->SetSize(decPoint(sizeDopeSheet.x, laneHeight));
	
	pCanvasPanelSheet->SetPosition(decPoint(0, 0));
	pCanvasPanelSheet->SetSize(decPoint(sizeDopeSheet.x, laneHeight));
	
	pCanvasBar->SetPosition(decPoint(-2, (laneHeight - pBarHeight) / 2));
	pCanvasBar->SetSize(decPoint(pCanvasPanelSheet->GetSize().x + 4, pBarHeight));
	
	// handles
	pCanvasHandles->RemoveAllCanvas();
	pCanvasHandles->SetSize(pCanvasPanelSheet->GetSize());
	
	const ceStrip::List &list = GetStripList();
	const int count = list.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(i == pStrips.GetCount()){
			deFont * const font = pWindow.GetFontText()->GetEngineFont();
			
			deCanvasManager &canvasManager = *pWindow.GetEngine()->GetCanvasManager();
			const decPoint handleSize(5, pBarHeight + 4);
			const decPoint handleSizeHalf(handleSize.x, handleSize.y / 2);
			
			const decColor colorText(pWindow.GetEnvironment().GetSystemColor(igdeEnvironment::escWidgetForeground));
			const decColor colorTextBg(0.75f, 0.75f, 0.75f);
			const decColor colorTextLine(0.25f, 0.25f, 0.25f);
			
			const cStrip::Ref strip(cStrip::Ref::New());
			
			CreateHandle(strip->handlePause, strip->handlePauseBg, handleSizeHalf);
			CreateHandle(strip->handleDuration, strip->handleDurationBg, handleSize);
			
			strip->stripIdBg = canvasManager.CreateCanvasPaint();
			strip->stripIdBg->SetFillColor(colorTextBg);
			strip->stripIdBg->SetLineColor(colorTextLine);
			strip->stripIdBg->SetThickness(1);
			strip->stripIdBg->SetOrder(0);
			
			strip->stripId = canvasManager.CreateCanvasText();
			strip->stripId->SetFont(font);
			strip->stripId->SetFontSize((float)font->GetLineHeight());
			strip->stripId->SetColor(colorText);
			
			pStrips.Add(strip);
		}
		
		cStrip &strip = pStrips.GetAt(i);
		
		strip.handlePause->SetOrder((float)pCanvasHandles->GetCanvasCount());
		pCanvasHandles->AddCanvas(strip.handlePause);
		
		strip.handleDuration->SetOrder((float)pCanvasHandles->GetCanvasCount());
		pCanvasHandles->AddCanvas(strip.handleDuration);
		
		strip.stripIdBg->SetOrder((float)pCanvasHandles->GetCanvasCount());
		pCanvasHandles->AddCanvas(strip.stripIdBg);
		
		strip.stripId->SetOrder((float)pCanvasHandles->GetCanvasCount());
		pCanvasHandles->AddCanvas(strip.stripId);
	}
}

void ceWDSLane::EditStrip(ceStrip *strip){
	ceCAActorSpeak * const action = pWindow.GetActionASpeak();
	if(!action || !strip){
		DETHROW(deeInvalidParam);
	}
	
	igdeDialog::Ref refDialog;
	decString title, text;
	title.Format("Edit %s", pLabel.GetString());
	text.Format("%s: ", pLabel.GetString());
	refDialog = ceDialogEditStrip::Ref::New(pWindow.GetEnvironment(), title, text);
	ceDialogEditStrip &dialog = (ceDialogEditStrip&)(igdeDialog&)refDialog;
	
	decStringList idList;
	FillIDList(idList);
	dialog.SetIDList(idList);
	dialog.SetListener(cListenerResetDuration::Ref::New(*this));
	dialog.SetAutoResetDuration(false);
	
	dialog.SetFromStrip(*strip);
	
	if(!dialog.Run(&pWindow.GetWindowMain())){
		return;
	}
	
	const ceStrip::Ref newStrip(dialog.CreateStrip());
	if(*newStrip == *strip){
		return;
	}
	
	pWindow.GetConversation()->GetUndoSystem()->Add(UndoStripReplace(strip, newStrip));
}

float ceWDSLane::DefaultDuration(const decString &) {
	return 0.5f;
}



void ceWDSLane::OnContextMenu(igdeMenuCascade &menu, const decPoint &position){
	igdeUIHelper &helper = pWindow.GetEnvironment().GetUIHelper();
	
	const int index = GetStripAt(position.x);
	ceStrip * const strip = index != -1 ? GetStripList().GetAt(index) : nullptr;
	const int indexInsert = GetInsertStripAt(position.x);
	
	helper.MenuCommand(menu, cActionStripAdd::Ref::New(*this, indexInsert));
	helper.MenuCommand(menu, cActionStripRemove::Ref::New(*this, strip));
	helper.MenuCommand(menu, cActionStripRemoveAll::Ref::New(*this));
	helper.MenuCommand(menu, cActionStripMoveLeft::Ref::New(*this, index));
	helper.MenuCommand(menu, cActionStripMoveRight::Ref::New(*this, index));
	helper.MenuCommand(menu, cActionStripEdit::Ref::New(*this, strip));
}



// Protected Functions
////////////////////////

void ceWDSLane::FillIDListLookAt(decStringList &list){
	const ceCAActorSpeak * const action = pWindow.GetActionASpeak();
	if(!action){
		return;
	}
	
	pWindow.GetConversation()->AllTargets().Visit([&](const ceTarget &t){
		list.Add(t.GetName());
	});
}

void ceWDSLane::CreateHandle(deCanvasView::Ref &canvas, deCanvasPaint::Ref &canvasBg, const decPoint &size){
	igdeEnvironment &env = pWindow.GetEnvironment();
	const decColor colorBackground(env.GetSystemColor(igdeEnvironment::escWidgetBackground));
	const decColor colorBorder(0.0f, 0.0f, 0.0f); //env.GetSystemColor(igdeEnvironment::escWidgetShadow));
	deCanvasManager &canvasManager = *pWindow.GetEngine()->GetCanvasManager();
	
	canvas = canvasManager.CreateCanvasView();
	canvas->SetSize(size);
	
	canvasBg = canvasManager.CreateCanvasPaint();
	canvasBg->SetFillColor(colorBackground);
	canvasBg->SetLineColor(colorBorder);
	canvasBg->SetThickness(1);
	canvasBg->SetOrder(0);
	canvasBg->SetSize(size);
	canvas->AddCanvas(canvasBg);
	
	deCanvasPaint::Ref canvasKnob(canvasManager.CreateCanvasPaint());
	canvasKnob->SetFillColor(colorBorder);
	canvasKnob->SetThickness(0);
	canvasKnob->SetOrder(1);
	canvasKnob->SetPosition(decPoint(2, 2));
	canvasKnob->SetSize(size - decPoint(4, 4));
	canvas->AddCanvas(canvasKnob);
}
