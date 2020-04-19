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
	ceWPConversationListener( ceWPConversation &panel );
	/** Cleans up the listener. */
	virtual ~ceWPConversationListener();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Conversation changed. */
	virtual void ConversationChanged( ceConversation *conversation );
	
	/** Target count or order changed. */
	virtual void TargetStructureChanged( ceConversation *conversation );
	/** Target changed. */
	virtual void TargetChanged( ceConversation *conversation, ceTarget *target );
	/** Active target changed. */
	virtual void ActiveTargetChanged( ceConversation *conversation );
	
	/** Camera shot count or order changed. */
	virtual void CameraShotStructureChanged( ceConversation *conversation );
	/** Camera shot changed. */
	virtual void CameraShotChanged( ceConversation *conversation, ceCameraShot *cameraShot );
	/** Active camera shot changed. */
	virtual void ActiveCameraShotChanged( ceConversation *conversation );
	
	/** Pose count or order changed. */
	virtual void PoseStructureChanged( ceConversation *conversation );
	/** Pose changed. */
	virtual void PoseChanged( ceConversation *conversation, cePose *pose );
	/** Active pose changed. */
	virtual void ActivePoseChanged( ceConversation *conversation );
	
	/** Gesture count or order changed. */
	virtual void GestureStructureChanged( ceConversation *conversation );
	/** Gesture changed. */
	virtual void GestureChanged( ceConversation *conversation, ceGesture *gesture );
	/** Active gesture changed. */
	virtual void ActiveGestureChanged( ceConversation *conversation );
	
	/** Face pose controller names changed. */
	virtual void FacePoseControllerNamesChanged( ceConversation *conversation );
	/** Face pose count or order changed. */
	virtual void FacePoseStructureChanged( ceConversation *conversation );
	/** Face pose changed. */
	virtual void FacePoseChanged( ceConversation *conversation, ceFacePose *facePose );
	/** Active face pose changed. */
	virtual void ActiveFacePoseChanged( ceConversation *conversation );
	
	/** LookAt count or order changed. */
	virtual void LookAtStructureChanged( ceConversation *conversation );
	/** LookAt changed. */
	virtual void LookAtChanged( ceConversation *conversation, ceLookAt *lookat );
	/** Active lookat changed. */
	virtual void ActiveLookAtChanged( ceConversation *conversation );
	
	/** Actor count or order changed. */
	virtual void ActorStructureChanged( ceConversation *conversation );
	/** Actor changed. */
	virtual void ActorChanged( ceConversation *conversation, ceConversationActor *actor );
	/*@}*/
};

#endif
