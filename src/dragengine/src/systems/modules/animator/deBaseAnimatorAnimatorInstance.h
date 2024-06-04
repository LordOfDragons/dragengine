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

#ifndef _DEBASEANIMATORANIMATORINSTANCE_H_
#define _DEBASEANIMATORANIMATORINSTANCE_H_

#include "../../../dragengine_export.h"



/**
 * \brief Animator module animator instance peer.
 */
class DE_DLL_EXPORT deBaseAnimatorAnimatorInstance{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBaseAnimatorAnimatorInstance();
	
	/** \brief Clean up peer. */
	virtual ~deBaseAnimatorAnimatorInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	
	/**
	 * \brief Apply state of animator to the component if existing.
	 * 
	 * Animator modules can decide to calculate this in parallel. If
	 * \em direct is \em true the application is always done
	 * synchronously and is done after the call returns. If \em direct
	 * is \em false the calculation can be parallel. The affected
	 * resources take care of waiting for the result to become ready
	 * if required.
	 */
	virtual void Apply( bool direct );
	
	/**
	 * \brief Capture current state of component into rules matching identifier.
	 */
	virtual void CaptureStateInto( int identifier );
	
	/**
	 * \brief Store animation frame from animation into rules matching identifier.
	 * 
	 * If \em moveName does not exist in the animation a default state is captured.
	 * 
	 * \throws deeInvalidParam \em moveName is NULL.
	 */
	virtual void StoreFrameInto( int identifier, const char *moveName, float moveTime );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Animator changed. */
	virtual void AnimatorChanged();
	
	/** \brief Component changed. */
	virtual void ComponentChanged();
	
	/** \brief Animation changed. */
	virtual void AnimationChanged();
	
	/** \brief Blend factor changed. */
	virtual void BlendFactorChanged();
	
	/** \brief Enable retargeting changed. */
	virtual void EnableRetargetingChanged();
	
	/** \brief Protect dynamic bones changed. */
	virtual void ProtectDynamicBonesChanged();
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( int index );
	/*@}*/
};

#endif
