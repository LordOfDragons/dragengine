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

#ifndef _CEWPTTREEITEMMODEL_H_
#define _CEWPTTREEITEMMODEL_H_

#include <deigde/gui/resources/igdeIcon.h>

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceConversation;
class ceConversationAction;
class ceConversationCondition;
class cePlayback;
class ceWPTTreeItem;
class ceWPTTreeModel;
class ceWPTTIMAction;
class ceWPTTIMCondition;
class ceWindowMain;
class igdeMenuCascade;



/**
 * \brief Base class for tree item models.
 * 
 * Tree item models are responsible to update the visual state and content
 * of an assigned tree item.
 */
class ceWPTTreeItemModel : public deObject{
public:
	/** \brief Tree item types. */
	enum eTypes{
		etActionCameraShot,
		etActionMusic,
		etActionActorSpeak,
		etActionIfElse,
		etActionIfElseCase,
		etActionIfElseCaseCondition,
		etActionIfElseCaseActions,
		etActionIfElseElse,
		etActionPlayerChoice,
		etActionPlayerChoiceActions,
		etActionPlayerChoiceOption,
		etActionPlayerChoiceOptionCondition,
		etActionPlayerChoiceOptionActions,
		etActionStopConversation,
		etActionStopTopic,
		etActionSnippet,
		etActionSetVariable,
		etActionSetActorParameter,
		etActionActorCommand,
		etActionGameCommand,
		etActionWait,
		etActionWaitCondition,
		etActionWaitActions,
		etActionTrigger,
		etActionActorAdd,
		etActionActorRemove,
		etActionCoordSystemAdd,
		etActionCoordSystemRemove,
		etActionComment,
		
		etConditionLogic,
		etConditionHasActor,
		etConditionActorInConversation,
		etConditionVariable,
		etConditionActorParameter,
		etConditionActorCommand,
		etConditionGameCommand,
		etConditionTrigger
	};
	
	
	
private:
	ceWindowMain &pWindowMain;
	ceConversation &pConversation;
	const eTypes pType;
	
	decString pText, pDescription;
	igdeIcon::Ref pIcon;
	decObjectOrderedSet pChildren;
	bool pExpanded;
	
	ceWPTTreeModel *pTree; // weak reference;
	ceWPTTreeItemModel *pParent; // weak reference
	ceWPTTreeItem *pTreeItem; // weak reference
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete new tree item model. */
	ceWPTTreeItemModel( ceWindowMain &windowMain, ceConversation &conversation, eTypes type );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTreeItemModel();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Window selection. */
	inline ceWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Game definition. */
	inline ceConversation &GetConversation() const{ return pConversation; }
	
	/** \brief Tree item type. */
	inline eTypes GetType() const{ return pType; }
	
	
	
	/** \brief Tree item text. */
	inline const decString &GetText() const{ return pText; }
	
	/**
	 * \brief Set tree item text.
	 * 
	 * If tree item is assigned updates tree item text.
	 */
	void SetText( const char *text );
	
	/** \brief Tree item description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/**
	 * \brief Set tree item description.
	 * 
	 * If tree item is assigned updates tree item description.
	 */
	void SetDescription( const char *description );
	
	/** \brief Tree item icon. */
	inline igdeIcon *GetIcon() const{ return pIcon; }
	
	/**
	 * \brief Set tree item icon.
	 * 
	 * If tree item is assigned updates tree item icons.
	 */
	void SetIcon( igdeIcon *icon );
	
	/** \brief Expanded. */
	inline bool GetExpanded() const{ return pExpanded; }
	
	/** \brief Set expanded. */
	void SetExpanded( bool expanded );
	
	
	
	/** \brief Number of children. */
	int GetChildCount() const;
	
	/** \brief Child at index. */
	ceWPTTreeItemModel *GetChildAt( int index ) const;
	
	/** \brief Add child. */
	void AddChild( ceWPTTreeItemModel *child );
	
	/** \brief Insert child at position. */
	void InsertChild( ceWPTTreeItemModel *child, int position );
	
	/** \brief Remove child. */
	void RemoveChild( ceWPTTreeItemModel *child );
	
	/** \brief Remove all children. */
	void RemoveAllChildren();
	
	/**
	 * \brief Move child before or after another child.
	 */
	void MoveChild( ceWPTTreeItemModel *child, int to );
	
	/**
	 * \brief Move child before or after another child.
	 */
	void MoveChild( int from, int to );
	
	
	
	/** \brief Tree or \em NULL if not top level. */
	inline ceWPTTreeModel *GetTree() const{ return pTree; }
	
	/** \brief Set tree or \em NULL if not top level. */
	void SetTree( ceWPTTreeModel *tree );
	
	/** \brief First non-NULL tree found while traveling up parents or \em NULL if not found. */
	ceWPTTreeModel *GetFirstTree() const;
	
	/** \brief Parent or \em NULL. */
	inline ceWPTTreeItemModel *GetParent() const{ return pParent; }
	
	/** \brief Set parent or \em NULL. */
	void SetParent( ceWPTTreeItemModel *parent );
	
	/** \brief Assigned tree item or \em NULL. */
	inline ceWPTTreeItem *GetTreeItem() const{ return pTreeItem; }
	
	/**
	 * \brief Assign tree item or \em NULL.
	 * 
	 * If tree item is not \em NULL fully updates tree item with stored data.
	 */
	void SetTreeItem( ceWPTTreeItem *treeItem );
	
	/** \brief Sort children. */
	void SortChildren();
	
	/** \brief Sort parent item. */
	void ParentSortItems();
	
	/** \brief Set item as current item and make it visible. */
	void SetAsCurrentItem();
	
	
	
	/** \brief User selected item. */
	virtual void OnSelected();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	
	/** \brief User requests context menu for selected child action. */
	virtual void ContextMenuAction( igdeMenuCascade &contextMenu, ceConversationAction *action );
	
	/** \brief User requests context menu for selected child condition. */
	virtual void ContextMenuCondition( igdeMenuCascade &contextMenu, ceConversationCondition *condition );
	
	/** \brief Deep find action. */
	virtual ceWPTTIMAction *DeepFindAction( ceConversationAction *action );
	
	/** \brief Deep find condition. */
	virtual ceWPTTIMCondition *DeepFindCondition( ceConversationCondition *condition );
	
	/** \brief Expanded state changed. */
	virtual void OnExpandedChanged();
	
	/** \brief Get action owning this model if any. */
	virtual ceConversationAction *GetOwnerAction() const;
	
	/** \brief Get condition owning this model if any. */
	virtual ceConversationCondition *GetOwnerCondition() const;
	
	/** \brief Build playback continuing from here. */
	virtual void BuildPlaybackFromHere() const;
	/*@}*/
};

#endif
