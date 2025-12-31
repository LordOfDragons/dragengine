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
public:
	typedef deTObjectReference<ceWPViewListener> Ref;
	
private:
	ceWPView &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	ceWPViewListener(ceWPView &panel);
	
	/** \brief Clean up listener. */
protected:
	~ceWPViewListener() override;
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Conversation changed. */
	void ConversationChanged(ceConversation *conversation) override;
	
	/** \brief The sky changed. */
	void SkyChanged(ceConversation *conversation) override;
	
	/** \brief The environment component changed. */
	void EnvObjectChanged(ceConversation *conversation) override;
	
	/** \brief Camera changed. */
	void CameraChanged(ceConversation *conversation) override;
	
	/** \brief File count or order changed. */
	void FileStructureChanged(ceConversation *conversation) override;
	
	/** \brief File changed. */
	void FileChanged(ceConversation *conversation, ceConversationFile *file) override;
	
	/** \brief Topic count or order changed. */
	void TopicStructureChanged(ceConversation *conversation, ceConversationFile *file) override;
	
	/** \brief Topic changed. */
	void TopicChanged(ceConversation *conversation, ceConversationFile *file, ceConversationTopic *topic) override;
	
	
	
	/** \brief Actor count or order changed. */
	void ActorStructureChanged(ceConversation *conversation) override;
	
	/** \brief Actor changed. */
	void ActorChanged(ceConversation *conversation, ceConversationActor *actor) override;
	
	/** \brief Actor active pose changed. */
	void ActorActivePoseChanged(ceConversation *conversation, ceConversationActor *actor) override;
	
	/** \brief Actor poses changed. */
	void ActorPosesChanged(ceConversation *conversation, ceConversationActor *actor) override;
	
	/** \brief Actor active pose gestures changed. */
	void ActorPoseGesturesChanged(ceConversation *conversation,
		ceConversationActor *actor, ceActorPose *pose) override;
	
	/** \brief Actor commands changed. */
	void ActorCommandsChanged(ceConversation *conversation, ceConversationActor *actor) override;
	
	/** \brief Actor parameters changed. */
	void ActorParametersChanged(ceConversation *conversation, ceConversationActor *actor) override;
	
	/** \brief Active actor changed. */
	void ActiveActorChanged(ceConversation *conversation) override;
	
	
	
	/** \brief Coordinate system count or order changed. */
	void CoordSystemStructureChanged(ceConversation *conversation) override;
	
	/** \brief Coordinate system changed. */
	void CoordSystemChanged(ceConversation *conversation, ceCoordSystem *coordSystem) override;
	
	/** \brief Active coordinate system changed. */
	void ActiveCoordSystemChanged(ceConversation *conversation) override;
	
	/** \brief Prop count or order changed. */
	void PropStructureChanged(ceConversation *conversation) override;
	
	/** \brief Prop changed. */
	void PropChanged(ceConversation *conversation, ceProp *prop) override;
	
	/** \brief Active prop changed. */
	void ActivePropChanged(ceConversation *conversation) override;
	
	/** \brief Playback parameters changed. */
	void PlaybackChanged(ceConversation *conversation) override;
	
	/** \brief Playback info list changed. */
	void PlaybackCommandListChanged(ceConversation *conversation) override;
	
	/** \brief Playback variable list changed. */
	void PlaybackVarListChanged(ceConversation *conversation) override;
	
	/** \brief Playback trigger table changed. */
	void PlaybackTriggerTableChanged(ceConversation *conversation) override;
	
	/** \brief Playback missing words changed. */
	void PlaybackMissingWordsChanged(ceConversation *conversation) override;
	/*@}*/
};

#endif
