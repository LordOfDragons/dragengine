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

#include "stClassCanvas.h"
#include "stClassColor.h"
#include "../stClassScripting.h"
#include "../math/stClassPoint.h"
#include "../../ScriptingSmalltalk.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasVisitorIdentify.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/canvas/deCanvasCanvasView.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>
#include <dragengine/resources/canvas/deCanvasVideoPlayer.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/common/exceptions.h>



// Class struct
/////////////////

struct csCanvasClass : public csClassObject{
	OOP scripting;
};

struct csCanvas : public csObject{
	stClassCanvas *clsCanvas;
	deCanvas *canvas;
	deCanvasVisitorIdentify::eCanvasTypes type;
};



// Class stClassCanvas
////////////////////////

// Constructor, destructor
////////////////////////////

stClassCanvas::stClassCanvas(ScriptingSmalltalk &st) :
pST(st){
	// deCanvasVisitorIdentify::eCanvasTypes
	pSymbolImage = gst_symbol_to_oop("image");
	pSymbolPaint = gst_symbol_to_oop("paint");
	pSymbolCanvasView = gst_symbol_to_oop("canvasView");
	pSymbolRenderWorld = gst_symbol_to_oop("renderWorld");
	pSymbolText = gst_symbol_to_oop("text");
	pSymbolVideoPlayer = gst_symbol_to_oop("videoPlayer");
	pSymbolView = gst_symbol_to_oop("view");
	
	// deCanvasPaint::eShapeTypes
	pSymbolPoints = gst_symbol_to_oop("points");
	pSymbolLines = gst_symbol_to_oop("lines");
	pSymbolRectangle = gst_symbol_to_oop("rectangle");
	pSymbolEllipse = gst_symbol_to_oop("ellipse");
	pSymbolPie = gst_symbol_to_oop("pie");
	pSymbolPolygon = gst_symbol_to_oop("polygon");
	
	// functions
	gst_define_cfunc("DECanvas.new", (void*)ccNew);
	gst_define_cfunc("DECanvas.finalize", (void*)ccFinalize);
	
	gst_define_cfunc("DECanvas.getType", (void*)ccGetType);
	gst_define_cfunc("DECanvas.getPosition", (void*)ccGetPosition);
	gst_define_cfunc("DECanvas.setPosition", (void*)ccSetPosition);
	gst_define_cfunc("DECanvas.getSize", (void*)ccGetSize);
	gst_define_cfunc("DECanvas.setSize", (void*)ccSetSize);
	gst_define_cfunc("DECanvas.getTransform", (void*)ccGetTransform);
	gst_define_cfunc("DECanvas.setTransform", (void*)ccSetTransform);
	gst_define_cfunc("DECanvas.getVisible", (void*)ccGetVisible);
	gst_define_cfunc("DECanvas.setVisible", (void*)ccSetVisible);
	gst_define_cfunc("DECanvas.getOrder", (void*)ccGetOrder);
	gst_define_cfunc("DECanvas.setOrder", (void*)ccSetOrder);
	gst_define_cfunc("DECanvas.getTransparency", (void*)ccGetTransparency);
	gst_define_cfunc("DECanvas.setTransparency", (void*)ccSetTransparency);
	
	gst_define_cfunc("DECanvas.getImage", (void*)ccGetImage);
	gst_define_cfunc("DECanvas.setImage", (void*)ccSetImage);
	gst_define_cfunc("DECanvas.getRepeatX", (void*)ccGetRepeatX);
	gst_define_cfunc("DECanvas.setRepeatX", (void*)ccSetRepeatX);
	gst_define_cfunc("DECanvas.getRepeatY", (void*)ccGetRepeatY);
	gst_define_cfunc("DECanvas.setRepeatY", (void*)ccSetRepeatY);
	
	gst_define_cfunc("DECanvas.getShapeType", (void*)ccGetShapeType);
	gst_define_cfunc("DECanvas.setShapeType", (void*)ccSetShapeType);
	gst_define_cfunc("DECanvas.getLineColor", (void*)ccGetLineColor);
	gst_define_cfunc("DECanvas.setLineColor", (void*)ccSetLineColor);
	gst_define_cfunc("DECanvas.getFillColor", (void*)ccGetFillColor);
	gst_define_cfunc("DECanvas.setFillColor", (void*)ccSetFillColor);
	gst_define_cfunc("DECanvas.getThickness", (void*)ccGetThickness);
	gst_define_cfunc("DECanvas.setThickness", (void*)ccSetThickness);
	gst_define_cfunc("DECanvas.getPointCount", (void*)ccGetPointCount);
	gst_define_cfunc("DECanvas.getPointAt", (void*)ccGetPointAt);
	gst_define_cfunc("DECanvas.addPoint", (void*)ccAddPoint);
	gst_define_cfunc("DECanvas.removeAllPoints", (void*)ccRemoveAllPoints);
	
	gst_define_cfunc("DECanvas.getCanvasView", (void*)ccGetCanvasView);
	gst_define_cfunc("DECanvas.setCanvasView", (void*)ccSetCanvasView);
	
	gst_define_cfunc("DECanvas.getCamera", (void*)ccGetCamera);
	gst_define_cfunc("DECanvas.setCamera", (void*)ccSetCamera);
	
	gst_define_cfunc("DECanvas.getFont", (void*)ccGetFont);
	gst_define_cfunc("DECanvas.setFont", (void*)ccSetFont);
	gst_define_cfunc("DECanvas.getText", (void*)ccGetText);
	gst_define_cfunc("DECanvas.setText", (void*)ccSetText);
	gst_define_cfunc("DECanvas.getColor", (void*)ccGetColor);
	gst_define_cfunc("DECanvas.setColor", (void*)ccSetColor);
	
	gst_define_cfunc("DECanvas.getVideoPlayer", (void*)ccGetVideoPlayer);
	gst_define_cfunc("DECanvas.setVideoPlayer", (void*)ccSetVideoPlayer);
	
	gst_define_cfunc("DECanvas.getCanvasCount", (void*)ccGetCanvasCount);
	gst_define_cfunc("DECanvas.addCanvas", (void*)ccAddCanvas);
	gst_define_cfunc("DECanvas.removeCanvas", (void*)ccRemoveCanvas);
	gst_define_cfunc("DECanvas.removeAllCanvas", (void*)ccRemoveAllCanvas);
}

