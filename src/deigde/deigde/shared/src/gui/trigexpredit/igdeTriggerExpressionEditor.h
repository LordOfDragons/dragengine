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

#ifndef _IGDETRIGGEREXPRESSIONEDITOR_H_
#define _IGDETRIGGEREXPRESSIONEDITOR_H_

#include "../igdeButtonReference.h"
#include "../igdeToggleButtonReference.h"
#include "../igdeTextFieldReference.h"
#include "../igdeListBoxReference.h"
#include "../igdeTreeListReference.h"
#include "../event/igdeActionListener.h"
#include "../event/igdeActionReference.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/common/string/decString.h>


class igdeTriggerExpression;
class igdeTriggerExpressionComponent;
class igdeTriggerExpressionParser;
class igdeTriggerTargetList;
class igdeTreeItem;


/**
 * \brief Trigger Expression Editor.
 */
class DE_DLL_EXPORT igdeTriggerExpressionEditor : public igdeContainerFlow, igdeActionListener{
private:
	igdeTriggerExpressionParser *pParser;
	const igdeTriggerTargetList *pTargetList;
	igdeTriggerExpression *pWorkExpression;
	decString pExpression;
	
	igdeToggleButtonReference pBtnNegate;
	igdeToggleButtonReference pBtnCurState;
	igdeToggleButtonReference pBtnAnd;
	igdeToggleButtonReference pBtnOr;
	igdeToggleButtonReference pBtnTarget;
	igdeButtonReference pBtnAddChild;
	igdeButtonReference pBtnRemoveChild;
	igdeTextFieldReference pEditFilterTargetName;
	igdeTextFieldReference pEditTargetName;
	igdeListBoxReference pListTargetName;
	igdeTextFieldReference pEditExpression;
	igdeTreeListReference pTreeExpression;
	
	igdeActionReference pAction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create editor. */
	igdeTriggerExpressionEditor( igdeEnvironment &environment );
	
	/** \brief Create editor. */
	igdeTriggerExpressionEditor( igdeEnvironment &environment, igdeAction *action );
	
	
	
protected:
	/**
	 * \brief Clean up editor.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeTriggerExpressionEditor();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parser. */
	inline igdeTriggerExpressionParser *GetParser() const{ return pParser; }
	
	/** \brief Set parser. */
	void SetParser( igdeTriggerExpressionParser *parser );
	
	/** \brief Target list. */
	inline const igdeTriggerTargetList *GetTargetList() const{ return pTargetList; }
	
	/** \brief Set target list. */
	void SetTargetList( const igdeTriggerTargetList *targetList );
	
	/** \brief Expression. */
	inline const decString &GetExpression() const{ return pExpression; }
	
	/** \brief Set expression. */
	void SetExpression( const char *expression );
	
	/** \brief Action or NULL. */
	inline igdeAction *GetAction() const{ return pAction; }
	
	/** \brief Set action or NULL. */
	void SetAction( igdeAction *action );
	
	
	
	/** \brief Update text. */
	void UpdateText();
	
	/** \brief Rebuild tree. */
	void RebuildTree();
	
	/** \brief Update tree. */
	void UpdateTree();
	
	/** \brief Add expression component to tree. */
	void AddComponentToTree( igdeTreeItem *parent, igdeTriggerExpressionComponent *component );
	
	/** \brief Update expresion component in the tree. */
	void UpdateComponentInTree( igdeTreeItem *item, igdeTriggerExpressionComponent *component );
	
	/** \brief Update expression component currently selected in the tree. */
	void UpdateExpressionComponent();
	
	/** \brief Update expression from tree. */
	void UpdateExpressionFromTree();
	
	/** \brief Selected tree item. */
	igdeTreeItem *GetSelectedTreeItem() const;
	
	/** \brief Selected component in the tree or NULL if there is no valid selection. */
	igdeTriggerExpressionComponent *GetSelectedComponent() const;
	
	/** \brief Update from target list. */
	void UpdateFromTargetList();
	
	/** \brief Set index of selected item in the target list or -1 if there is no selection. */
	void SetSelectedItemInTargetList( int selection );
	
	/** \brief Select target in list. */
	void SetSelectedItemInTargetList( const char *name );
	
	
	
	/**
	 * \brief Run action.
	 * 
	 * Called if expression changed by user. Default implementation calls OnAction
	 * on set action object if present.
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
