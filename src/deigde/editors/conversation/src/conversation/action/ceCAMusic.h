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

#ifndef _CECAMUSIC_H_
#define _CECAMUSIC_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Music Conversation Action.
 * Sets the music to play. Music is defined globally in the conversation with a unique name.
 */
class ceCAMusic : public ceConversationAction{
private:
	decString pName;
	
public:
	typedef deTObjectReference<ceCAMusic> Ref;
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new music conversation action. */
	ceCAMusic();
	/** Creates a new music conversation action. */
	ceCAMusic(const ceCAMusic &action);
	/** Cleans up the music conversation action. */
protected:
	~ceCAMusic() override;
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the name of the music to use. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name of the music to use. */
	void SetName(const char *name);
	
	/** Create a copy of this action. */
    ceConversationAction::Ref CreateCopy() const override;
	/*@}*/
};

#endif
