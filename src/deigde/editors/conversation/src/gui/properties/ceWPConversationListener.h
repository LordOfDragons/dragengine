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
	virtual ~ceWPConversationListener();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Conversation changed. */
	virtual void ConversationChanged(ceConversation *conversation);
	
	/** Target count or order changed. */
	virtual void TargetStructureChanged(ceConversation *conversation);
	/** Target changed. */
	virtual void TargetChanged(ceConversation *conversation, ceTarget *target);
	/** Active target changed. */
	virtual void ActiveTargetChanged(ceConversation *conversation);
	
	/** Camera shot count or order changed. */
	virtual void CameraShotStructureChanged(ceConversation *conversation);
	/** Camera shot changed. */
	virtual void CameraShotChanged(ceConversation *conversation, ceCameraShot *cameraShot);
	/** Active camera shot changed. */
	virtual void ActiveCameraShotChanged(ceConversation *conversation);
	
	/** Gesture count or order changed. */
	virtual void GestureStructureChanged(ceConversation *conversation);
	/** Gesture changed. */
	virtual void GestureChanged(ceConversation *conversation, ceGesture *gesture);
	/** Active gesture changed. */
	virtual void ActiveGestureChanged(ceConversation *conversation);
	
	/** Face pose controller names changed. */
	virtual void FacePoseControllerNamesChanged(ceConversation *conversation);
	/** Face pose count or order changed. */
	virtual void FacePoseStructureChanged(ceConversation *conversation);
	/** Face pose changed. */
	virtual void FacePoseChanged(ceConversation *conversation, ceFacePose *facePose);
	/** Active face pose changed. */
	virtual void ActiveFacePoseChanged(ceConversation *conversation);
	
	/** Actor count or order changed. */
	virtual void ActorStructureChanged(ceConversation *conversation);
	/** Actor changed. */
	virtual void ActorChanged(ceConversation *conversation, ceConversationActor *actor);
	/*@}*/
};

#endif
