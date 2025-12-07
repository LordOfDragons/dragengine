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

#ifndef _CEWPCONVERSATIONLISTENER_H_
#define _CEWPCONVERSATIONLISTENER_H_

#include "../../conversation/ceConversationListener.h"

class ceWPConversation;



/**
 * \brief Conversation panel listener.
 */
class ceWPConversationListener : public ceConversationListener{
private:
	ceWPConversation &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new listener. */
	ceWPConversationListener(ceWPConversation &panel);
	/** Cleans up the listener. */
	~ceWPConversationListener() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Conversation changed. */
	void ConversationChanged(ceConversation *conversation) override;
	
	/** Target count or order changed. */
	void TargetStructureChanged(ceConversation *conversation) override;
	/** Target changed. */
	void TargetChanged(ceConversation *conversation, ceTarget *target) override;
	/** Active target changed. */
	void ActiveTargetChanged(ceConversation *conversation) override;
	
	/** Camera shot count or order changed. */
	void CameraShotStructureChanged(ceConversation *conversation) override;
	/** Camera shot changed. */
	void CameraShotChanged(ceConversation *conversation, ceCameraShot *cameraShot) override;
	/** Active camera shot changed. */
	void ActiveCameraShotChanged(ceConversation *conversation) override;
	
	/** Gesture count or order changed. */
	void GestureStructureChanged(ceConversation *conversation) override;
	/** Gesture changed. */
	void GestureChanged(ceConversation *conversation, ceGesture *gesture) override;
	/** Active gesture changed. */
	void ActiveGestureChanged(ceConversation *conversation) override;
	
	/** Face pose controller names changed. */
	void FacePoseControllerNamesChanged(ceConversation *conversation) override;
	/** Face pose count or order changed. */
	void FacePoseStructureChanged(ceConversation *conversation) override;
	/** Face pose changed. */
	void FacePoseChanged(ceConversation *conversation, ceFacePose *facePose) override;
	/** Active face pose changed. */
	void ActiveFacePoseChanged(ceConversation *conversation) override;
	
	/** Actor count or order changed. */
	void ActorStructureChanged(ceConversation *conversation) override;
	/** Actor changed. */
	void ActorChanged(ceConversation *conversation, ceConversationActor *actor) override;
	/*@}*/
};

#endif
