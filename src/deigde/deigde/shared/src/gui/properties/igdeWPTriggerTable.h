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
class DE_DLL_EXPORT igdeWPTriggerTable : public igdeContainerFlow, igdeActionListener{
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
