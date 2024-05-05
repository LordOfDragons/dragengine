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

#ifndef _DEBASEPHYSICSPARTICLEEMITTER_H_
#define _DEBASEPHYSICSPARTICLEEMITTER_H_

#include "../../../dragengine_export.h"


/**
 * \brief Physics Emitter Particle System Peer.
 */
class DE_DLL_EXPORT deBasePhysicsParticleEmitter{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBasePhysicsParticleEmitter();
	
	/** \brief Clean up peer. */
	virtual ~deBasePhysicsParticleEmitter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Burst parameters changed. */
	virtual void BurstChanged();
	
	/** \brief Controller count changed. */
	virtual void ControllerCountChanged();
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( int controller );
	
	/** \brief Type count changed. */
	virtual void TypeCountChanged();
	
	/** \brief Type changed. */
	virtual void TypeChanged( int type );
	
	/** \brief Requests of the Graphic Module changed. */
	virtual void GraphicModuleRequestsChanged();
	/*@}*/
};

#endif
