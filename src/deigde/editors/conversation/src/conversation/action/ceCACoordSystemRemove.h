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

#ifndef _CECACOORDSYSTEMREMOVE_H_
#define _CECACOORDSYSTEMREMOVE_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Remove coordinate system conversation action.
 * \details Removes a coordinate system from the conversation if existing.
 */
class ceCACoordSystemRemove : public ceConversationAction{
private:
	decString pCoordSystemID;
	
public:
	typedef deTObjectReference<ceCACoordSystemRemove> Ref;
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new conversation action. */
	ceCACoordSystemRemove();
	/** \brief Creates a new conversation action. */
	ceCACoordSystemRemove(const ceCACoordSystemRemove &action);
	/** \brief Cleans up the conversation action. */
protected:
	~ceCACoordSystemRemove() override;
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the coordinate system id. */
	inline const decString &GetCoordSystemID() const{ return pCoordSystemID; }
	/** \brief Sets the coordinate system id. */
	void SetCoordSystemID(const char *id);
	
	/** \brief Create a copy of this action. */
    ceConversationAction::Ref CreateCopy() const override;
	/*@}*/
};

#endif
