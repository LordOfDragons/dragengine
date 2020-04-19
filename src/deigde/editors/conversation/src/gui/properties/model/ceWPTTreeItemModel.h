/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CEWPTTREEITEMMODEL_H_
#define _CEWPTTREEITEMMODEL_H_

#include <deigde/gui/resources/igdeIconReference.h>

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
	
	decString pText;
	igdeIconReference pIcon;
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
