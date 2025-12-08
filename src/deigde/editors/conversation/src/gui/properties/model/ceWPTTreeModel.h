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
		PreventUpdateGuard(ceWPTTreeModel &model);
		~PreventUpdateGuard();
	};
	
	friend PreventUpdateGuard;
	
	
	
private:
	ceWindowMain &pWindowMain;
	ceConversation::Ref pConversation;
	ceWPTTreeModelListener *pListener;
	ceConversationListener &pForwardListener;
	
	decObjectOrderedSet pChildren;
	
	igdeTreeList *pTreeList; // weak reference
	bool pPreventUpdate;
	
	
	
public:
	/** Constructors and Destructors */
	/*@{*/
	/** Create new tree model. */
	ceWPTTreeModel(ceWindowMain &windowMain, ceConversation *conversation,
		ceConversationListener &forwardListener);
	
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
	ceWPTTreeItemModel *GetChildAt(int index) const;
	
	/** Add child. */
	void AddChild(ceWPTTreeItemModel *child);
	
	/** Insert child at position. */
	void InsertChild(ceWPTTreeItemModel *child, int position);
	
	/** Remove child. */
	void RemoveChild(ceWPTTreeItemModel *child);
	
	/** Remove all children. */
	void RemoveAllChildren();
	
	/**
	 * Move child before or after another child.
	 */
	void MoveChild(ceWPTTreeItemModel *child, int to);
	
	/**
	 * Move child before or after another child.
	 */
	void MoveChild(int from, int to);
	
	
	
	/** Child with action or \em NULL. */
	ceWPTTIMAction *GetChildWith(ceConversationAction *action) const;
	
	
	
	/** Update actions. */
	void UpdateActions();
	
	
	/** Assigned tree list or \em NULL. */
	inline igdeTreeList *GetTreeList() const{ return pTreeList; }
	
	/**
	 * Assign tree list or \em NULL.
	 * 
	 * If tree list is not \em NULL fully updates tree with stored data.
	 */
	void SetTreeList(igdeTreeList *treeList);
	
	
	
	/** User requests context menu for selected item. */
	void OnContextMenu(igdeMenuCascade &contextMenu);
	
	/** User requests context menu for selected child action. */
	void ContextMenuAction(igdeMenuCascade &contextMenu, ceConversationAction *action);
	
	/** User requests context menu for topic specifi actions. */
	void ContextMenuTopic(igdeMenuCascade &contextMenu);
	
	/** Deep find action. */
	ceWPTTIMAction *DeepFindAction(ceConversationAction *action) const;
	
	/** Deep find condition. */
	ceWPTTIMCondition *DeepFindCondition(ceConversationCondition *condition) const;
	
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
