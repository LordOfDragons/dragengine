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

#include "spTypeCanvas.h"
#include "../spBaseModule.h"
#include "../../ScriptingPython.h"

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



// Python Binding
///////////////////

struct sObjectData{
	deCanvas *canvas;
	deCanvasVisitorIdentify::eCanvasTypes type;
};



// Class spTypeCanvas
/////////////////////////

// Constructor, destructor
////////////////////////////

spTypeCanvas::spTypeCanvas(ScriptingPython &sp) : spBaseType(sp, "Canvas"){
	SetDocumentation("Canvas resource");
	SetNativeDataSize(sizeof(sObjectData));
	SetParent(sp.GetNamespaceScenery());
	
	AddMethod("getType", (PyCFunction)cfGetType, METH_NOARGS, "");
	AddMethod("getPosition", (PyCFunction)cfGetPosition, METH_NOARGS, "");
	AddMethod("setPosition", (PyCFunction)cfSetPosition, METH_VARARGS, "");
	AddMethod("getSize", (PyCFunction)cfGetSize, METH_NOARGS, "");
	AddMethod("setSize", (PyCFunction)cfSetSize, METH_VARARGS, "");
	AddMethod("getTransform", (PyCFunction)cfGetTransform, METH_NOARGS, "");
	AddMethod("setTransform", (PyCFunction)cfSetTransform, METH_VARARGS, "");
	AddMethod("getVisible", (PyCFunction)cfGetVisible, METH_NOARGS, "");
	AddMethod("setVisible", (PyCFunction)cfSetVisible, METH_VARARGS, "");
	AddMethod("getOrder", (PyCFunction)cfGetOrder, METH_NOARGS, "");
	AddMethod("setOrder", (PyCFunction)cfSetOrder, METH_VARARGS, "");
	AddMethod("getTransparency", (PyCFunction)cfGetTransparency, METH_NOARGS, "");
	AddMethod("setTransparency", (PyCFunction)cfSetTransparency, METH_VARARGS, "");
	
	AddMethod("getImage", (PyCFunction)cfGetImage, METH_NOARGS, "");
	AddMethod("setImage", (PyCFunction)cfSetImage, METH_VARARGS, "");
	AddMethod("getRepeatX", (PyCFunction)cfGetRepeatX, METH_NOARGS, "");
	AddMethod("setRepeatX", (PyCFunction)cfSetRepeatX, METH_VARARGS, "");
	AddMethod("getRepeatY", (PyCFunction)cfGetRepeatY, METH_NOARGS, "");
	AddMethod("setRepeatY", (PyCFunction)cfSetRepeatY, METH_VARARGS, "");
	
	AddMethod("getShapeType", (PyCFunction)cfGetShapeType, METH_NOARGS, "");
	AddMethod("setShapeType", (PyCFunction)cfSetShapeType, METH_VARARGS, "");
	AddMethod("getLineColor", (PyCFunction)cfGetLineColor, METH_NOARGS, "");
	AddMethod("setLineColor", (PyCFunction)cfSetLineColor, METH_VARARGS, "");
	AddMethod("getFillColor", (PyCFunction)cfGetFillColor, METH_NOARGS, "");
	AddMethod("setFillColor", (PyCFunction)cfSetFillColor, METH_VARARGS, "");
	AddMethod("getThickness", (PyCFunction)cfGetThickness, METH_NOARGS, "");
	AddMethod("setThickness", (PyCFunction)cfSetThickness, METH_VARARGS, "");
	AddMethod("getPointCount", (PyCFunction)cfGetPointCount, METH_NOARGS, "");
	AddMethod("getPointAt", (PyCFunction)cfGetPointAt, METH_VARARGS, "");
	AddMethod("addPoint", (PyCFunction)cfAddPoint, METH_VARARGS, "");
	AddMethod("removeAllPoints", (PyCFunction)cfRemoveAllPoints, METH_NOARGS, "");
	
	AddMethod("getCanvasView", (PyCFunction)cfGetCanvasView, METH_NOARGS, "");
	AddMethod("setCanvasView", (PyCFunction)cfSetCanvasView, METH_VARARGS, "");
	
	AddMethod("getCamera", (PyCFunction)cfGetCamera, METH_NOARGS, "");
	AddMethod("setCamera", (PyCFunction)cfSetCamera, METH_VARARGS, "");
	
	AddMethod("getFont", (PyCFunction)cfGetFont, METH_NOARGS, "");
	AddMethod("setFont", (PyCFunction)cfSetFont, METH_VARARGS, "");
	AddMethod("getText", (PyCFunction)cfGetText, METH_NOARGS, "");
	AddMethod("setText", (PyCFunction)cfSetText, METH_VARARGS, "");
	AddMethod("getColor", (PyCFunction)cfGetColor, METH_NOARGS, "");
	AddMethod("setColor", (PyCFunction)cfSetColor, METH_VARARGS, "");
	
	AddMethod("getVideoPlayer", (PyCFunction)cfGetVideoPlayer, METH_NOARGS, "");
	AddMethod("setVideoPlayer", (PyCFunction)cfSetVideoPlayer, METH_VARARGS, "");
	
	AddMethod("getCanvasCount", (PyCFunction)cfGetCanvasCount, METH_VARARGS, "");
	AddMethod("addCanvas", (PyCFunction)cfAddCanvas, METH_VARARGS, "");
	AddMethod("removeCanvas", (PyCFunction)cfRemoveCanvas, METH_VARARGS, "");
	AddMethod("removeAllCanvas", (PyCFunction)cfRemoveAllCanvas, METH_NOARGS, "");
}

