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

#ifndef _CECONVERSATIONCONDITION_H_
#define _CECONVERSATIONCONDITION_H_

#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/collection/decTSet.h>
#include <dragengine/deObject.h>



/**
 * \brief Conversation condition.
 */
class ceConversationCondition : public deObject{
public:
	typedef deTObjectReference<ceConversationCondition> Ref;
	typedef decTObjectOrderedSet<ceConversationCondition> List;
	typedef decTSet<int> IndexSet;
	
	
	/** \brief Condition types. */
	enum eConditionTypes{
		/** \brief Logic operator. */
		ectLogic,
		
		/** \brief Actor is present in the conversation. */
		ectHasActor,
		
		/** \brief Actor is in a conversation. */
		ectActorInConversation,
		
		/** \brief Compare variable against value. */
		ectVariable,
		
		/** \brief Compare actor parameter against value. */
		ectActorParameter,
		
		/** \brief Send condition command an actor. */
		ectActorCommand,
		
		/** \brief Send condition command to game. */
		ectGameCommand,
		
		/** \brief Trigger state. */
		ectTrigger
	};
	
	
	
private:
	eConditionTypes pType;
	IndexSet pTreeExpandedLists;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create conversation condition. */
	ceConversationCondition(eConditionTypes type);
	
	/** \brief Create copy of conversation condition. */
	ceConversationCondition(const ceConversationCondition &condition);
	
	/** \brief Clean up conversation action. */
	virtual ~ceConversationCondition();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Type. */
	inline eConditionTypes GetType() const{ return pType; }
	
	/** \brief List of expanded lists in a tree view. */
	inline IndexSet &GetTreeExpandedLists(){ return pTreeExpandedLists; }
	inline const IndexSet &GetTreeExpandedLists() const{ return pTreeExpandedLists; }
	
	/** \brief Create copy of condition. */
	virtual Ref CreateCopy() const = 0;
	/*@}*/
};

#endif
