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

#ifndef _CEWPTTREEMODEL_H_
#define _CEWPTTREEMODEL_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceConversation;
class ceConversationAction;
class ceConversationCondition;
class ceWPTTIMAction;
class ceWPTTIMCondition;
class ceWPTTreeModelListener;
class ceWPTTreeItem;
class ceWPTTreeItemModel;
class ceWindowMain;
class ceConversationListener;

class igdeTreeList;
class igdeMenuCascade;


/**
 * Tree model.
 * 
 * Tree model is responsible to update the visual state and content of assigned tree list.
 */
class ceWPTTreeModel{
public:
	class PreventUpdateGuard{
		ceWPTTreeModel &pModel;
		bool pPrevPreventUpdate;
		
	public:
		PreventUpdateGuard( ceWPTTreeModel &model );
		~PreventUpdateGuard();
	};
	
	friend PreventUpdateGuard;
	
	
	
private:
	ceWindowMain &pWindowMain;
	ceConversation *pConversation;
	ceWPTTreeModelListener *pListener;
	ceConversationListener &pForwardListener;
	
	decObjectOrderedSet pChildren;
	
	igdeTreeList *pTreeList; // weak reference
	bool pPreventUpdate;
	
	
	
public:
	/** Constructors and Destructors */
	/*@{*/
	/** Create new tree model. */
	ceWPTTreeModel( ceWindowMain &windowMain, ceConversation *conversation,
		ceConversationListener &forwardListener );
	
	/** Clean up tree model. */
	~ceWPTTreeModel();
	/*@}*/
	
	
	
public:
	/** Management */
	/*@{*/
	/** Window main. */
	inline ceWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Game definition. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	
	
	
	/** Number of children. */
	int GetChildCount() const;
	
	/** Child at index. */
	ceWPTTreeItemModel *GetChildAt( int index ) const;
	
	/** Add child. */
	void AddChild( ceWPTTreeItemModel *child );
	
	/** Insert child at position. */
	void InsertChild( ceWPTTreeItemModel *child, int position );
	
	/** Remove child. */
	void RemoveChild( ceWPTTreeItemModel *child );
	
	/** Remove all children. */
	void RemoveAllChildren();
	
	/**
	 * Move child before or after another child.
	 */
	void MoveChild( ceWPTTreeItemModel *child, int to );
	
	/**
	 * Move child before or after another child.
	 */
	void MoveChild( int from, int to );
	
	
	
	/** Child with action or \em NULL. */
	ceWPTTIMAction *GetChildWith( ceConversationAction *action ) const;
	
	
	
	/** Update actions. */
	void UpdateActions();
	
	
	/** Assigned tree list or \em NULL. */
	inline igdeTreeList *GetTreeList() const{ return pTreeList; }
	
	/**
	 * Assign tree list or \em NULL.
	 * 
	 * If tree list is not \em NULL fully updates tree with stored data.
	 */
	void SetTreeList( igdeTreeList *treeList );
	
	
	
	/** User requests context menu for selected item. */
	void OnContextMenu( igdeMenuCascade &contextMenu );
	
	/** User requests context menu for selected child action. */
	void ContextMenuAction( igdeMenuCascade &contextMenu, ceConversationAction *action );
	
	/** User requests context menu for topic specifi actions. */
	void ContextMenuTopic( igdeMenuCascade &contextMenu );
	
	/** Deep find action. */
	ceWPTTIMAction *DeepFindAction( ceConversationAction *action ) const;
	
	/** Deep find condition. */
	ceWPTTIMCondition *DeepFindCondition( ceConversationCondition *condition ) const;
	
	/** Select topic active element. */
	void SelectTopicActive();
	
	/** Prevent update. */
	inline bool GetPreventUpdate() const{ return pPreventUpdate; }
	
	/** Forward listener. */
	inline ceConversationListener &GetForwardListener(){ return pForwardListener; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
