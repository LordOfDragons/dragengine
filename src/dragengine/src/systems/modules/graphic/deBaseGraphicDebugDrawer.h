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

#ifndef _DEBASEGRAPHICDEBUGDRAWER_H_
#define _DEBASEGRAPHICDEBUGDRAWER_H_

#include "../../../dragengine_export.h"

class deDebugDrawerShape;


/**
 * \brief Graphic module debug drawer peer.
 */
class DE_DLL_EXPORT deBaseGraphicDebugDrawer{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBaseGraphicDebugDrawer();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicDebugDrawer();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Scaling changed. */
	virtual void ScalingChanged();
	
	/** \brief Visibile changed. */
	virtual void VisibleChanged();
	
	/** \brief X-Ray changed. */
	virtual void XRayChanged();
	
	/** \brief Shape visible changed. */
	virtual void ShapeVisibleChanged();
	
	/** \brief Shape edge or fill colors changed. */
	virtual void ShapeColorChanged();
	
	/** \brief Shape position, orientation or scaling changed. */
	virtual void ShapeGeometryChanged();
	
	/** \brief Shape shapes or faces changed. */
	virtual void ShapeContentChanged();
	
	/** \brief Shapes have been added or removed. */
	virtual void ShapeLayoutChanged();
	/*@}*/
};
#endif
