/* 
 * Drag[en]gine IGDE Rig Editor
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
