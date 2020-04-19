/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEBASEANIMATORANIMATORINSTANCE_H_
#define _DEBASEANIMATORANIMATORINSTANCE_H_



/**
 * \brief Animator module animator instance peer.
 */
class deBaseAnimatorAnimatorInstance{
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
