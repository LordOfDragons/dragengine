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

#ifndef _CEWINDOWDOPESHEETLISTENER_H_
#define _CEWINDOWDOPESHEETLISTENER_H_

#include "../../conversation/ceConversationListener.h"

class ceWindowDopeSheet;



/**
 * \brief Dope sheet window listener.
 */
class ceWindowDopeSheetListener : public ceConversationListener{
private:
	ceWindowDopeSheet &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	ceWindowDopeSheetListener( ceWindowDopeSheet &window );
	
	/** \brief Clean up listener. */
	virtual ~ceWindowDopeSheetListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Conversation changed. */
	virtual void ConversationChanged( ceConversation *conversation );
	
	/** \brief Camera shot changed. */
	virtual void CameraShotChanged( ceConversation *conversation, ceCameraShot *cameraShot );
	
	/** \brief Active camera shot changed. */
	virtual void ActiveCameraShotChanged( ceConversation *conversation );
	
	
	
	/** \brief File count or order changed. */
	virtual void FileStructureChanged( ceConversation *conversation );
	
	/** \brief Active file changed. */
	virtual void ActiveFileChanged( ceConversation *conversation );
	
	
	
	/** \brief Topic count or order changed. */
	virtual void TopicStructureChanged( ceConversation *conversation, ceConversationFile *file );
	
	/** \brief Active topic changed. */
	virtual void ActiveTopicChanged( ceConversation *conversation, ceConversationFile *file );
	
	
	
	/** \brief Action changed. */
	virtual void ActionChanged( ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action );
	
	/** \brief Active element changed. */
	virtual void ActiveChanged( ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic );
	
	
	
	/** \brief Playback parameters changed. */
	virtual void PlaybackChanged( ceConversation *conversation );
	
	/** \brief Playback action stack changed. */
	virtual void PlaybackActionStackChanged( ceConversation *conversation );
	/*@}*/
};

#endif
