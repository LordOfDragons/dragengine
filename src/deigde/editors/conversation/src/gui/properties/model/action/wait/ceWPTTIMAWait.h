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

#ifndef _CEWPTTIMAWAIT_H_
#define _CEWPTTIMAWAIT_H_

#include "../ceWPTTIMAction.h"
#include "../../../../../conversation/action/ceCAWait.h"

class ceWPTTIMAWaitCondition;
class ceWPTTIMAWaitActions;


/**
 * \brief Action if-else.
 */
class ceWPTTIMAWait : public ceWPTTIMAction{
private:
	ceWPTTIMAWaitCondition *pCondition;
	ceWPTTIMAWaitActions *pActions;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMAWait(ceWindowMain &windowMain, ceConversation &conversation, ceCAWait *action);
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMAWait();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Action. */
	inline ceCAWait *GetActionWait() const{ return (ceCAWait*)GetAction(); }
	
	/** \brief Model with condition. */
	inline ceWPTTIMAWaitCondition *GetChildCondition() const{ return pCondition; }
	
	/** \brief Model with actions. */
	inline ceWPTTIMAWaitActions *GetChildActions() const{ return pActions; }
	
	/** \brief Update action. */
	virtual void Update();
	
	/** \brief Update action lists if present. */
	virtual void UpdateActionLists();
	
	/** \brief Expanded state changed. */
	virtual void OnExpandedChanged();
	/*@}*/
};

#endif
