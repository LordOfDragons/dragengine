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

#ifndef _CEPLAYBACKEVALUATECONDITION_H_
#define _CEPLAYBACKEVALUATECONDITION_H_

class ceCConditionTrigger;
class cePlayback;
class ceConversation;
class ceConversationCondition;
class ceCConditionHasActor;
class ceCConditionLogic;
class ceCConditionVariable;
class ceCConditionActorParameter;
class ceCConditionActorCommand;
class ceCConditionGameCommand;
class ceCConditionActorInConversation;



/**
 * \brief Conversation playback evaluate condition.
 */
class cePlaybackEvaluateCondition{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create evaluate condition object. */
	cePlaybackEvaluateCondition();
	
	/** \brief Clean up evaluate condition object. */
	~cePlaybackEvaluateCondition();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Evaluate condition. */
	bool EvaluateCondition(ceConversation &conversation,
		const ceConversationCondition &condition);
	
	/** \brief Evaluate logic condition. */
	bool EvaluateLogic(ceConversation &conversation,
		const ceCConditionLogic &condition);
	
	/** \brief Evaluate has actor condition. */
	bool EvaluateHasActor(ceConversation &conversation,
		const ceCConditionHasActor &condition);
	
	/** \brief Evaluate actor in conversation. */
	bool EvaluateActorInConversation(ceConversation &conversation,
		const ceCConditionActorInConversation &condition);
	
	/** \brief Evaluate variable condition. */
	bool EvaluateVariable(ceConversation &conversation,
		const ceCConditionVariable &condition);
	
	/** \brief Evaluate actor parameter condition. */
	bool EvaluateActorParameter(ceConversation &conversation,
		const ceCConditionActorParameter &condition);
	
	/** \brief Evaluate actor command condition. */
	bool EvaluateActorCommand(ceConversation &conversation,
		const ceCConditionActorCommand &condition);
	
	/** \brief Evaluate game command condition. */
	bool EvaluateGameCommand(ceConversation &conversation,
		const ceCConditionGameCommand &condition);
	
	/** \brief Evaluate trigger condition. */
	bool EvaluateTrigger(ceConversation &conversation,
		const ceCConditionTrigger &condition);
	/*@}*/
};

#endif
