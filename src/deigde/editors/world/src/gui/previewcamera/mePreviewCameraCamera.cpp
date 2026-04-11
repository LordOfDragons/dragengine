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

#include "mePreviewCamera.h"
#include "mePreviewCameraCamera.h"
#include "../meWindowMain.h"
#include "../../configuration/meConfiguration.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/wrapper/object/igdeWOSOVisitor.h>
#include <deigde/gui/wrapper/object/igdeWOSOComponent.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontSize.h>
#include <dragengine/resources/canvas/deCanvasManager.h>


namespace{

class cCollectIgnoreResources : public igdeWOSOVisitor{
private:
	decTObjectSet<deComponent> &pComponents;
	
public:
	cCollectIgnoreResources(decTObjectSet<deComponent> &components) :
	pComponents(components){
	}
	
	void VisitComponent(igdeWOSOComponent &component) override{
		if(component.GetComponent()){
			pComponents.Add(component.GetComponent());
		}
		if(component.GetComponentInteraction()){
			pComponents.Add(component.GetComponentInteraction());
		}
		if(component.GetOutlineComponent()){
			pComponents.Add(component.GetOutlineComponent());
		}
	}
};

}


// Class mePreviewCameraCamera
////////////////////////////////

// Constructor, destructor
////////////////////////////

mePreviewCameraCamera::mePreviewCameraCamera(mePreviewCamera &previewCamera, meObject *object) :
pPreviewCamera(previewCamera),
pObject(object)
{
	DEASSERT_NOTNULL(pObject)
	DEASSERT_NOTNULL(pObject->GetCamera())
	
	pCreateContent();
}

mePreviewCameraCamera::~mePreviewCameraCamera(){
	auto container = pPreviewCamera.GetPanel();
	if(container){
		container->RemoveCanvas(pCanvasPanel);
	}
}


// Management
///////////////

void mePreviewCameraCamera::Update(float elapsed){
	pUpdateIgnoreResources();
}


// Private Functions
//////////////////////

void mePreviewCameraCamera::pCreateContent(){
	deCanvasManager &canvasMgr = *pPreviewCamera.GetWindowMain().GetEngine()->GetCanvasManager();
	deCanvasView &parentPanel = pPreviewCamera.GetPanel();
	
	const decPoint &renderWorldSize = pPreviewCamera.GetRenderWorldSize();
	const auto &font = pPreviewCamera.GetFontLabel()->GetEngineFont();
	const meCamera &camera = *pObject->GetCamera();
	const decBoundary borderSize(1, 1, 1, 1);
	
	const decString &label = camera.GetName();
	const decPoint labelSize(font->TextSize(label));
	
	pCanvasPanel = canvasMgr.CreateCanvasView();
	pCanvasPanel->SetOrder((float)parentPanel.GetCanvasCount());
	pCanvasPanel->SetSize(renderWorldSize + borderSize.GetSize());
	
	pCanvasRenderWorld = canvasMgr.CreateCanvasRenderWorld();
	pCanvasRenderWorld->SetPosition(borderSize.GetTopLeft());
	pCanvasRenderWorld->SetSize(renderWorldSize);
	pCanvasRenderWorld->SetCamera(camera.GetEngineCamera());
	pCanvasRenderWorld->SetOrder(0.0f);
	pCanvasPanel->AddCanvas(pCanvasRenderWorld);
	
	pCanvasLabelBg = canvasMgr.CreateCanvasPaint();
	pCanvasLabelBg->SetPosition(borderSize.GetTopLeft());
	pCanvasLabelBg->SetSize(labelSize + decPoint(2, 2));
	pCanvasLabelBg->SetFillColor({0.0f, 0.0f, 0.0f, 0.5f});
	pCanvasLabelBg->SetOrder(1.0f);
	pCanvasPanel->AddCanvas(pCanvasLabelBg);
	
	pCanvasLabel = canvasMgr.CreateCanvasText();
	pCanvasLabel->SetPosition(borderSize.GetTopLeft() + decPoint(1, 1));
	pCanvasLabel->SetSize(labelSize);
	pCanvasLabel->SetText(label);
	pCanvasLabel->SetColor({1.0f, 1.0f, 1.0f});
	pCanvasLabel->SetFont(font);
	pCanvasLabel->SetFontSize((float)font->GetLineHeight());
	pCanvasLabel->SetOrder(2.0f);
	pCanvasPanel->AddCanvas(pCanvasLabel);
	
	pCanvasBorder = canvasMgr.CreateCanvasPaint();
	pCanvasBorder->SetPosition({});
	pCanvasBorder->SetSize(renderWorldSize + borderSize.GetSize());
	pCanvasBorder->SetLineColor({1.0f, 0.0f, 0.0f});
	pCanvasBorder->SetThickness(1.0f);
	pCanvasBorder->SetOrder(3.0f);
	pCanvasPanel->AddCanvas(pCanvasBorder);
	
	parentPanel.AddCanvas(pCanvasPanel);
}

void mePreviewCameraCamera::pUpdateIgnoreResources(){
	if(!pObject->GetCamera() || !pObject->GetCamera()->GetEngineCamera()){
		return;
	}
	
	decTObjectSet<deComponent> ignoreComponents;
	if(pObject->GetObjectWrapper()){
		cCollectIgnoreResources visitor(ignoreComponents);
		pObject->GetObjectWrapper()->VisitSubObjects(visitor);
	}
	
	deCamera &camera = pObject->GetCamera()->GetEngineCamera();
	
	if(pIgnoreComponents != ignoreComponents){
		pIgnoreComponents.Visit([&](deComponent *component){
			camera.RemoveIgnoreComponent(component);
		});
		
		ignoreComponents.Visit([&](deComponent *component){
			camera.AddIgnoreComponent(component);
		});
		
		pIgnoreComponents = std::move(ignoreComponents);
	}
}
