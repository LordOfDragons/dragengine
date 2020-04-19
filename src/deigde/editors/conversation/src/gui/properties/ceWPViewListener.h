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
	ceWPViewListener( ceWPView &panel );
	
	/** \brief Clean up listener. */
	virtual ~ceWPViewListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Conversation changed. */
	virtual void ConversationChanged( ceConversation *conversation );
	
	/** \brief The sky changed. */
	virtual void SkyChanged( ceConversation *conversation );
	
	/** \brief The environment component changed. */
	virtual void EnvObjectChanged( ceConversation *conversation );
	
	/** \brief Camera changed. */
	virtual void CameraChanged( ceConversation *conversation );
	
	/** \brief File count or order changed. */
	virtual void FileStructureChanged( ceConversation *conversation );
	
	/** \brief File changed. */
	virtual void FileChanged( ceConversation *conversation, ceConversationFile *file );
	
	/** \brief Topic count or order changed. */
	virtual void TopicStructureChanged( ceConversation *conversation, ceConversationFile *file );
	
	/** \brief Topic changed. */
	virtual void TopicChanged( ceConversation *conversation, ceConversationFile *file, ceConversationTopic *topic );
	
	
	
	/** \brief Actor count or order changed. */
	virtual void ActorStructureChanged( ceConversation *conversation );
	
	/** \brief Actor changed. */
	virtual void ActorChanged( ceConversation *conversation, ceConversationActor *actor );
	
	/** \brief Actor active pose changed. */
	virtual void ActorActivePoseChanged( ceConversation *conversation, ceConversationActor *actor );
	
	/** \brief Actor poses changed. */
	virtual void ActorPosesChanged( ceConversation *conversation, ceConversationActor *actor );
	
	/** \brief Actor active pose gestures changed. */
	virtual void ActorPoseGesturesChanged( ceConversation *conversation,
		ceConversationActor *actor, ceActorPose *pose );
	
	/** \brief Actor commands changed. */
	virtual void ActorCommandsChanged( ceConversation *conversation, ceConversationActor *actor );
	
	/** \brief Actor parameters changed. */
	virtual void ActorParametersChanged( ceConversation *conversation, ceConversationActor *actor );
	
	/** \brief Active actor changed. */
	virtual void ActiveActorChanged( ceConversation *conversation );
	
	
	
	/** \brief Coordinate system count or order changed. */
	virtual void CoordSystemStructureChanged( ceConversation *conversation );
	
	/** \brief Coordinate system changed. */
	virtual void CoordSystemChanged( ceConversation *conversation, ceCoordSystem *coordSystem );
	
	/** \brief Active coordinate system changed. */
	virtual void ActiveCoordSystemChanged( ceConversation *conversation );
	
	/** \brief Prop count or order changed. */
	virtual void PropStructureChanged( ceConversation *conversation );
	
	/** \brief Prop changed. */
	virtual void PropChanged( ceConversation *conversation, ceProp *prop );
	
	/** \brief Active prop changed. */
	virtual void ActivePropChanged( ceConversation *conversation );
	
	/** \brief Playback parameters changed. */
	virtual void PlaybackChanged( ceConversation *conversation );
	
	/** \brief Playback info list changed. */
	virtual void PlaybackCommandListChanged( ceConversation *conversation );
	
	/** \brief Playback variable list changed. */
	virtual void PlaybackVarListChanged( ceConversation *conversation );
	
	/** \brief Playback trigger table changed. */
	virtual void PlaybackTriggerTableChanged( ceConversation *conversation );
	
	/** \brief Playback missing words changed. */
	virtual void PlaybackMissingWordsChanged( ceConversation *conversation );
	/*@}*/
};

#endif