stClassCanvas::~stClassCanvas(){
}



// Management
///////////////

void stClassCanvas::SetUpLinks(){
	pOOPClass = gst_class_name_to_oop("DECanvas");
	if(!pOOPClass){
		DETHROW(deeInvalidParam); // Canvas.st missing !
	}
	
	csCanvasClass &csclass = *((csCanvasClass*)OOP_TO_OBJ(pOOPClass));
	csclass.scripting = pST.GetClassScripting()->GetSingleton();
}

deCanvas *stClassCanvas::OOPToCanvas(OOP object) const{
	if(object == pST.GetNil()){
		return NULL;
	}
	
	if(gst_get_object_class(object) != pOOPClass){
		DETHROW(deeInvalidAction);
	}
	return ((csCanvas*)OOP_TO_OBJ(object))->canvas;
}

OOP stClassCanvas::CanvasToOOP(deCanvas *canvas){
	if(!canvas){
		return pST.GetNil();
	}
	
	OOP oopCanvas = NULL;
	
	deCanvasVisitorIdentify identify;
	canvas->Visit(identify);
	if(identify.IsUnknown()){
		DETHROW(deeInvalidParam);
	}
	
	oopCanvas = pST.CreateNewObjectWithInit(pOOPClass, sizeof(csCanvas) - sizeof(csObject));
	if(!oopCanvas || oopCanvas == pST.GetNil()){
		DETHROW(deeOutOfMemory);
	}
	
	csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(oopCanvas));
	cscanvas.clsCanvas = this;
	cscanvas.canvas = canvas;
	cscanvas.canvas->AddReference();
	cscanvas.type = identify.GetType();
	
	return oopCanvas;
}



// cCall Methods
//////////////////

