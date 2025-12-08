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

#ifndef _REWPCONSTRAINTLISTENER_H_
#define _REWPCONSTRAINTLISTENER_H_

#include "../../rig/reRigNotifier.h"

class reWPConstraint;



/**
 * \brief Constraint panel listener.
 */
class reWPConstraintListener : public reRigNotifier{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<reWPConstraintListener> Ref;
	
	

private:
	reWPConstraint &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	reWPConstraintListener(reWPConstraint &panel);
	
	/** \brief Clean up listener. */
	~reWPConstraintListener() override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Bone count changed. */
	void BoneCountChanged(reRig *rig) override;
	
	/** \brief A bone changed. */
	void BoneChanged(reRig *rig, reRigBone *bone) override;
	
	/** \brief A constraint changed. */
	void ConstraintChanged(reRig *rig, reRigConstraint *constraint) override;
	
	/** \brief A constraint degree of freedom changed. */
	virtual void ConstraintDofChanged(reRig *rig, reRigConstraint *constraint,
		deColliderConstraint::eDegreesOfFreedom dof);
	
	/** \brief The active constraint changed. */
	void ActiveConstraintChanged(reRig *rig) override;
	/*@}*/
};

#endif
