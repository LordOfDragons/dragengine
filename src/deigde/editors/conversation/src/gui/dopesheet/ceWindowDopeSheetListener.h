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
	~ceWindowDopeSheetListener() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Conversation changed. */
	void ConversationChanged(ceConversation *conversation) override;
	
	/** \brief Camera shot changed. */
	void CameraShotChanged(ceConversation *conversation, ceCameraShot *cameraShot) override;
	
	/** \brief Active camera shot changed. */
	void ActiveCameraShotChanged(ceConversation *conversation) override;
	
	
	
	/** \brief File count or order changed. */
	void FileStructureChanged(ceConversation *conversation) override;
	
	/** \brief Active file changed. */
	void ActiveFileChanged(ceConversation *conversation) override;
	
	
	
	/** \brief Topic count or order changed. */
	void TopicStructureChanged(ceConversation *conversation, ceConversationFile *file) override;
	
	/** \brief Active topic changed. */
	void ActiveTopicChanged(ceConversation *conversation, ceConversationFile *file) override;
	
	
	
	/** \brief Action changed. */
	void ActionChanged(ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action) override;
	
	/** \brief Active element changed. */
	void ActiveChanged(ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic) override;
	
	
	
	/** \brief Playback parameters changed. */
	void PlaybackChanged(ceConversation *conversation) override;
	
	/** \brief Playback action stack changed. */
	void PlaybackActionStackChanged(ceConversation *conversation) override;
	/*@}*/
};

#endif
