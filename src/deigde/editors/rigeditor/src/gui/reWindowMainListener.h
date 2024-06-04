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
private:
	reWindowMain &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	reWindowMainListener( reWindowMain &window );
	
	/** \brief Clean up listener. */
	~reWindowMainListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Element or work mode changed. */
	virtual void ModeChanged( reRig *rig );
	
	/** \brief Changed or saved state changed. */
	virtual void StateChanged( reRig *rig );
	
	/** \brief Undos changed. */
	virtual void UndoChanged( reRig *rig );
	
	
	
	/** \brief A bone changed selection state. */
	virtual void BoneSelectedChanged( reRig *rig, reRigBone *bone );
	
	/** \brief All bones have been deselected. */
	virtual void AllBonesDeselected( reRig *rig );
	
	/** \brief The active bone changed. */
	virtual void ActiveBoneChanged( reRig *rig );
	
	
	
	/** \brief Shape count changed. */
	virtual void ShapeCountChanged( reRig *rig );
	
	/** \brief A shape changed selection state. */
	virtual void ShapeSelectedChanged( reRig *rig, reRigShape *shape );
	
	/** \brief All shapes have been deselected. */
	virtual void AllShapesDeselected( reRig *rig );
	
	/** \brief The active shape changed. */
	virtual void ActiveShapeChanged( reRig *rig );
	
	
	
	/** \brief Constraint count changed. */
	virtual void ConstraintCountChanged( reRig *rig );
	
	/** \brief A constraint changed selection state. */
	virtual void ConstraintSelectedChanged( reRig *rig, reRigConstraint *constraint );
	
	/** \brief All constraints have been deselected. */
	virtual void AllConstraintsDeselected( reRig *rig );
	
	/** \brief The active constraint changed. */
	virtual void ActiveConstraintChanged( reRig *rig );
	
	
	
	/** \brief Push count changed. */
	virtual void PushCountChanged( reRig *rig );
	
	/** \brief A push changed selection state. */
	virtual void PushSelectedChanged( reRig *rig, reRigPush *push );
	
	/** \brief All pushes have been deselected. */
	virtual void AllPushesDeselected( reRig *rig );
	
	/** \brief The active push changed. */
	virtual void ActivePushChanged( reRig *rig );
	/*@}*/
};

#endif
