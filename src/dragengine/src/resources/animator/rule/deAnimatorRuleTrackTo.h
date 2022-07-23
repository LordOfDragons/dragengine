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

#ifndef _DEANIMATORRULETRACKTO_H_
#define _DEANIMATORRULETRACKTO_H_

#include "deAnimatorRule.h"
#include "../../../common/string/decString.h"


/**
 * \brief Animator Track To Rule.
 * Adjust the orientation of the bones to point one of their axes towards another
 * bone or a controller position. A second bone axis is orientated towards the
 * component up axis, the tracked bone up axis or a controller vector. If align
 * up axis is disabled the bones up-axes are oriented towards the component up-axis.
 * If enabled the bone up-axes are oriented towards the track bone is set otherwise
 * the up-axis is taken from the up-controller vector. In addition one bone-axis
 * can be locked to obtain a locked track.
 */
class DE_DLL_EXPORT deAnimatorRuleTrackTo : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleTrackTo> Ref;
	
	
	
public:
	/** \brief Track axis. */
	enum eTrackAxis{
		/** \brief Positive X-Axis. */
		etaPosX,
		
		/** \brief Positive Y-Axis. */
		etaPosY,
		
		/** \brief Positive Z-Axis. */
		etaPosZ,
		
		/** \brief Negative X-Axis. */
		etaNegX,
		
		/** \brief Negative Y-Axis. */
		etaNegY,
		
		/** \brief Negative Z-Axis. */
		etaNegZ
	};
	
	/** \brief Up-axis target. */
	enum eUpTarget{
		/** \brief World X-Axis. */
		eutWorldX,
		
		/** \brief World Y-Axis. */
		eutWorldY,
		
		/** \brief World Z-Axis. */
		eutWorldZ,
		
		/** \brief Component X-Axis. */
		eutComponentX,
		
		/** \brief Component Y-Axis. */
		eutComponentY,
		
		/** \brief Component Z-Axis. */
		eutComponentZ,
		
		/** \brief Track Bone X-Axis. */
		eutTrackBoneX,
		
		/** \brief Track Bone Y-Axis. */
		eutTrackBoneY,
		
		/** \brief Track Bone Z-Axis. */
		eutTrackBoneZ,
		
		/** \brief Controller Vector. */
		eutController,
	};
	
	/** \brief Locked axis. */
	enum eLockedAxis{
		/** \brief No locked axis. */
		elaNone,
		
		/** \brief Locked X-Axis. */
		elaX,
		
		/** \brief Locked Y-Axis. */
		elaY,
		
		/** \brief Locked Z-Axis. */
		elaZ
	};
	
	
	
private:
	decString pTrackBone;
	eTrackAxis pTrackAxis;
	eTrackAxis pUpAxis;
	eUpTarget pUpTarget;
	eLockedAxis pLockedAxis;
	
	deAnimatorControllerTarget pTargetPosition;
	deAnimatorControllerTarget pTargetUp;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator rule. */
	deAnimatorRuleTrackTo();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleTrackTo();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name of the track bone or empty string to use none. */
	inline const decString &GetTrackBone() const{ return pTrackBone; }
	
	/** \brief Set name of the track bone or empty string to use none. */
	void SetTrackBone( const char *boneName );
	
	/** \brief Track axis. */
	inline eTrackAxis GetTrackAxis() const{ return pTrackAxis; }
	
	/** \brief Set track axis. */
	void SetTrackAxis( eTrackAxis axis );
	
	/** \brief Up axis. */
	inline eTrackAxis GetUpAxis() const{ return pUpAxis; }
	
	/** \brief Set up axis. */
	void SetUpAxis( eTrackAxis axis );
	
	/** \brief Up target. */
	inline eUpTarget GetUpTarget() const{ return pUpTarget; }
	
	/** \brief Set up target. */
	void SetUpTarget( eUpTarget target );
	
	/** \brief Locked axis. */
	inline eLockedAxis GetLockedAxis() const{ return pLockedAxis; }
	
	/** \brief Set locked axis. */
	void SetLockedAxis( eLockedAxis axis );
	
	/** \brief Position target. */
	inline deAnimatorControllerTarget &GetTargetPosition(){ return pTargetPosition; }
	inline const deAnimatorControllerTarget &GetTargetPosition() const{ return pTargetPosition; }
	
	/** \brief Up target. */
	inline deAnimatorControllerTarget &GetTargetUp(){ return pTargetUp; }
	inline const deAnimatorControllerTarget &GetTargetUp() const{ return pTargetUp; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
