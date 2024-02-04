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

#ifndef _CEWPTOPICLISTENER_H_
#define _CEWPTOPICLISTENER_H_

#include "../../conversation/ceConversationListener.h"

class ceWPTopic;



/**
 * Topic panel listener.
 */
class ceWPTopicListener : public ceConversationListener{
private:
	ceWPTopic &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create listener. */
	ceWPTopicListener( ceWPTopic &panel );
	
	/** Clean up listener. */
	virtual ~ceWPTopicListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Conversation changed. */
	virtual void ConversationChanged( ceConversation *conversation );
	
	/** Target count or order changed. */
	virtual void TargetStructureChanged( ceConversation *conversation );
	
	/** Target changed. */
	virtual void TargetChanged( ceConversation *conversation, ceTarget *target );
	
	
	
	/** Camera shot count or order changed. */
	virtual void CameraShotStructureChanged( ceConversation *conversation );
	
	/** Camera shot changed. */
	virtual void CameraShotChanged( ceConversation *conversation, ceCameraShot *cameraShot );
	
	
	
	/** Gesture count or order changed. */
	virtual void GestureStructureChanged( ceConversation *conversation );
	
	/** Gesture changed. */
	virtual void GestureChanged( ceConversation *conversation, ceGesture *gesture );
	
	
	
	/** Face pose count or order changed. */
	virtual void FacePoseStructureChanged( ceConversation *conversation );
	
	/** Face pose changed. */
	virtual void FacePoseChanged( ceConversation *conversation, ceFacePose *facePose );
	
	
	
	/** File count or order changed. */
	virtual void FileStructureChanged( ceConversation *conversation );
	
	/** File changed. */
	virtual void FileChanged( ceConversation *conversation, ceConversationFile *file );
	
	/** Active file changed. */
	virtual void ActiveFileChanged( ceConversation *conversation );
	
	
	
	/** Topic count or order changed. */
	virtual void TopicStructureChanged( ceConversation *conversation, ceConversationFile *file );
	
	/** Topic changed. */
	virtual void TopicChanged( ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic );
	
	/** Active topic changed. */
	virtual void ActiveTopicChanged( ceConversation *conversation, ceConversationFile *file );
	
	
	
	/** Action count or order changed. */
	virtual void ActionStructureChanged( ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action );
	
	/** Action changed. */
	virtual void ActionChanged( ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action );
	
	/** Condition count or order changed. */
	virtual void ConditionStructureChanged( ceConversation *conversation,
		ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action );
	
	/** Condition changed. */
	virtual void ConditionChanged( ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action, ceConversationCondition *condition );
	
	
	
	/** Actor count or order changed. */
	virtual void ActorStructureChanged( ceConversation *conversation );
	
	/** Actor changed. */
	virtual void ActorChanged( ceConversation *conversation, ceConversationActor *actor );
	
	
	
	/** Coordinate system count or order changed. */
	virtual void CoordSystemStructureChanged( ceConversation *conversation );
	
	/** Coordinate system changed. */
	virtual void CoordSystemChanged( ceConversation *conversation, ceCoordSystem *coordSystem );
	
	
	
	/** Language pack changed. */
	virtual void LanguagePackChanged( ceConversation *conversation );
	/*@}*/
};

#endif
