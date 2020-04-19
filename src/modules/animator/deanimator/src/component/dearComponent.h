/* 
 * Drag[en]gine Animator Module
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

#ifndef _DEARCOMPONENT_H_
#define _DEARCOMPONENT_H_

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorComponent.h>

class dearComponentBoneState;
class dearTaskApplyRules;
class deDEAnimator;
class deComponent;



/**
 * \brief Component peer.
 */
class dearComponent : public deBaseAnimatorComponent{
private:
	deDEAnimator &pModule;
	deComponent &pComponent;
	
	dearComponentBoneState *pBoneStates;
	int pBoneStateCount;
	decDMatrix pMatrix;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	dearComponent( deDEAnimator &module, deComponent &component );
	
	/** \brief Clean up peer. */
	virtual ~dearComponent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deDEAnimator &GetModule(){ return pModule; }
	inline const deDEAnimator &GetModule() const{ return pModule; }
	
	/** \brief Component resource. */
	inline deComponent &GetComponent() const{ return pComponent; }
	
	
	
	/** \brief Bone states. */
	inline dearComponentBoneState *GetBoneStates() const{ return pBoneStates; }
	
	/** \brief Number of bone states. */
	inline int GetBoneStateCount() const{ return pBoneStateCount; }
	
	/**
	 * \brief Bone state at index.
	 * \throws deeInvalidParam \em index is less than 0.
	 * \throws deeInvalidParam \em index is greater or equal than GetBoneStateCount().
	 */
	dearComponentBoneState &GetBoneStateAt( int index ) const;
	
	/** \brief Component matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Prepare bones similar to deComponent::PrepareBones(). */
	void PrepareBones();
	
	/** \brief Update bone states from component bones and store component matrix. */
	void UpdateFromComponent();
	
	/** \brief Update matrix from component only. */
	void UpdateMatrixFromComponent();
	
	/** \brief Update component bones from bone states. */
	void UpdateComponent();
	
	/** \brief Update component bones using values calculated by PrepareBones() and set the component valid. */
	void UpdateComponentPrepareBones();
	
	
	
	/** \brief Rig changed. */
	virtual void RigChanged();
	/*@}*/
};

#endif
