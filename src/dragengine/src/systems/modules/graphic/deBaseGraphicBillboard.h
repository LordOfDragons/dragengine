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

#ifndef _DEBASEGRAPHICBILLBOARD_H_
#define _DEBASEGRAPHICBILLBOARD_H_

#include "../../../dragengine_export.h"

/**
 * \brief Graphic Module Billboard Peer.
 */
class DE_DLL_EXPORT deBaseGraphicBillboard{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new peer. */
	deBaseGraphicBillboard();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicBillboard();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Axis changed. */
	virtual void AxisChanged();
	
	/** \brief Size changed. */
	virtual void SizeChanged();
	
	/** \brief Offset changed. */
	virtual void OffsetChanged();
	
	/** \brief Skin changed. */
	virtual void SkinChanged();
	
	/** \brief Dynamic skin changed. */
	virtual void DynamicSkinChanged();
	
	/** \brief Locked or spherical changed. */
	virtual void ParametersChanged();
	
	/** \brief Visibility changed. */
	virtual void VisibilityChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	/*@}*/
};

#endif
