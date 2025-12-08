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

#ifndef _CEWPTTIMACTION_H_
#define _CEWPTTIMACTION_H_

#include "../ceWPTTreeItemModel.h"
#include "../../../../conversation/action/ceConversationAction.h"


/**
 * \brief Base class for actions.
 */
class ceWPTTIMAction : public ceWPTTreeItemModel{
private:
	ceConversationAction::Ref pAction;
	
public:
	static const ceConversationAction::eActionTypes ListAddMenuActions[19];
	static const int ListAddMenuActionsCount;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMAction(ceWindowMain &windowMain, eTypes type,
		ceConversation &conversation, ceConversationAction *action);
	
protected:
	/** \brief Clean up tree item model. */
	~ceWPTTIMAction() override;
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Action. */
	inline const ceConversationAction::Ref &GetAction() const{ return pAction; }
	
	/** \brief Deep find action. */
	ceWPTTIMAction *DeepFindAction(ceConversationAction *action) override;
	
	/** \brief User requests context menu for selected item. */
	void OnContextMenu(igdeMenuCascade &contextMenu) override;
	
	/** \brief Update action. */
	virtual void Update();
	
	/** \brief Update action lists if present. */
	virtual void UpdateActionLists();
	
	/** \brief Get action owning this model if any. */
	ceConversationAction *GetOwnerAction() const override;
	
	/** \brief Build playback continuing from here. */
	void BuildPlaybackFromHere() const override;
	/*@}*/
};

#endif
