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
class dearComponentVPSState;
class deDEAnimator;
class deComponent;



/**
 * Component peer.
 */
class dearComponent : public deBaseAnimatorComponent{
private:
	deDEAnimator &pModule;
	deComponent &pComponent;
	
	dearComponentBoneState *pBoneStates;
	int pBoneStateCount;
	
	dearComponentVPSState *pVPSStates;
	int pVPSStateCount;
	
	decDMatrix pMatrix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	dearComponent( deDEAnimator &module, deComponent &component );
	
	/** Clean up peer. */
	virtual ~dearComponent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Module. */
	inline deDEAnimator &GetModule(){ return pModule; }
	inline const deDEAnimator &GetModule() const{ return pModule; }
	
	/** Component resource. */
	inline deComponent &GetComponent() const{ return pComponent; }
	
	
	
	/** Bone states. */
	inline dearComponentBoneState *GetBoneStates() const{ return pBoneStates; }
	
	/** Number of bone states. */
	inline int GetBoneStateCount() const{ return pBoneStateCount; }
	
	/** Bone state at index. */
	dearComponentBoneState &GetBoneStateAt( int index ) const;
	
	
	
	/** Vertex position set states. */
	inline dearComponentVPSState *GetVPSStates() const{ return pVPSStates; }
	
	/** Count of vertex position set states. */
	inline int GetVPSStateCount() const{ return pVPSStateCount; }
	
	/** Vertex position set state at index. */
	dearComponentVPSState &GetVPSStateAt( int index ) const;
	
	
	
	/** Component matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	
	
	/** Prepare bones similar to deComponent::PrepareBones(). */
	void PrepareBones();
	
	/** Update bone states from component bones and store component matrix. */
	void UpdateFromComponent();
	
	/** Update matrix from component only. */
	void UpdateMatrixFromComponent();
	
	/** Update component bones from bone states. */
	void UpdateComponent();
	
	/** Update component bones using values calculated by PrepareBones() and set the component valid. */
	void UpdateComponentPrepareBones();
	
	
	
	/** Model changed. */
	virtual void ModelChanged();
	
	/** Rig changed. */
	virtual void RigChanged();
	/*@}*/
};

#endif
