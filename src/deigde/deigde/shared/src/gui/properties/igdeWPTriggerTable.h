/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEWPTRIGGERTABLE_H_
#define _IGDEWPTRIGGERTABLE_H_

#include <dragengine/common/math/decMath.h>

#include "../igdeListBoxReference.h"
#include "../igdeTextFieldReference.h"
#include "../event/igdeActionListener.h"
#include "../event/igdeActionReference.h"
#include "../layout/igdeContainerFlow.h"
#include "../resources/igdeIconReference.h"


class igdeTriggerTargetList;
class igdeTriggerTarget;
class igdeEnvironment;


/**
 * \brief Trigger table panel.
 */
class igdeWPTriggerTable : public igdeContainerFlow, igdeActionListener{
private:
	igdeTriggerTargetList *pTriggerTargetList;
	
	igdeIconReference pIconFired;
	igdeIconReference pIconNotFired;
	
	igdeActionReference pActionToggle;
	igdeActionReference pActionClear;
	igdeActionReference pActionAdd;
	igdeActionReference pActionRemove;
	igdeActionReference pActionFire;
	igdeActionReference pActionReset;
	igdeActionReference pActionFullReset;
	
	igdeListBoxReference pListTriggerTable;
	igdeTextFieldReference pEditTriggerTableFilter;
	
	igdeActionReference pAction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeWPTriggerTable( igdeEnvironment &environment );
	
	/** \brief Create panel. */
	igdeWPTriggerTable( igdeEnvironment &environment, igdeAction *action );
	
	
	
protected:
	/** \brief Clean up widget. */
	virtual ~igdeWPTriggerTable();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Trigger target list or NULL if not set. */
	inline igdeTriggerTargetList *GetTriggerTargetList() const{ return pTriggerTargetList; }
	
	/** \brief Set trigger target list or NULL if not set. */
	void SetTriggerTargetList( igdeTriggerTargetList *triggerTargetList );
	
	/** \brief Selected target or NULL. */
	igdeTriggerTarget *GetSelectedTarget() const;
	
	/** \brief Select target if possible. */
	void SelectTarget( igdeTriggerTarget *target );
	
	/** \brief Update after changing trigger target list outside widget. */
	void UpdateTable();
	
	/** \brief Actions. */
	inline igdeAction *GetActionToggle() const{ return pActionToggle; }
	inline igdeAction *GetActionClear() const{ return pActionClear; }
	inline igdeAction *GetActionAdd() const{ return pActionAdd; }
	inline igdeAction *GetActionRemove() const{ return pActionRemove; }
	inline igdeAction *GetActionFire() const{ return pActionFire; }
	inline igdeAction *GetActionReset() const{ return pActionReset; }
	inline igdeAction *GetActionFullReset() const{ return pActionFullReset; }
	
	
	
	/** \brief Action or NULL. */
	inline igdeAction *GetAction() const{ return pAction; }
	
	/** \brief Set action or NULL. */
	void SetAction( igdeAction *action );
	
	/**
	 * \brief Trigger table changed.
	 * 
	 * Called if user changes any of the trigger table parameters.
	 * Default implementation calls OnAction on set action object if present.
	 */
	virtual void OnAction();
	
	/** \brief Action parameters changed. */
	virtual void OnParameterChanged( igdeAction *action );
	
	/** \brief Action has been destroyed. */
	virtual void OnDestroyed( igdeAction *action );
	/*@}*/
	
	
	
private:
	void pCreateContent();
};

#endif
