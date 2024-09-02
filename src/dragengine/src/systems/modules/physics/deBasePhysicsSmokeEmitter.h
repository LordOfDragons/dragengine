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

#ifndef _DEBASEPHYSICSSMOKEEMITTER_H_
#define _DEBASEPHYSICSSMOKEEMITTER_H_

#include "../../../dragengine_export.h"


/**
 * \brief Smoke Emitter Particle System Peer.
 *
 * Peer for the smoke emitter resource used by the physics system.
 */
class DE_DLL_EXPORT deBasePhysicsSmokeEmitter{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBasePhysicsSmokeEmitter();
	
	/** \brief Clean up peer. */
	virtual ~deBasePhysicsSmokeEmitter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Emitter position changed. */
	virtual void EmitterPositionChanged();
	
	/** \brief Emitter orientation changed. */
	virtual void EmitterOrientationChanged();
	
	/** \brief Emitter casting property changed. */
	virtual void CastPropertyChanged();
	
	/** \brief Enable casting changed. */
	virtual void EnableCastingChanged();
	
	/** \brief Component changed. */
	virtual void ComponentChanged();
	
	/** \brief Cast Texture changed. */
	virtual void CastTextureChanged();
	
	/** \brief Skin changed. */
	virtual void SkinChanged();
	
	/** \brief Collision filter changed. */
	virtual void CollisionFilterChanged();
	
	/** \brief Gravity changed. */
	virtual void GravityChanged();
	/*@}*/
};

#endif
