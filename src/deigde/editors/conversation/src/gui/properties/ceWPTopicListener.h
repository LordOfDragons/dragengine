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

#ifndef _CEWPTOPICLISTENER_H_
#define _CEWPTOPICLISTENER_H_

#include "../../conversation/ceConversationListener.h"

class ceWPTopic;



/**
 * Topic panel listener.
 */
class ceWPTopicListener : public ceConversationListener{
public:
	typedef deTObjectReference<ceWPTopicListener> Ref;
	
private:
	ceWPTopic &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create listener. */
	ceWPTopicListener(ceWPTopic &panel);
	
	/** Clean up listener. */
	~ceWPTopicListener() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Conversation changed. */
	void ConversationChanged(ceConversation *conversation) override;
	
	/** Target count or order changed. */
	void TargetStructureChanged(ceConversation *conversation) override;
	
	/** Target changed. */
	void TargetChanged(ceConversation *conversation, ceTarget *target) override;
	
	
	
	/** Camera shot count or order changed. */
	void CameraShotStructureChanged(ceConversation *conversation) override;
	
	/** Camera shot changed. */
	void CameraShotChanged(ceConversation *conversation, ceCameraShot *cameraShot) override;
	
	
	
	/** Gesture count or order changed. */
	void GestureStructureChanged(ceConversation *conversation) override;
	
	/** Gesture changed. */
	void GestureChanged(ceConversation *conversation, ceGesture *gesture) override;
	
	
	
	/** Face pose count or order changed. */
	void FacePoseStructureChanged(ceConversation *conversation) override;
	
	/** Face pose changed. */
	void FacePoseChanged(ceConversation *conversation, ceFacePose *facePose) override;
	
	
	
	/** File count or order changed. */
	void FileStructureChanged(ceConversation *conversation) override;
	
	/** File changed. */
	void FileChanged(ceConversation *conversation, ceConversationFile *file) override;
	
	/** Active file changed. */
	void ActiveFileChanged(ceConversation *conversation) override;
	
	
	
	/** Topic count or order changed. */
	void TopicStructureChanged(ceConversation *conversation, ceConversationFile *file) override;
	
	/** Topic changed. */
	void TopicChanged(ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic) override;
	
	/** Active topic changed. */
	void ActiveTopicChanged(ceConversation *conversation, ceConversationFile *file) override;
	
	
	
	/** Action count or order changed. */
	void ActionStructureChanged(ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action) override;
	
	/** Action changed. */
	void ActionChanged(ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action) override;
	
	/** Condition count or order changed. */
	void ConditionStructureChanged(ceConversation *conversation,
		ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action) override;
	
	/** Condition changed. */
	void ConditionChanged(ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action, ceConversationCondition *condition) override;
	
	
	
	/** Actor count or order changed. */
	void ActorStructureChanged(ceConversation *conversation) override;
	
	/** Actor changed. */
	void ActorChanged(ceConversation *conversation, ceConversationActor *actor) override;
	
	
	
	/** Coordinate system count or order changed. */
	void CoordSystemStructureChanged(ceConversation *conversation) override;
	
	/** Coordinate system changed. */
	void CoordSystemChanged(ceConversation *conversation, ceCoordSystem *coordSystem) override;
	
	
	
	/** Language pack changed. */
	void LanguagePackChanged(ceConversation *conversation) override;
	/*@}*/
};

#endif
