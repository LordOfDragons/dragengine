/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	/*@}*/
};

#endif
