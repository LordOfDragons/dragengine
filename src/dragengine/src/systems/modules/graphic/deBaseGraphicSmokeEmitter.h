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

#ifndef _DEBASEGRAPHICSMOKEEMITTER_H_
#define _DEBASEGRAPHICSMOKEEMITTER_H_

#include "../../../dragengine_export.h"


class deSmokeEmitter;


/**
 * \brief Graphic Module Smoke Emitter Peer
 *
 * Peer for the smoke emitter resource used by graphic modules.
 */
class DE_DLL_EXPORT deBaseGraphicSmokeEmitter{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBaseGraphicSmokeEmitter();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicSmokeEmitter();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Volume position changed. */
	virtual void VolumePositionChanged();
	
	/** \brief Skin changed. */
	virtual void SkinChanged();
	
	/** \brief Points changed. */
	virtual void PointsChanged();
	/*@}*/
};

#endif
