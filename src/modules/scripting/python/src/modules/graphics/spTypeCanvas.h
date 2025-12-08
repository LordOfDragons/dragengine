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

#ifndef _SPTYPECANVAS_H_
#define _SPTYPECANVAS_H_

#include "../spBaseType.h"

class deCanvas;



/**
 * \brief Canvas type.
 */
class spTypeCanvas : public spBaseType{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create type. */
	spTypeCanvas(ScriptingPython &sp);
	
	/** \brief Clean up module. */
	~spTypeCanvas() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Canvas from object or \NULL if object is \em None. */
	deCanvas *CanvasFromObject(PyObject *object);
	
	/** \brief Create object from a graphics or \em None if object is \em NULL. */
	PyObject *ObjectFromCanvas(deCanvas::Ref canvas);
	
	
	
	/** \brief Constructor. */
	virtual void Constructor(PyObject *myself, PyObject *args, PyObject *kwds);
	
	/** \brief Destructor. */
	virtual void Destructor(PyObject *myself);
	/*@}*/
	
	
	
	/** \name Functions */
	/*@{*/
	static PyObject *cfGetType(PyObject *myself);
	static PyObject *cfGetPosition(PyObject *myself);
	static PyObject *cfSetPosition(PyObject *myself, PyObject *args);
	static PyObject *cfGetSize(PyObject *myself);
	static PyObject *cfSetSize(PyObject *myself, PyObject *args);
	static PyObject *cfGetTransform(PyObject *myself);
	static PyObject *cfSetTransform(PyObject *myself, PyObject *args);
	static PyObject *cfGetVisible(PyObject *myself);
	static PyObject *cfSetVisible(PyObject *myself, PyObject *args);
	static PyObject *cfGetOrder(PyObject *myself);
	static PyObject *cfSetOrder(PyObject *myself, PyObject *args);
	static PyObject *cfGetTransparency(PyObject *myself);
	static PyObject *cfSetTransparency(PyObject *myself, PyObject *args);
	
	static PyObject *cfGetImage(PyObject *myself);
	static PyObject *cfSetImage(PyObject *myself, PyObject *args);
	static PyObject *cfGetRepeatX(PyObject *myself);
	static PyObject *cfSetRepeatX(PyObject *myself, PyObject *args);
	static PyObject *cfGetRepeatY(PyObject *myself);
	static PyObject *cfSetRepeatY(PyObject *myself, PyObject *args);
	
	static PyObject *cfGetShapeType(PyObject *myself);
	static PyObject *cfSetShapeType(PyObject *myself, PyObject *args);
	static PyObject *cfGetLineColor(PyObject *myself);
	static PyObject *cfSetLineColor(PyObject *myself, PyObject *args);
	static PyObject *cfGetFillColor(PyObject *myself);
	static PyObject *cfSetFillColor(PyObject *myself, PyObject *args);
	static PyObject *cfGetThickness(PyObject *myself);
	static PyObject *cfSetThickness(PyObject *myself, PyObject *args);
	static PyObject *cfGetPointCount(PyObject *myself);
	static PyObject *cfGetPointAt(PyObject *myself, PyObject *args);
	static PyObject *cfAddPoint(PyObject *myself, PyObject *args);
	static PyObject *cfRemoveAllPoints(PyObject *myself);
	
	static PyObject *cfGetCanvasView(PyObject *myself);
	static PyObject *cfSetCanvasView(PyObject *myself, PyObject *args);
	
	static PyObject *cfGetCamera(PyObject *myself);
	static PyObject *cfSetCamera(PyObject *myself, PyObject *args);
	
	static PyObject *cfGetFont(PyObject *myself);
	static PyObject *cfSetFont(PyObject *myself, PyObject *args);
	static PyObject *cfGetText(PyObject *myself);
	static PyObject *cfSetText(PyObject *myself, PyObject *args);
	static PyObject *cfGetColor(PyObject *myself);
	static PyObject *cfSetColor(PyObject *myself, PyObject *args);
	
	static PyObject *cfGetVideoPlayer(PyObject *myself);
	static PyObject *cfSetVideoPlayer(PyObject *myself, PyObject *args);
	
	static PyObject *cfGetCanvasCount(PyObject *myself, PyObject *args);
	static PyObject *cfAddCanvas(PyObject *myself, PyObject *args);
	static PyObject *cfRemoveCanvas(PyObject *myself, PyObject *args);
	static PyObject *cfRemoveAllCanvas(PyObject *myself);
	/*@}*/
};

#endif
