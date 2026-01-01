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

#ifndef _CEWPTTIMACTIONS_H_
#define _CEWPTTIMACTIONS_H_

#include "../ceWPTTreeItemModel.h"
#include "../../../../conversation/action/ceConversationAction.h"

class ceWPTTIMAction;


/**
 * \brief Actions tree model.
 */
class ceWPTTIMActions : public ceWPTTreeItemModel{
public:
	typedef deTObjectReference<ceWPTTIMActions> Ref;
	
private:
	const ceConversationAction::List &pActions;
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMActions(ceWindowMain &windowMain, ceConversation &conversation,
		eTypes type, const ceConversationAction::List &actions);
	
protected:
	/** \brief Clean up tree item model. */
	~ceWPTTIMActions() override;
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Model with action or \em nullptr. */
	ceWPTTIMAction *GetChildWith(ceConversationAction *action) const;
	
	/** \brief Structure changed. */
	void StructureChanged();
	
	
	
	/** \brief Update action. */
	virtual void Update();
	
	/** \brief Update actions. */
	virtual void UpdateActions();
	
	/** \brief Build playback continuing from here. */
	void BuildPlaybackFromHere() const override;
	
	
	
	/** \brief Create action model. */
	static ceWPTTIMAction::Ref CreateActionModel(ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationAction *action);
	/*@}*/
};

#endif
