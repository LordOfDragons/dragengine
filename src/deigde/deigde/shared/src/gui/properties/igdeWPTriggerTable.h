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

#include "../igdeListBox.h"
#include "../igdeTextField.h"
#include "../event/igdeActionListener.h"
#include "../event/igdeAction.h"
#include "../layout/igdeContainerFlow.h"
#include "../resources/igdeIcon.h"


class igdeTriggerTargetList;
class igdeTriggerTarget;
class igdeEnvironment;


/**
 * \brief Trigger table panel.
 */
class DE_DLL_EXPORT igdeWPTriggerTable : public igdeContainerFlow, igdeActionListener{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeWPTriggerTable> Ref;
	
	
private:
	igdeTriggerTargetList *pTriggerTargetList;
	
	igdeIcon::Ref pIconFired;
	igdeIcon::Ref pIconNotFired;
	
	igdeAction::Ref pActionToggle;
	igdeAction::Ref pActionClear;
	igdeAction::Ref pActionAdd;
	igdeAction::Ref pActionRemove;
	igdeAction::Ref pActionFire;
	igdeAction::Ref pActionReset;
	igdeAction::Ref pActionFullReset;
	
	igdeListBox::Ref pListTriggerTable;
	igdeTextField::Ref pEditTriggerTableFilter;
	
	igdeAction::Ref pAction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeWPTriggerTable(igdeEnvironment &environment);
	
	/** \brief Create panel. */
	igdeWPTriggerTable(igdeEnvironment &environment, igdeAction *action);
	
	
	
protected:
	/** \brief Clean up widget. */
	virtual ~igdeWPTriggerTable();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Trigger target list or nullptr if not set. */
	inline igdeTriggerTargetList *GetTriggerTargetList() const{ return pTriggerTargetList; }
	
	/** \brief Set trigger target list or nullptr if not set. */
	void SetTriggerTargetList(igdeTriggerTargetList *triggerTargetList);
	
	/** \brief Selected target or nullptr. */
	igdeTriggerTarget *GetSelectedTarget() const;
	
	/** \brief Select target if possible. */
	void SelectTarget(igdeTriggerTarget *target);
	
	/** \brief Update after changing trigger target list outside widget. */
	void UpdateTable();
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionToggle() const{ return pActionToggle; }
	inline const igdeAction::Ref &GetActionClear() const{ return pActionClear; }
	inline const igdeAction::Ref &GetActionAdd() const{ return pActionAdd; }
	inline const igdeAction::Ref &GetActionRemove() const{ return pActionRemove; }
	inline const igdeAction::Ref &GetActionFire() const{ return pActionFire; }
	inline const igdeAction::Ref &GetActionReset() const{ return pActionReset; }
	inline const igdeAction::Ref &GetActionFullReset() const{ return pActionFullReset; }
	
	
	
	/** \brief Action or nullptr. */
	inline const igdeAction::Ref &GetAction() const{ return pAction; }
	
	/** \brief Set action or nullptr. */
	void SetAction(igdeAction *action);
	
	/**
	 * \brief Trigger table changed.
	 * 
	 * Called if user changes any of the trigger table parameters.
	 * Default implementation calls OnAction on set action object if present.
	 */
	virtual void OnAction();
	
	/** \brief Action parameters changed. */
	virtual void OnParameterChanged(igdeAction *action);
	
	/** \brief Action has been destroyed. */
	virtual void OnDestroyed(igdeAction *action);
	/*@}*/
	
	
	
private:
	void pCreateContent();
};

#endif
