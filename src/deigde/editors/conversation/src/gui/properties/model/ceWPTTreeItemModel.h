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
#include <dragengine/common/collection/decTOrderedSet.h>

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
 * Base class for tree item models.
 * 
 * Tree item models are responsible to update the visual state and content
 * of an assigned tree item.
 */
class ceWPTTreeItemModel : public deObject{
public:
	typedef deTObjectReference<ceWPTTreeItemModel> Ref;
	typedef decTObjectOrderedSet<ceWPTTreeItemModel> ModelList;
	
	
	/** Tree item types. */
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
	ModelList pChildren;
	bool pExpanded;
	
	ceWPTTreeModel *pTree; // weak reference;
	ceWPTTreeItemModel *pParent; // weak reference
	ceWPTTreeItem *pTreeItem; // weak reference
	
	
	
public:
	/** Constructors and Destructors */
	/*@{*/
	/** Crete new tree item model. */
	ceWPTTreeItemModel(ceWindowMain &windowMain, ceConversation &conversation, eTypes type);
	
protected:
	/** Clean up tree item model. */
	virtual ~ceWPTTreeItemModel();
	/*@}*/
	
	
	
public:
	/** Management */
	/*@{*/
	/** Window selection. */
	inline ceWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Game definition. */
	inline ceConversation &GetConversation() const{ return pConversation; }
	
	/** Tree item type. */
	inline eTypes GetType() const{ return pType; }
	
	
	
	/** Tree item text. */
	inline const decString &GetText() const{ return pText; }
	
	/**
	 * Set tree item text.
	 * 
	 * If tree item is assigned updates tree item text.
	 */
	void SetText(const char *text);
	
	/** Tree item description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/**
	 * Set tree item description.
	 * 
	 * If tree item is assigned updates tree item description.
	 */
	void SetDescription(const char *description);
	
	/** Tree item icon. */
	inline const igdeIcon::Ref &GetIcon() const{ return pIcon; }
	
	/**
	 * Set tree item icon.
	 * 
	 * If tree item is assigned updates tree item icons.
	 */
	void SetIcon(igdeIcon *icon);
	
	/** Expanded. */
	inline bool GetExpanded() const{ return pExpanded; }
	
	/** Set expanded. */
	void SetExpanded(bool expanded);
	
	
	
	/** Children. */
	inline const ModelList &GetChildren() const{ return pChildren; }
	
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
	
	
	
	/** Tree or \em nullptr if not top level. */
	inline ceWPTTreeModel *GetTree() const{ return pTree; }
	
	/** Set tree or \em nullptr if not top level. */
	void SetTree(ceWPTTreeModel *tree);
	
	/** First non-nullptr tree found while traveling up parents or \em nullptr if not found. */
	ceWPTTreeModel *GetFirstTree() const;
	
	/** Parent or \em nullptr. */
	inline ceWPTTreeItemModel *GetParent() const{ return pParent; }
	
	/** Set parent or \em nullptr. */
	void SetParent(ceWPTTreeItemModel *parent);
	
	/** Assigned tree item or \em nullptr. */
	inline ceWPTTreeItem *GetTreeItem() const{ return pTreeItem; }
	
	/**
	 * Assign tree item or \em nullptr.
	 * 
	 * If tree item is not \em nullptr fully updates tree item with stored data.
	 */
	void SetTreeItem(ceWPTTreeItem *treeItem);
	
	/** Sort children. */
	void SortChildren();
	
	/** Sort parent item. */
	void ParentSortItems();
	
	/** Set item as current item and make it visible. */
	void SetAsCurrentItem();
	
	
	
	/** User selected item. */
	virtual void OnSelected();
	
	/** User requests context menu for selected item. */
	virtual void OnContextMenu(igdeMenuCascade &contextMenu);
	
	/** User requests context menu for selected child action. */
	virtual void ContextMenuAction(igdeMenuCascade &contextMenu, ceConversationAction *action);
	
	/** User requests context menu for selected child condition. */
	virtual void ContextMenuCondition(igdeMenuCascade &contextMenu, ceConversationCondition *condition);
	
	/** Deep find action. */
	virtual ceWPTTIMAction *DeepFindAction(ceConversationAction *action);
	
	/** Deep find condition. */
	virtual ceWPTTIMCondition *DeepFindCondition(ceConversationCondition *condition);
	
	/** Expanded state changed. */
	virtual void OnExpandedChanged();
	
	/** Get action owning this model if any. */
	virtual ceConversationAction *GetOwnerAction() const;
	
	/** Get condition owning this model if any. */
	virtual ceConversationCondition *GetOwnerCondition() const;
	
	/** Build playback continuing from here. */
	virtual void BuildPlaybackFromHere() const;
	/*@}*/
};

#endif