OOP stClassCanvas::ccNew(OOP self, OOP type){
	csCanvasClass &csclass = *((csCanvasClass*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP(csclass.scripting);
	const stClassCanvas &clsCanvas = *st.GetClassCanvas();
	deCanvas *canvas = NULL;
	OOP result = NULL;
	
	try{
		if(!IS_OOP(type)){
			DETHROW(deeInvalidParam);
		}
		
		if(OOP_TO_OBJ(type) == OOP_TO_OBJ(clsCanvas.pSymbolPaint)){
			canvas = st.GetGameEngine()->GetCanvasManager()->CreateCanvasPaint();
			
		}else if(OOP_TO_OBJ(type) == OOP_TO_OBJ(clsCanvas.pSymbolImage)){
			canvas = st.GetGameEngine()->GetCanvasManager()->CreateCanvasImage();
			
		}else if(OOP_TO_OBJ(type) == OOP_TO_OBJ(clsCanvas.pSymbolText)){
			canvas = st.GetGameEngine()->GetCanvasManager()->CreateCanvasText();
			
		}else if(OOP_TO_OBJ(type) == OOP_TO_OBJ(clsCanvas.pSymbolCanvasView)){
			canvas = st.GetGameEngine()->GetCanvasManager()->CreateCanvasCanvasView();
			
		}else if(OOP_TO_OBJ(type) == OOP_TO_OBJ(clsCanvas.pSymbolRenderWorld)){
			canvas = st.GetGameEngine()->GetCanvasManager()->CreateCanvasRenderWorld();
			
		}else if(OOP_TO_OBJ(type) == OOP_TO_OBJ(clsCanvas.pSymbolVideoPlayer)){
			canvas = st.GetGameEngine()->GetCanvasManager()->CreateCanvasVideoPlayer();
			
		}else if(OOP_TO_OBJ(type) == OOP_TO_OBJ(clsCanvas.pSymbolView)){
			canvas = st.GetGameEngine()->GetCanvasManager()->CreateCanvasView();
			
		}else{
			gst_eval_code("SystemExceptions.InvalidArgument new signal");
		}
		
		result = st.GetClassCanvas()->CanvasToOOP(canvas);
		gst_perform(result, st.GetSelectorInitialize());
		
		printf("new canvas %p\n", canvas);
		canvas->FreeReference();
		
	}catch(const deException &e){
		if(canvas){
			canvas->FreeReference();
		}
		result = st.GetNil();
		e.PrintError();
	}
	
	return result;
}

void stClassCanvas::ccFinalize(OOP self){
	csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	
	//printf( "finalize canvas %p\n", cscanvas.canvas );
	if(cscanvas.canvas){
		cscanvas.canvas->FreeReference();
		cscanvas.canvas = NULL;
		cscanvas.type = deCanvasVisitorIdentify::ectUnknown;
	}
}




OOP stClassCanvas::ccGetType(OOP self){
	const csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	
	switch(cscanvas.type){
	case deCanvasVisitorIdentify::ectImage:
		return cscanvas.clsCanvas->pSymbolImage;
		
	case deCanvasVisitorIdentify::ectPaint:
		return cscanvas.clsCanvas->pSymbolPaint;
		
	case deCanvasVisitorIdentify::ectCanvasView:
		return cscanvas.clsCanvas->pSymbolCanvasView;
		
	case deCanvasVisitorIdentify::ectRenderWorld:
		return cscanvas.clsCanvas->pSymbolRenderWorld;
		
	case deCanvasVisitorIdentify::ectText:
		return cscanvas.clsCanvas->pSymbolText;
		
	case deCanvasVisitorIdentify::ectVideoPlayer:
		return cscanvas.clsCanvas->pSymbolVideoPlayer;
		
	case deCanvasVisitorIdentify::ectView:
		return cscanvas.clsCanvas->pSymbolView;
		
	default:
		return cscanvas.clsCanvas->GetST().GetNil();
	}
}

OOP stClassCanvas::ccGetPosition(OOP self){
	const csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = cscanvas.clsCanvas->GetST();
	
	try{
		return st.GetClassPoint()->PointToOOP(cscanvas.canvas->GetPosition());
		
	}catch(const deException &e){
		e.PrintError();
		return st.GetNil();
	}
}

void stClassCanvas::ccSetPosition(OOP self, OOP position){
	csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = cscanvas.clsCanvas->GetST();
	
	try{
		const decPoint &rposition = st.GetClassPoint()->OOPToPoint(position);
		cscanvas.canvas->SetPosition(rposition);
		
	}catch(const deException &e){
		e.PrintError();
	}
}

OOP stClassCanvas::ccGetSize(OOP self){
	const csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = cscanvas.clsCanvas->GetST();
	
	try{
		return st.GetClassPoint()->PointToOOP(cscanvas.canvas->GetSize());
		
	}catch(const deException &e){
		e.PrintError();
		return st.GetNil();
	}
}

void stClassCanvas::ccSetSize(OOP self, OOP size){
	csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = cscanvas.clsCanvas->GetST();
	
	try{
		const decPoint &rsize = st.GetClassPoint()->OOPToPoint(size);
		cscanvas.canvas->SetSize(rsize);
		
	}catch(const deException &e){
		e.PrintError();
	}
}

OOP stClassCanvas::ccGetTransform(OOP self){
	return ((csCanvas*)OOP_TO_OBJ(self))->clsCanvas->GetST().GetNil();
}

void stClassCanvas::ccSetTransform(OOP self, OOP transform){
}

bool stClassCanvas::ccGetVisible(OOP self){
	const csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	return cscanvas.canvas->GetVisible();
}

void stClassCanvas::ccSetVisible(OOP self, bool visible){
	csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	cscanvas.canvas->SetVisible(visible);
}

float stClassCanvas::ccGetOrder(OOP self){
	const csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	return cscanvas.canvas->GetOrder();
}

void stClassCanvas::ccSetOrder(OOP self, float order){
	csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	cscanvas.canvas->SetOrder(order);
}

float stClassCanvas::ccGetTransparency(OOP self){
	const csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	return cscanvas.canvas->GetTransparency();
}

void stClassCanvas::ccSetTransparency(OOP self, float transparency){
	csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	cscanvas.canvas->SetTransparency(transparency);
}



OOP stClassCanvas::ccGetImage(OOP self){
	return ((csCanvas*)OOP_TO_OBJ(self))->clsCanvas->GetST().GetNil();
}

void stClassCanvas::ccSetImage(OOP self, OOP image){
}

int stClassCanvas::ccGetRepeatX(OOP self){
	return 1;
}

void stClassCanvas::ccSetRepeatX(OOP self, int count){
}

int stClassCanvas::ccGetRepeatY(OOP self){
	return 1;
}

void stClassCanvas::ccSetRepeatY(OOP self, int count){
}



OOP stClassCanvas::ccGetShapeType(OOP self){
	const csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	
	switch(cscanvas.type){
	case deCanvasVisitorIdentify::ectPaint:{
		const deCanvasPaint &canvasPaint = *((deCanvasPaint*)cscanvas.canvas);
		switch(canvasPaint.GetShapeType()){
		case deCanvasPaint::estPoints:
			return cscanvas.clsCanvas->pSymbolPoints;
			
		case deCanvasPaint::estLines:
			return cscanvas.clsCanvas->pSymbolLines;
			
		case deCanvasPaint::estRectangle:
			return cscanvas.clsCanvas->pSymbolRectangle;
			
		case deCanvasPaint::estEllipse:
			return cscanvas.clsCanvas->pSymbolEllipse;
			
		case deCanvasPaint::estPie:
			return cscanvas.clsCanvas->pSymbolPie;
			
		case deCanvasPaint::estPolygon:
			return cscanvas.clsCanvas->pSymbolPolygon;
		}
		}
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void stClassCanvas::ccSetShapeType(OOP self, OOP shapeType){
	const csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	
	switch(cscanvas.type){
	case deCanvasVisitorIdentify::ectPaint:{
		deCanvasPaint &canvasPaint = *((deCanvasPaint*)cscanvas.canvas);
		
		if(!IS_OOP(shapeType)){
			DETHROW(deeInvalidParam);
		}
		
		if(OOP_TO_OBJ(shapeType) == OOP_TO_OBJ(cscanvas.clsCanvas->pSymbolPoints)){
			canvasPaint.SetShapeType(deCanvasPaint::estPoints);
			
		}else if(OOP_TO_OBJ(shapeType) == OOP_TO_OBJ(cscanvas.clsCanvas->pSymbolLines)){
			canvasPaint.SetShapeType(deCanvasPaint::estLines);
			
		}else if(OOP_TO_OBJ(shapeType) == OOP_TO_OBJ(cscanvas.clsCanvas->pSymbolRectangle)){
			canvasPaint.SetShapeType(deCanvasPaint::estRectangle);
			
		}else if(OOP_TO_OBJ(shapeType) == OOP_TO_OBJ(cscanvas.clsCanvas->pSymbolEllipse)){
			canvasPaint.SetShapeType(deCanvasPaint::estEllipse);
			
		}else if(OOP_TO_OBJ(shapeType) == OOP_TO_OBJ(cscanvas.clsCanvas->pSymbolPie)){
			canvasPaint.SetShapeType(deCanvasPaint::estPie);
			
		}else if(OOP_TO_OBJ(shapeType) == OOP_TO_OBJ(cscanvas.clsCanvas->pSymbolPolygon)){
			canvasPaint.SetShapeType(deCanvasPaint::estPolygon);
			
		}else{
			DETHROW(deeInvalidParam);
		}
		}break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

OOP stClassCanvas::ccGetLineColor(OOP self){
	const csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = cscanvas.clsCanvas->GetST();
	
	switch(cscanvas.type){
	case deCanvasVisitorIdentify::ectPaint:{
		const deCanvasPaint &canvasPaint = *((deCanvasPaint*)cscanvas.canvas);
		return st.GetClassColor()->ColorToOOP(canvasPaint.GetLineColor());
		}
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void stClassCanvas::ccSetLineColor(OOP self, OOP color){
	csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = cscanvas.clsCanvas->GetST();
	
	switch(cscanvas.type){
	case deCanvasVisitorIdentify::ectPaint:{
		deCanvasPaint &canvasPaint = *((deCanvasPaint*)cscanvas.canvas);
		const decColor &rcolor = st.GetClassColor()->OOPToColor(color);
		canvasPaint.SetLineColor(rcolor);
		}break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

OOP stClassCanvas::ccGetFillColor(OOP self){
	const csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = cscanvas.clsCanvas->GetST();
	
	switch(cscanvas.type){
	case deCanvasVisitorIdentify::ectPaint:{
		const deCanvasPaint &canvasPaint = *((deCanvasPaint*)cscanvas.canvas);
		return st.GetClassColor()->ColorToOOP(canvasPaint.GetFillColor());
		}
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void stClassCanvas::ccSetFillColor(OOP self, OOP color){
	csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = cscanvas.clsCanvas->GetST();
	
	switch(cscanvas.type){
	case deCanvasVisitorIdentify::ectPaint:{
		deCanvasPaint &canvasPaint = *((deCanvasPaint*)cscanvas.canvas);
		const decColor &rcolor = st.GetClassColor()->OOPToColor(color);
		canvasPaint.SetFillColor(rcolor);
		}break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

float stClassCanvas::ccGetThickness(OOP self){
	const csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	
	switch(cscanvas.type){
	case deCanvasVisitorIdentify::ectPaint:{
		const deCanvasPaint &canvasPaint = *((deCanvasPaint*)cscanvas.canvas);
		return canvasPaint.GetThickness();
		}
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void stClassCanvas::ccSetThickness(OOP self, float thickness){
	csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	
	switch(cscanvas.type){
	case deCanvasVisitorIdentify::ectPaint:{
		deCanvasPaint &canvasPaint = *((deCanvasPaint*)cscanvas.canvas);
		canvasPaint.SetThickness(thickness);
		}break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

int stClassCanvas::ccGetPointCount(OOP self){
	const csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	
	switch(cscanvas.type){
	case deCanvasVisitorIdentify::ectPaint:{
		const deCanvasPaint &canvasPaint = *((deCanvasPaint*)cscanvas.canvas);
		return canvasPaint.GetPointCount();
		}
		
	default:
		DETHROW(deeInvalidParam);
	}
}

OOP stClassCanvas::ccGetPointAt(OOP self, int index){
	return ((csCanvas*)OOP_TO_OBJ(self))->clsCanvas->GetST().GetNil();
}

void stClassCanvas::ccAddPoint(OOP self, OOP point){
}

void stClassCanvas::ccRemoveAllPoints(OOP self){
}



OOP stClassCanvas::ccGetCanvasView(OOP self){
	return ((csCanvas*)OOP_TO_OBJ(self))->clsCanvas->GetST().GetNil();
}

void stClassCanvas::ccSetCanvasView(OOP self, OOP canvasView){
}



OOP stClassCanvas::ccGetCamera(OOP self){
	return ((csCanvas*)OOP_TO_OBJ(self))->clsCanvas->GetST().GetNil();
}

void stClassCanvas::ccSetCamera(OOP self, OOP camera){
}



OOP stClassCanvas::ccGetFont(OOP self){
	return ((csCanvas*)OOP_TO_OBJ(self))->clsCanvas->GetST().GetNil();
}

void stClassCanvas::ccSetFont(OOP self, OOP font){
}

OOP stClassCanvas::ccGetText(OOP self){
	return ((csCanvas*)OOP_TO_OBJ(self))->clsCanvas->GetST().GetNil();
}

void stClassCanvas::ccSetText(OOP self, OOP text){
}

OOP stClassCanvas::ccGetColor(OOP self){
	return ((csCanvas*)OOP_TO_OBJ(self))->clsCanvas->GetST().GetNil();
}

void stClassCanvas::ccSetColor(OOP self, OOP color){
}



OOP stClassCanvas::ccGetVideoPlayer(OOP self){
	return ((csCanvas*)OOP_TO_OBJ(self))->clsCanvas->GetST().GetNil();
}

void stClassCanvas::ccSetVideoPlayer(OOP self, OOP videoPlayer){
}



int stClassCanvas::ccGetCanvasCount(OOP self){
	csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	
	switch(cscanvas.type){
	case deCanvasVisitorIdentify::ectView:{
		deCanvasView &canvasView = *((deCanvasView*)cscanvas.canvas);
		return canvasView.GetCanvasCount();
		}break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void stClassCanvas::ccAddCanvas(OOP self, OOP canvas){
	csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	
	switch(cscanvas.type){
	case deCanvasVisitorIdentify::ectView:{
		deCanvasView &canvasView = *((deCanvasView*)cscanvas.canvas);
		
		deCanvas * const rcanvas = cscanvas.clsCanvas->OOPToCanvas(canvas);
		if(!canvas){
			DETHROW(deeNullPointer);
		}
		
		canvasView.AddCanvas(rcanvas);
		}break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void stClassCanvas::ccRemoveCanvas(OOP self, OOP canvas){
	csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	
	switch(cscanvas.type){
	case deCanvasVisitorIdentify::ectView:{
		deCanvasView &canvasView = *((deCanvasView*)cscanvas.canvas);
		
		deCanvas * const rcanvas = cscanvas.clsCanvas->OOPToCanvas(canvas);
		if(!canvas){
			DETHROW(deeNullPointer);
		}
		
		canvasView.RemoveCanvas(rcanvas);
		}break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void stClassCanvas::ccRemoveAllCanvas(OOP self){
	csCanvas &cscanvas = *((csCanvas*)OOP_TO_OBJ(self));
	
	switch(cscanvas.type){
	case deCanvasVisitorIdentify::ectView:{
		deCanvasView &canvasView = *((deCanvasView*)cscanvas.canvas);
		canvasView.RemoveAllCanvas();
		}break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}
