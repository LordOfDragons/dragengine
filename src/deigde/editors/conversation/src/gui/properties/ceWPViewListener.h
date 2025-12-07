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

#ifndef _CEWPVIEWLISTENER_H_
#define _CEWPVIEWLISTENER_H_

#include "../../conversation/ceConversationListener.h"

class ceWPView;



/**
 * \brief View panel listener.
 */
class ceWPViewListener : public ceConversationListener{
private:
	ceWPView &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	ceWPViewListener(ceWPView &panel);
	
	/** \brief Clean up listener. */
	virtual ~ceWPViewListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Conversation changed. */
	virtual void ConversationChanged(ceConversation *conversation);
	
	/** \brief The sky changed. */
	virtual void SkyChanged(ceConversation *conversation);
	
	/** \brief The environment component changed. */
	virtual void EnvObjectChanged(ceConversation *conversation);
	
	/** \brief Camera changed. */
	virtual void CameraChanged(ceConversation *conversation);
	
	/** \brief File count or order changed. */
	virtual void FileStructureChanged(ceConversation *conversation);
	
	/** \brief File changed. */
	virtual void FileChanged(ceConversation *conversation, ceConversationFile *file);
	
	/** \brief Topic count or order changed. */
	virtual void TopicStructureChanged(ceConversation *conversation, ceConversationFile *file);
	
	/** \brief Topic changed. */
	virtual void TopicChanged(ceConversation *conversation, ceConversationFile *file, ceConversationTopic *topic);
	
	
	
	/** \brief Actor count or order changed. */
	virtual void ActorStructureChanged(ceConversation *conversation);
	
	/** \brief Actor changed. */
	virtual void ActorChanged(ceConversation *conversation, ceConversationActor *actor);
	
	/** \brief Actor active pose changed. */
	virtual void ActorActivePoseChanged(ceConversation *conversation, ceConversationActor *actor);
	
	/** \brief Actor poses changed. */
	virtual void ActorPosesChanged(ceConversation *conversation, ceConversationActor *actor);
	
	/** \brief Actor active pose gestures changed. */
	virtual void ActorPoseGesturesChanged(ceConversation *conversation,
		ceConversationActor *actor, ceActorPose *pose);
	
	/** \brief Actor commands changed. */
	virtual void ActorCommandsChanged(ceConversation *conversation, ceConversationActor *actor);
	
	/** \brief Actor parameters changed. */
	virtual void ActorParametersChanged(ceConversation *conversation, ceConversationActor *actor);
	
	/** \brief Active actor changed. */
	virtual void ActiveActorChanged(ceConversation *conversation);
	
	
	
	/** \brief Coordinate system count or order changed. */
	virtual void CoordSystemStructureChanged(ceConversation *conversation);
	
	/** \brief Coordinate system changed. */
	virtual void CoordSystemChanged(ceConversation *conversation, ceCoordSystem *coordSystem);
	
	/** \brief Active coordinate system changed. */
	virtual void ActiveCoordSystemChanged(ceConversation *conversation);
	
	/** \brief Prop count or order changed. */
	virtual void PropStructureChanged(ceConversation *conversation);
	
	/** \brief Prop changed. */
	virtual void PropChanged(ceConversation *conversation, ceProp *prop);
	
	/** \brief Active prop changed. */
	virtual void ActivePropChanged(ceConversation *conversation);
	
	/** \brief Playback parameters changed. */
	virtual void PlaybackChanged(ceConversation *conversation);
	
	/** \brief Playback info list changed. */
	virtual void PlaybackCommandListChanged(ceConversation *conversation);
	
	/** \brief Playback variable list changed. */
	virtual void PlaybackVarListChanged(ceConversation *conversation);
	
	/** \brief Playback trigger table changed. */
	virtual void PlaybackTriggerTableChanged(ceConversation *conversation);
	
	/** \brief Playback missing words changed. */
	virtual void PlaybackMissingWordsChanged(ceConversation *conversation);
	/*@}*/
};

#endif
