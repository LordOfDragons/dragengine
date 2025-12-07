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

#ifndef _CEWINDOWDOPESHEETLISTENER_H_
#define _CEWINDOWDOPESHEETLISTENER_H_

#include "../../conversation/ceConversationListener.h"

class ceWindowDopeSheet;



/**
 * \brief Dope sheet window listener.
 */
class ceWindowDopeSheetListener : public ceConversationListener{
private:
	ceWindowDopeSheet &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	ceWindowDopeSheetListener(ceWindowDopeSheet &window);
	
	/** \brief Clean up listener. */
	virtual ~ceWindowDopeSheetListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Conversation changed. */
	virtual void ConversationChanged(ceConversation *conversation);
	
	/** \brief Camera shot changed. */
	virtual void CameraShotChanged(ceConversation *conversation, ceCameraShot *cameraShot);
	
	/** \brief Active camera shot changed. */
	virtual void ActiveCameraShotChanged(ceConversation *conversation);
	
	
	
	/** \brief File count or order changed. */
	virtual void FileStructureChanged(ceConversation *conversation);
	
	/** \brief Active file changed. */
	virtual void ActiveFileChanged(ceConversation *conversation);
	
	
	
	/** \brief Topic count or order changed. */
	virtual void TopicStructureChanged(ceConversation *conversation, ceConversationFile *file);
	
	/** \brief Active topic changed. */
	virtual void ActiveTopicChanged(ceConversation *conversation, ceConversationFile *file);
	
	
	
	/** \brief Action changed. */
	virtual void ActionChanged(ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action);
	
	/** \brief Active element changed. */
	virtual void ActiveChanged(ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic);
	
	
	
	/** \brief Playback parameters changed. */
	virtual void PlaybackChanged(ceConversation *conversation);
	
	/** \brief Playback action stack changed. */
	virtual void PlaybackActionStackChanged(ceConversation *conversation);
	/*@}*/
};

#endif
