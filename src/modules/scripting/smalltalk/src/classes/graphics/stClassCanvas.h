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

#ifndef _STCLASSCANVAS_H_
#define _STCLASSCANVAS_H_

#include "../../stCommon.h"

class ScriptingSmalltalk;
class deCanvas;



/**
 * \brief Script class canvas.
 */
class stClassCanvas{
private:
	ScriptingSmalltalk &pST;
	OOP pOOPClass;
	
	// deCanvasVisitorIdentify::eCanvasTypes
	OOP pSymbolImage;
	OOP pSymbolPaint;
	OOP pSymbolCanvasView;
	OOP pSymbolRenderWorld;
	OOP pSymbolText;
	OOP pSymbolVideoPlayer;
	OOP pSymbolView;
	
	// deCanvasPaint::eShapeTypes
	OOP pSymbolPoints;
	OOP pSymbolLines;
	OOP pSymbolRectangle;
	OOP pSymbolEllipse;
	OOP pSymbolPie;
	OOP pSymbolPolygon;

public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	stClassCanvas(ScriptingSmalltalk &st);
	
	/** \brief Clean up script class. */
	virtual ~stClassCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline ScriptingSmalltalk &GetST() const{ return pST; }
	
	/** \brief Set up links. */
	void SetUpLinks();
	
	/** \brief Canvas from an object or \em NULL if object is \em nil. */
	deCanvas *OOPToCanvas(OOP object) const;
	
	/** \brief Push canvas object or \em nil if canvas is \em NULL. */
	OOP CanvasToOOP(deCanvas *canvas);
	/*@}*/
	
	
	
	/** @name cCall Methods */
	/*@{*/
	
	/** \brief Create new canvas object. */
	static OOP ccNew(OOP self, OOP type);
	
	/** \brief Finalize. */
	static void ccFinalize(OOP self);
	
	
	
	/** \brief Type of canvas. */
	static OOP ccGetType(OOP self);
	
	/** \brief Position in parent canvas view units. */
	static OOP ccGetPosition(OOP self);
	
	/** \brief Set position in parent canvas view units. */
	static void ccSetPosition(OOP self, OOP position);
	
	/** \brief Size in parent canvas view units. */
	static OOP ccGetSize(OOP self);
	
	/** \brief Set size in parent canvas view units. */
	static void ccSetSize(OOP self, OOP size);
	
	/** \brief Transformation matrix affecting the canvas and all its content. */
	static OOP ccGetTransform(OOP self);
	
	/** \brief Set transformation matrix affecting the canvas and all its content. */
	static void ccSetTransform(OOP self, OOP transform);
	
	/** \brief Canvas is visible. */
	static bool ccGetVisible(OOP self);
	
	/** \brief Set if canvas is visible. */
	static void ccSetVisible(OOP self, bool visible);
	
	/** \brief Render order. Larger renders ontop of smaller. */
	static float ccGetOrder(OOP self);
	
	/** \brief Set render order. Larger renders ontop of smaller. */
	static void ccSetOrder(OOP self, float order);
	
	/** \brief Transparency in the range from 0 to 1 where 1 is opaque and 0 fully transparent. */
	static float ccGetTransparency(OOP self);
	
	/** \brief Set transparency in the range from 0 to 1 where 1 is opaque and 0 fully transparent. */
	static void ccSetTransparency(OOP self, float transparency);
	
	
	
	
	/** \brief Image or \em NULL if not set. */
	static OOP ccGetImage(OOP self);
	
	/** \brief Set image or \em NULL to unset. */
	static void ccSetImage(OOP self, OOP image);
	
	/** \brief Repeat count along X axis. */
	static int ccGetRepeatX(OOP self);
	
	/** \brief Set repeat count along X axis. */
	static void ccSetRepeatX(OOP self, int count);
	
	/** \brief Repeat count along Y axis. */
	static int ccGetRepeatY(OOP self);
	
	/** \brief Set repeat count along Y axis. */
	static void ccSetRepeatY(OOP self, int count);
	
	
	
	/** \brief Shape type. */
	static OOP ccGetShapeType(OOP self);
	
	/** \brief Set shape type. */
	static void ccSetShapeType(OOP self, OOP shapeType);
	
	/** \brief Line color. */
	static OOP ccGetLineColor(OOP self);
	
	/** \brief Set line color. */
	static void ccSetLineColor(OOP self, OOP color);
	
	/** \brief Fill color. */
	static OOP ccGetFillColor(OOP self);
	
	/** \brief Set fill color. */
	static void ccSetFillColor(OOP self, OOP color);
	
	/** \brief Line thickness in units. */
	static float ccGetThickness(OOP self);
	
	/** \brief Line thickness in units. */
	static void ccSetThickness(OOP self, float thickness);
	
	/** \brief Number of points. */
	static int ccGetPointCount(OOP self);
	
	/** \brief Point at position. */
	static OOP ccGetPointAt(OOP self, int index);
	
	/** \brief Add a point. */
	static void ccAddPoint(OOP self, OOP point);
	
	/** \brief Remove all points. */
	static void ccRemoveAllPoints(OOP self);
	
	
	
	
	/** \brief Canvas view or \em NULL if not set. */
	static OOP ccGetCanvasView(OOP self);
	
	/** \brief Set canvas view or \em NULL to unset. */
	static void ccSetCanvasView(OOP self, OOP canvasView);
	
	
	
	/** \brief Camera or \em NULL if not set. */
	static OOP ccGetCamera(OOP self);
	
	/** \brief Set camera or \em NULL to unset. */
	static void ccSetCamera(OOP self, OOP camera);
	
	
	
	/** \brief Font or \em NULL if not set. */
	static OOP ccGetFont(OOP self);
	
	/** \brief Set font or \em NULL to unset. */
	static void ccSetFont(OOP self, OOP font);
	
	/** \brief Text. */
	static OOP ccGetText(OOP self);
	
	/** \brief Set text. */
	static void ccSetText(OOP self, OOP text);
	
	/** \brief Text color. */
	static OOP ccGetColor(OOP self);
	
	/** \brief Set text color. */
	static void ccSetColor(OOP self, OOP color);
	
	
	
	/** \brief Video player or \em NULL if not set. */
	static OOP ccGetVideoPlayer(OOP self);
	
	/** \brief Set video player or \em NULL to unset. */
	static void ccSetVideoPlayer(OOP self, OOP videoPlayer);
	
	
	
	/** \brief Number of child canvas. */
	static int ccGetCanvasCount(OOP self);
	
	/** \brief Add child canvas. */
	static void ccAddCanvas(OOP self, OOP canvas);
	
	/** \brief Remove child canvas. */
	static void ccRemoveCanvas(OOP self, OOP canvas);
	
	/** \brief Remove all child canvas. */
	static void ccRemoveAllCanvas(OOP self);
	/*@}*/
};

#endif