spTypeCanvas::~spTypeCanvas(){
}



// Management
///////////////

deCanvas *spTypeCanvas::CanvasFromObject(PyObject *object){
	if(!object){
		return NULL;
	}
	
	return ((sObjectData*)GetObjectData(object))->canvas;
}

PyObject *spTypeCanvas::ObjectFromCanvas(deCanvas *canvas){
	if(!canvas){
		Py_INCREF(Py_None);
		return Py_None;
	}
	
	PyObject * const pyoGraphics = CreatePyObject();
	deCanvasVisitorIdentify identify;
	canvas->Visit(identify);
	if(identify.IsUnknown()){
		PyErr_BadArgument();
		return NULL;
	}
	
	sObjectData &od = *((sObjectData*)GetObjectData(pyoGraphics));
	od.canvas = canvas;
	canvas->AddReference();
	od.type = identify.GetType();
	
	return pyoGraphics;
}



void spTypeCanvas::Constructor(PyObject *myself, PyObject *args, PyObject *kwds){
	sObjectData &od = *((sObjectData*)GetObjectData(myself));
	
	od.canvas = NULL;
	
	int type;
	if(!PyArg_ParseTuple(args, "i", &type)){
		DETHROW(deeInvalidParam);
	}
	
	switch((deCanvasVisitorIdentify::eCanvasTypes)type){
	case deCanvasVisitorIdentify::ectPaint:
		od.canvas = GetSP().GetGameEngine()->GetCanvasManager()->CreateCanvasPaint();
		break;
		
	case deCanvasVisitorIdentify::ectImage:
		od.canvas = GetSP().GetGameEngine()->GetCanvasManager()->CreateCanvasImage();
		break;
		
	case deCanvasVisitorIdentify::ectText:
		od.canvas = GetSP().GetGameEngine()->GetCanvasManager()->CreateCanvasText();
		break;
		
	case deCanvasVisitorIdentify::ectCanvasView:
		od.canvas = GetSP().GetGameEngine()->GetCanvasManager()->CreateCanvasCanvasView();
		break;
		
	case deCanvasVisitorIdentify::ectRenderWorld:
		od.canvas = GetSP().GetGameEngine()->GetCanvasManager()->CreateCanvasRenderWorld();
		break;
		
	case deCanvasVisitorIdentify::ectVideoPlayer:
		od.canvas = GetSP().GetGameEngine()->GetCanvasManager()->CreateCanvasVideoPlayer();
		break;
		
	case deCanvasVisitorIdentify::ectView:
		od.canvas = GetSP().GetGameEngine()->GetCanvasManager()->CreateCanvasView();
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
	
	od.type = (deCanvasVisitorIdentify::eCanvasTypes)type;
}

void spTypeCanvas::Destructor(PyObject *myself){
	sObjectData &od = *((sObjectData*)GetObjectData(myself));
	
	if(od.canvas){
		od.canvas->FreeReference();
		od.canvas = NULL;
		od.type = deCanvasVisitorIdentify::ectUnknown;
	}
}



// Functions
//////////////

// int getType()
PyObject *spTypeCanvas::cfGetType(PyObject *myself){
	const sObjectData od = *((sObjectData*)GetObjectData(myself));
	return Py_BuildValue("i", od.type);
}

// Point getPosition()
PyObject *spTypeCanvas::cfGetPosition(PyObject *myself){
	const deCanvas &canvas = *(((sObjectData*)GetObjectData(myself))->canvas);
	return Py_BuildValue("ii", canvas.GetPosition().x, canvas.GetPosition().y);
}

// void setPosition( Point position )
PyObject *spTypeCanvas::cfSetPosition(PyObject *myself, PyObject *args){
	deCanvas &canvas = *(((sObjectData*)GetObjectData(myself))->canvas);
	
	decPoint position;
	if(!PyArg_ParseTuple(args, "ii", &position.x, &position.y)){
		return NULL;
	}
	
	canvas.SetPosition(position);
	
	Py_INCREF(Py_None);
	return Py_None;
}

// Point getSize()
PyObject *spTypeCanvas::cfGetSize(PyObject *myself){
	const deCanvas &canvas = *(((sObjectData*)GetObjectData(myself))->canvas);
	return Py_BuildValue("ii", canvas.GetSize().x, canvas.GetSize().y);
}

// void setSize( Point )
PyObject *spTypeCanvas::cfSetSize(PyObject *myself, PyObject *args){
	deCanvas &canvas = *(((sObjectData*)GetObjectData(myself))->canvas);
	
	decPoint size;
	if(!PyArg_ParseTuple(args, "ii", &size.x, &size.y)){
		return NULL;
	}
	
	canvas.SetSize(size);
	
	Py_INCREF(Py_None);
	return Py_None;
}

// TexMatrix2 getTransform()
PyObject *spTypeCanvas::cfGetTransform(PyObject *myself){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void setTransform( TexMatrix2 matrix )
PyObject *spTypeCanvas::cfSetTransform(PyObject *myself, PyObject *args){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// bool getVisible()
PyObject *spTypeCanvas::cfGetVisible(PyObject *myself){
	const deCanvas &canvas = *(((sObjectData*)GetObjectData(myself))->canvas);
	return Py_BuildValue("b", canvas.GetVisible());
}

// void setVisible( bool )
PyObject *spTypeCanvas::cfSetVisible(PyObject *myself, PyObject *args){
	deCanvas &canvas = *(((sObjectData*)GetObjectData(myself))->canvas);
	bool visible;
	if(!PyArg_ParseTuple(args, "b", &visible)){
		return NULL;
	}
	
	canvas.SetVisible(visible);
	
	Py_INCREF(Py_None);
	return Py_None;
}

// float getOrder()
PyObject *spTypeCanvas::cfGetOrder(PyObject *myself){
	const deCanvas &canvas = *(((sObjectData*)GetObjectData(myself))->canvas);
	return Py_BuildValue("f", canvas.GetOrder());
}

// void setOrder( float order )
PyObject *spTypeCanvas::cfSetOrder(PyObject *myself, PyObject *args){
	deCanvas &canvas = *(((sObjectData*)GetObjectData(myself))->canvas);
	float order;
	if(!PyArg_ParseTuple(args, "f", &order)){
		return NULL;
	}
	
	canvas.SetOrder(order);
	
	Py_INCREF(Py_None);
	return Py_None;
}

// float getTransparency()
PyObject *spTypeCanvas::cfGetTransparency(PyObject *myself){
	const deCanvas &canvas = *(((sObjectData*)GetObjectData(myself))->canvas);
	return Py_BuildValue("f", canvas.GetTransparency());
}

// void setTransparency( float transparency )
PyObject *spTypeCanvas::cfSetTransparency(PyObject *myself, PyObject *args){
	deCanvas &canvas = *(((sObjectData*)GetObjectData(myself))->canvas);
	float transparency;
	if(!PyArg_ParseTuple(args, "f", &transparency)){
		return NULL;
	}
	
	canvas.SetOrder(transparency);
	
	Py_INCREF(Py_None);
	return Py_None;
}



// Image getImage()
PyObject *spTypeCanvas::cfGetImage(PyObject *myself){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void setImage( Image image )
PyObject *spTypeCanvas::cfSetImage(PyObject *myself, PyObject *args){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// int getRepeatX()
PyObject *spTypeCanvas::cfGetRepeatX(PyObject *myself){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void setRepeatX( int repeat )
PyObject *spTypeCanvas::cfSetRepeatX(PyObject *myself, PyObject *args){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// int getRepeatY()
PyObject *spTypeCanvas::cfGetRepeatY(PyObject *myself){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void setRepeatY( int reepat )
PyObject *spTypeCanvas::cfSetRepeatY(PyObject *myself, PyObject *args){
	
	Py_INCREF(Py_None);
	return Py_None;
}



// int getShapeType()
PyObject *spTypeCanvas::cfGetShapeType(PyObject *myself){
	const sObjectData od = *((sObjectData*)GetObjectData(myself));
	
	switch(od.type){
	case deCanvasVisitorIdentify::ectPaint:{
		const deCanvasPaint &canvasPaint = *((deCanvasPaint*)od.canvas);
		return Py_BuildValue("i", canvasPaint.GetShapeType());
		}
		
	default:
		PyErr_BadArgument();
		return NULL;
	}
}

// void setShapeType( int shapeType )
PyObject *spTypeCanvas::cfSetShapeType(PyObject *myself, PyObject *args){
	const sObjectData od = *((sObjectData*)GetObjectData(myself));
	
	switch(od.type){
	case deCanvasVisitorIdentify::ectPaint:{
		deCanvasPaint &canvasPaint = *((deCanvasPaint*)od.canvas);
		
		int shapeType;
		if(!PyArg_ParseTuple(args, "i", &shapeType)){
			return NULL;
		}
		
		canvasPaint.SetShapeType((deCanvasPaint::eShapeTypes)shapeType);
		}break;
		
	default:
		PyErr_BadArgument();
		return NULL;
	}
	
	Py_INCREF(Py_None);
	return Py_None;
}

// Color getLineColor()
PyObject *spTypeCanvas::cfGetLineColor(PyObject *myself){
	const sObjectData od = *((sObjectData*)GetObjectData(myself));
	
	switch(od.type){
	case deCanvasVisitorIdentify::ectPaint:{
		const deCanvasPaint &canvasPaint = *((deCanvasPaint*)od.canvas);
		return Py_BuildValue("ffff", canvasPaint.GetLineColor().r, canvasPaint.GetLineColor().b,
			canvasPaint.GetLineColor().b, canvasPaint.GetLineColor().a);
		}
		
	default:
		PyErr_BadArgument();
		return NULL;
	}
}

// void setLineColor( Color color )
PyObject *spTypeCanvas::cfSetLineColor(PyObject *myself, PyObject *args){
	const sObjectData od = *((sObjectData*)GetObjectData(myself));
	
	switch(od.type){
	case deCanvasVisitorIdentify::ectPaint:{
		deCanvasPaint &canvasPaint = *((deCanvasPaint*)od.canvas);
		
		decColor color;
		if(!PyArg_ParseTuple(args, "ffff", &color.r, &color.g, &color.b, &color.a)){
			return NULL;
		}
		
		canvasPaint.SetLineColor(color);
		}break;
		
	default:
		PyErr_BadArgument();
		return NULL;
	}
	
	Py_INCREF(Py_None);
	return Py_None;
}

// Color getFillColor()
PyObject *spTypeCanvas::cfGetFillColor(PyObject *myself){
	const sObjectData od = *((sObjectData*)GetObjectData(myself));
	
	switch(od.type){
	case deCanvasVisitorIdentify::ectPaint:{
		const deCanvasPaint &canvasPaint = *((deCanvasPaint*)od.canvas);
		return Py_BuildValue("ffff", canvasPaint.GetFillColor().r, canvasPaint.GetFillColor().b,
			canvasPaint.GetFillColor().b, canvasPaint.GetFillColor().a);
		}
		
	default:
		PyErr_BadArgument();
		return NULL;
	}
}

// void setFillColor( Color color )
PyObject *spTypeCanvas::cfSetFillColor(PyObject *myself, PyObject *args){
	const sObjectData od = *((sObjectData*)GetObjectData(myself));
	
	switch(od.type){
	case deCanvasVisitorIdentify::ectPaint:{
		deCanvasPaint &canvasPaint = *((deCanvasPaint*)od.canvas);
		
		decColor color;
		if(!PyArg_ParseTuple(args, "ffff", &color.r, &color.g, &color.b, &color.a)){
			return NULL;
		}
		
		canvasPaint.SetFillColor(color);
		}break;
		
	default:
		PyErr_BadArgument();
		return NULL;
	}
	
	Py_INCREF(Py_None);
	return Py_None;
}

// float getThickness()
PyObject *spTypeCanvas::cfGetThickness(PyObject *myself){
	const sObjectData od = *((sObjectData*)GetObjectData(myself));
	
	switch(od.type){
	case deCanvasVisitorIdentify::ectPaint:{
		const deCanvasPaint &canvasPaint = *((deCanvasPaint*)od.canvas);
		return Py_BuildValue("f", canvasPaint.GetThickness());
		}
		
	default:
		PyErr_BadArgument();
		return NULL;
	}
}

// void setThickness( float thickness )
PyObject *spTypeCanvas::cfSetThickness(PyObject *myself, PyObject *args){
	const sObjectData od = *((sObjectData*)GetObjectData(myself));
	
	switch(od.type){
	case deCanvasVisitorIdentify::ectPaint:{
		deCanvasPaint &canvasPaint = *((deCanvasPaint*)od.canvas);
		
		float thickness;
		if(!PyArg_ParseTuple(args, "f", &thickness)){
			return NULL;
		}
		
		canvasPaint.SetThickness(thickness);
		}break;
		
	default:
		PyErr_BadArgument();
		return NULL;
	}
	
	Py_INCREF(Py_None);
	return Py_None;
}

// int getPointCount()
PyObject *spTypeCanvas::cfGetPointCount(PyObject *myself){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// Point getPointAt( int index )
PyObject *spTypeCanvas::cfGetPointAt(PyObject *myself, PyObject *args){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void addPoint( Point point )
PyObject *spTypeCanvas::cfAddPoint(PyObject *myself, PyObject *args){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void removeAllPoints()
PyObject *spTypeCanvas::cfRemoveAllPoints(PyObject *myself){
	
	Py_INCREF(Py_None);
	return Py_None;
}



// Canvas getCanvasView()
PyObject *spTypeCanvas::cfGetCanvasView(PyObject *myself){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void setCanvasView( Canvas canvasView )
PyObject *spTypeCanvas::cfSetCanvasView(PyObject *myself, PyObject *args){
	
	Py_INCREF(Py_None);
	return Py_None;
}



// Camera getCamera()
PyObject *spTypeCanvas::cfGetCamera(PyObject *myself){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void setCamera( Camera camera )
PyObject *spTypeCanvas::cfSetCamera(PyObject *myself, PyObject *args){
	
	Py_INCREF(Py_None);
	return Py_None;
}



// Font getFont()
PyObject *spTypeCanvas::cfGetFont(PyObject *myself){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void setFont( Font font )
PyObject *spTypeCanvas::cfSetFont(PyObject *myself, PyObject *args){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// string getText()
PyObject *spTypeCanvas::cfGetText(PyObject *myself){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void setText( string text )
PyObject *spTypeCanvas::cfSetText(PyObject *myself, PyObject *args){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// Color getColor()
PyObject *spTypeCanvas::cfGetColor(PyObject *myself){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void setColor( Color color )
PyObject *spTypeCanvas::cfSetColor(PyObject *myself, PyObject *args){
	
	Py_INCREF(Py_None);
	return Py_None;
}



// VideoPlayer getVideoPlayer()
PyObject *spTypeCanvas::cfGetVideoPlayer(PyObject *myself){
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void setVideoPlayer( VideoPlayer videoPlayer )
PyObject *spTypeCanvas::cfSetVideoPlayer(PyObject *myself, PyObject *args){
	
	Py_INCREF(Py_None);
	return Py_None;
}



// int getCanvasCount()
PyObject *spTypeCanvas::cfGetCanvasCount(PyObject *myself, PyObject *args){
	const sObjectData od = *((sObjectData*)GetObjectData(myself));
	
	switch(od.type){
	case deCanvasVisitorIdentify::ectView:{
		deCanvasView &canvasView = *((deCanvasView*)od.canvas);
		return Py_BuildValue("i", canvasView.GetCanvasCount());
		}break;
		
	default:
		PyErr_BadArgument();
		return NULL;
	}
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void addCanvas( Canvas canvas )
PyObject *spTypeCanvas::cfAddCanvas(PyObject *myself, PyObject *args){
	spTypeCanvas &clsCanvas = *((spTypeCanvas*)GetOwnerClass(myself));
	const sObjectData od = *((sObjectData*)GetObjectData(myself));
	
	switch(od.type){
	case deCanvasVisitorIdentify::ectView:{
		deCanvasView &canvasView = *((deCanvasView*)od.canvas);
		
		PyObject *pyoCanvas = NULL;
		if(!PyArg_ParseTuple(args, "O", &pyoCanvas)){
			return NULL;
		}
		
		deCanvas * const canvas = clsCanvas.CanvasFromObject(pyoCanvas);
		if(!canvas){
			PyErr_BadArgument();
			return NULL;
		}
		
		canvasView.AddCanvas(canvas);
		}break;
		
	default:
		PyErr_BadArgument();
		return NULL;
	}
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void removeCanvas( Canvas canvas )
PyObject *spTypeCanvas::cfRemoveCanvas(PyObject *myself, PyObject *args){
	spTypeCanvas &clsCanvas = *((spTypeCanvas*)GetOwnerClass(myself));
	const sObjectData od = *((sObjectData*)GetObjectData(myself));
	
	switch(od.type){
	case deCanvasVisitorIdentify::ectView:{
		deCanvasView &canvasView = *((deCanvasView*)od.canvas);
		
		PyObject *pyoCanvas = NULL;
		if(!PyArg_ParseTuple(args, "O", &pyoCanvas)){
			return NULL;
		}
		
		deCanvas * const canvas = clsCanvas.CanvasFromObject(pyoCanvas);
		if(!canvas){
			PyErr_BadArgument();
			return NULL;
		}
		
		canvasView.RemoveCanvas(canvas);
		}break;
		
	default:
		PyErr_BadArgument();
		return NULL;
	}
	
	Py_INCREF(Py_None);
	return Py_None;
}

// void removeAllCanvas()
PyObject *spTypeCanvas::cfRemoveAllCanvas(PyObject *myself){
	const sObjectData od = *((sObjectData*)GetObjectData(myself));
	
	switch(od.type){
	case deCanvasVisitorIdentify::ectView:{
		deCanvasView &canvasView = *((deCanvasView*)od.canvas);
		canvasView.RemoveAllCanvas();
		}break;
		
	default:
		PyErr_BadArgument();
		return NULL;
	}
	
	Py_INCREF(Py_None);
	return Py_None;
}
