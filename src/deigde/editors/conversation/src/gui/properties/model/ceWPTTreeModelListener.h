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
	ceWPTTreeModelListener( ceWPTTreeModel &model );
	
	/** \brief Clean up listener. */
	virtual ~ceWPTTreeModelListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Target count or order changed. */
	virtual void TargetStructureChanged( ceConversation *conversation );
	
	/** \brief Target changed. */
	virtual void TargetChanged( ceConversation *conversation, ceTarget *target );
	
	
	
	/** \brief Camera shot count or order changed. */
	virtual void CameraShotStructureChanged( ceConversation *conversation );
	
	/** \brief Camera shot changed. */
	virtual void CameraShotChanged( ceConversation *conversation, ceCameraShot *cameraShot );
	
	
	
	/** \brief Gesture count or order changed. */
	virtual void GestureStructureChanged( ceConversation *conversation );
	
	/** \brief Gesture changed. */
	virtual void GestureChanged( ceConversation *conversation, ceGesture *gesture );
	
	
	
	/** \brief Face pose count or order changed. */
	virtual void FacePoseStructureChanged( ceConversation *conversation );
	
	/** \brief Face pose changed. */
	virtual void FacePoseChanged( ceConversation *conversation, ceFacePose *facePose );
	
	
	
	/** \brief File count or order changed. */
	virtual void FileStructureChanged( ceConversation *conversation );
	
	/** \brief File changed. */
	virtual void FileChanged( ceConversation *conversation, ceConversationFile *file );
	
	/** \brief Active file changed. */
	virtual void ActiveFileChanged( ceConversation *conversation );
	
	
	
	/** \brief Topic count or order changed. */
	virtual void TopicStructureChanged( ceConversation *conversation, ceConversationFile *file );
	
	/** \brief Topic changed. */
	virtual void TopicChanged( ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic );
	
	/** \brief Active topic changed. */
	virtual void ActiveTopicChanged( ceConversation *conversation, ceConversationFile *file );
	
	
	
	/** \brief Action count or order changed. */
	virtual void ActionStructureChanged( ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action );
	
	/** \brief Action changed. */
	virtual void ActionChanged( ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action );
	
	/** \brief Condition count or order changed. */
	virtual void ConditionStructureChanged( ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action );
	
	/** \brief Condition changed. */
	virtual void ConditionChanged( ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action, ceConversationCondition *condition );
	
	/** \brief Active changed. */
	virtual void ActiveChanged( ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic );
	
	
	
	/** \brief Actor count or order changed. */
	virtual void ActorStructureChanged( ceConversation *conversation );
	
	/** \brief Actor changed. */
	virtual void ActorChanged( ceConversation *conversation, ceConversationActor *actor );
	
	
	
	/** Language pack changed. */
	virtual void LanguagePackChanged( ceConversation *conversation );
	/*@}*/
};

#endif
