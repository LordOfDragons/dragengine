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

#ifndef _REWINDOWMAINLISTENER_H_
#define _REWINDOWMAINLISTENER_H_

#include "../rig/reRigNotifier.h"

class reWindowMain;



/**
 * \brief Main window listener.
 */
class reWindowMainListener : public reRigNotifier{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<reWindowMainListener> Ref;
	
	

private:
	reWindowMain &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	reWindowMainListener(reWindowMain &window);
	
	/** \brief Clean up listener. */
	~reWindowMainListener() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Element or work mode changed. */
	void ModeChanged(reRig *rig) override;
	
	/** \brief Changed or saved state changed. */
	void StateChanged(reRig *rig) override;
	
	/** \brief Undos changed. */
	void UndoChanged(reRig *rig) override;
	
	
	
	/** \brief A bone changed selection state. */
	void BoneSelectedChanged(reRig *rig, reRigBone *bone) override;
	
	/** \brief All bones have been deselected. */
	void AllBonesDeselected(reRig *rig) override;
	
	/** \brief The active bone changed. */
	void ActiveBoneChanged(reRig *rig) override;
	
	
	
	/** \brief Shape count changed. */
	void ShapeCountChanged(reRig *rig) override;
	
	/** \brief A shape changed selection state. */
	void ShapeSelectedChanged(reRig *rig, reRigShape *shape) override;
	
	/** \brief All shapes have been deselected. */
	void AllShapesDeselected(reRig *rig) override;
	
	/** \brief The active shape changed. */
	void ActiveShapeChanged(reRig *rig) override;
	
	
	
	/** \brief Constraint count changed. */
	void ConstraintCountChanged(reRig *rig) override;
	
	/** \brief A constraint changed selection state. */
	void ConstraintSelectedChanged(reRig *rig, reRigConstraint *constraint) override;
	
	/** \brief All constraints have been deselected. */
	void AllConstraintsDeselected(reRig *rig) override;
	
	/** \brief The active constraint changed. */
	void ActiveConstraintChanged(reRig *rig) override;
	
	
	
	/** \brief Push count changed. */
	void PushCountChanged(reRig *rig) override;
	
	/** \brief A push changed selection state. */
	void PushSelectedChanged(reRig *rig, reRigPush *push) override;
	
	/** \brief All pushes have been deselected. */
	void AllPushesDeselected(reRig *rig) override;
	
	/** \brief The active push changed. */
	void ActivePushChanged(reRig *rig) override;
	/*@}*/
};

#endif
