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

#ifndef _CEWPTTIMCHASACTOR_H_
#define _CEWPTTIMCHASACTOR_H_

#include "ceWPTTIMCondition.h"
#include "../../../../conversation/condition/ceCConditionHasActor.h"


/**
 * \brief Condition has actor.
 */
class ceWPTTIMCHasActor : public ceWPTTIMCondition{
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMCHasActor( ceWindowMain &windowMain, ceConversation &conversation,
		ceConversationAction &action, ceCConditionHasActor *condition );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMCHasActor();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Condition. */
	inline ceCConditionHasActor *GetConditionHasActor() const{
		return ( ceCConditionHasActor* )GetCondition();
	}
	
	/** \brief Update condition. */
	virtual void Update();
	/*@}*/
};

#endif
