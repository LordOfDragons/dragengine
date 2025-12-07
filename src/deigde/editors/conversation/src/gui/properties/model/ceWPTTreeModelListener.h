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

#ifndef _CEWPTTREEMODELLISTENER_H_
#define _CEWPTTREEMODELLISTENER_H_

#include "../../../conversation/ceConversationListener.h"

class ceWPTTreeModel;



/**
 * \brief Tree model listener.
 */
class ceWPTTreeModelListener : public ceConversationListener{
private:
	ceWPTTreeModel &pModel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	ceWPTTreeModelListener(ceWPTTreeModel &model);
	
	/** \brief Clean up listener. */
	~ceWPTTreeModelListener() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Target count or order changed. */
	void TargetStructureChanged(ceConversation *conversation) override;
	
	/** \brief Target changed. */
	void TargetChanged(ceConversation *conversation, ceTarget *target) override;
	
	
	
	/** \brief Camera shot count or order changed. */
	void CameraShotStructureChanged(ceConversation *conversation) override;
	
	/** \brief Camera shot changed. */
	void CameraShotChanged(ceConversation *conversation, ceCameraShot *cameraShot) override;
	
	
	
	/** \brief Gesture count or order changed. */
	void GestureStructureChanged(ceConversation *conversation) override;
	
	/** \brief Gesture changed. */
	void GestureChanged(ceConversation *conversation, ceGesture *gesture) override;
	
	
	
	/** \brief Face pose count or order changed. */
	void FacePoseStructureChanged(ceConversation *conversation) override;
	
	/** \brief Face pose changed. */
	void FacePoseChanged(ceConversation *conversation, ceFacePose *facePose) override;
	
	
	
	/** \brief File count or order changed. */
	void FileStructureChanged(ceConversation *conversation) override;
	
	/** \brief File changed. */
	void FileChanged(ceConversation *conversation, ceConversationFile *file) override;
	
	/** \brief Active file changed. */
	void ActiveFileChanged(ceConversation *conversation) override;
	
	
	
	/** \brief Topic count or order changed. */
	void TopicStructureChanged(ceConversation *conversation, ceConversationFile *file) override;
	
	/** \brief Topic changed. */
	void TopicChanged(ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic) override;
	
	/** \brief Active topic changed. */
	void ActiveTopicChanged(ceConversation *conversation, ceConversationFile *file) override;
	
	
	
	/** \brief Action count or order changed. */
	void ActionStructureChanged(ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action) override;
	
	/** \brief Action changed. */
	void ActionChanged(ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action) override;
	
	/** \brief Condition count or order changed. */
	void ConditionStructureChanged(ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action) override;
	
	/** \brief Condition changed. */
	void ConditionChanged(ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action, ceConversationCondition *condition) override;
	
	/** \brief Active changed. */
	void ActiveChanged(ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic) override;
	
	
	
	/** \brief Actor count or order changed. */
	void ActorStructureChanged(ceConversation *conversation) override;
	
	/** \brief Actor changed. */
	void ActorChanged(ceConversation *conversation, ceConversationActor *actor) override;
	
	
	
	/** Language pack changed. */
	void LanguagePackChanged(ceConversation *conversation) override;
	/*@}*/
};

#endif
