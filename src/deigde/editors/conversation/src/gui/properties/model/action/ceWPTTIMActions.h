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

class ceWPTTIMAction;
class ceConversationAction;
class ceConversationActionList;


/**
 * \brief Actions tree model.
 */
class ceWPTTIMActions : public ceWPTTreeItemModel{
private:
	const ceConversationActionList &pActions;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMActions( ceWindowMain &windowMain, ceConversation &conversation,
		eTypes type, const ceConversationActionList &actions );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMActions();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Model with action or \em NULL. */
	ceWPTTIMAction *GetChildWith( ceConversationAction *action ) const;
	
	/** \brief Structure changed. */
	void StructureChanged();
	
	
	
	/** \brief Update action. */
	virtual void Update();
	
	/** \brief Update actions. */
	virtual void UpdateActions();
	
	/** \brief Build playback continuing from here. */
	virtual void BuildPlaybackFromHere() const;
	
	
	
	/** \brief Create action model. */
	static ceWPTTIMAction *CreateActionModel( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationAction *action );
	/*@}*/
};

#endif
