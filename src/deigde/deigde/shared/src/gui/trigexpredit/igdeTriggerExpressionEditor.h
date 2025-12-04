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

#ifndef _IGDETRIGGEREXPRESSIONEDITOR_H_
#define _IGDETRIGGEREXPRESSIONEDITOR_H_

#include "../igdeButtonReference.h"
#include "../igdeToggleButtonReference.h"
#include "../igdeTextFieldReference.h"
#include "../igdeListBoxReference.h"
#include "../igdeTreeListReference.h"
#include "../event/igdeActionListener.h"
#include "../event/igdeAction.h"
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
	
	igdeAction::Ref pAction;
	
	
	
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
