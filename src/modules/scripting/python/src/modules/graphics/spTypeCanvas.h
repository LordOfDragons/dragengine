/* 
 * Drag[en]gine Python Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	spTypeCanvas( ScriptingPython &sp );
	
	/** \brief Clean up module. */
	virtual ~spTypeCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Canvas from object or \NULL if object is \em None. */
	deCanvas *CanvasFromObject( PyObject *object );
	
	/** \brief Create object from a graphics or \em None if object is \em NULL. */
	PyObject *ObjectFromCanvas( deCanvas *canvas );
	
	
	
	/** \brief Constructor. */
	virtual void Constructor( PyObject *myself, PyObject *args, PyObject *kwds );
	
	/** \brief Destructor. */
	virtual void Destructor( PyObject *myself );
	/*@}*/
	
	
	
	/** \name Functions */
	/*@{*/
	static PyObject *cfGetType( PyObject *myself );
	static PyObject *cfGetPosition( PyObject *myself );
	static PyObject *cfSetPosition( PyObject *myself, PyObject *args );
	static PyObject *cfGetSize( PyObject *myself );
	static PyObject *cfSetSize( PyObject *myself, PyObject *args );
	static PyObject *cfGetTransform( PyObject *myself );
	static PyObject *cfSetTransform( PyObject *myself, PyObject *args );
	static PyObject *cfGetVisible( PyObject *myself );
	static PyObject *cfSetVisible( PyObject *myself, PyObject *args );
	static PyObject *cfGetOrder( PyObject *myself );
	static PyObject *cfSetOrder( PyObject *myself, PyObject *args );
	static PyObject *cfGetTransparency( PyObject *myself );
	static PyObject *cfSetTransparency( PyObject *myself, PyObject *args );
	
	static PyObject *cfGetImage( PyObject *myself );
	static PyObject *cfSetImage( PyObject *myself, PyObject *args );
	static PyObject *cfGetRepeatX( PyObject *myself );
	static PyObject *cfSetRepeatX( PyObject *myself, PyObject *args );
	static PyObject *cfGetRepeatY( PyObject *myself );
	static PyObject *cfSetRepeatY( PyObject *myself, PyObject *args );
	
	static PyObject *cfGetShapeType( PyObject *myself );
	static PyObject *cfSetShapeType( PyObject *myself, PyObject *args );
	static PyObject *cfGetLineColor( PyObject *myself );
	static PyObject *cfSetLineColor( PyObject *myself, PyObject *args );
	static PyObject *cfGetFillColor( PyObject *myself );
	static PyObject *cfSetFillColor( PyObject *myself, PyObject *args );
	static PyObject *cfGetThickness( PyObject *myself );
	static PyObject *cfSetThickness( PyObject *myself, PyObject *args );
	static PyObject *cfGetPointCount( PyObject *myself );
	static PyObject *cfGetPointAt( PyObject *myself, PyObject *args );
	static PyObject *cfAddPoint( PyObject *myself, PyObject *args );
	static PyObject *cfRemoveAllPoints( PyObject *myself );
	
	static PyObject *cfGetCanvasView( PyObject *myself );
	static PyObject *cfSetCanvasView( PyObject *myself, PyObject *args );
	
	static PyObject *cfGetCamera( PyObject *myself );
	static PyObject *cfSetCamera( PyObject *myself, PyObject *args );
	
	static PyObject *cfGetFont( PyObject *myself );
	static PyObject *cfSetFont( PyObject *myself, PyObject *args );
	static PyObject *cfGetText( PyObject *myself );
	static PyObject *cfSetText( PyObject *myself, PyObject *args );
	static PyObject *cfGetColor( PyObject *myself );
	static PyObject *cfSetColor( PyObject *myself, PyObject *args );
	
	static PyObject *cfGetVideoPlayer( PyObject *myself );
	static PyObject *cfSetVideoPlayer( PyObject *myself, PyObject *args );
	
	static PyObject *cfGetCanvasCount( PyObject *myself, PyObject *args );
	static PyObject *cfAddCanvas( PyObject *myself, PyObject *args );
	static PyObject *cfRemoveCanvas( PyObject *myself, PyObject *args );
	static PyObject *cfRemoveAllCanvas( PyObject *myself );
	/*@}*/
};

#endif
