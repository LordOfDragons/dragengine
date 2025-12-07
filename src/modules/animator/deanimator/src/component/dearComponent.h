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
	dearComponent(deDEAnimator &module, deComponent &component);
	
	/** Clean up peer. */
	virtual ~dearComponent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Module. */
	inline deDEAnimator &GetModule(){return pModule;}
	inline const deDEAnimator &GetModule() const{return pModule;}
	
	/** Component resource. */
	inline deComponent &GetComponent() const{return pComponent;}
	
	
	
	/** Bone states. */
	inline dearComponentBoneState *GetBoneStates() const{return pBoneStates;}
	
	/** Number of bone states. */
	inline int GetBoneStateCount() const{return pBoneStateCount;}
	
	/** Bone state at index. */
	dearComponentBoneState &GetBoneStateAt(int index) const;
	
	
	
	/** Vertex position set states. */
	inline dearComponentVPSState *GetVPSStates() const{return pVPSStates;}
	
	/** Count of vertex position set states. */
	inline int GetVPSStateCount() const{return pVPSStateCount;}
	
	/** Vertex position set state at index. */
	dearComponentVPSState &GetVPSStateAt(int index) const;
	
	
	
	/** Component matrix. */
	inline const decDMatrix &GetMatrix() const{return pMatrix;}
	
	
	
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
