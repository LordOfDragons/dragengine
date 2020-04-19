/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _REWPPUSHLISTENER_H_
#define _REWPPUSHLISTENER_H_

#include "../../rig/reRigNotifier.h"

class reWPPush;



/**
 * \brief Push panel listenerer
 */
class reWPPushListener : public reRigNotifier{
private:
	reWPPush &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	reWPPushListener( reWPPush &panel );
	
	/** \brief Clean up listener. */
	virtual ~reWPPushListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief A push changed. */
	void PushChanged( reRig *rig, reRigPush *push );
	
	/** \brief The active push changed. */
	void ActivePushChanged( reRig *rig );
	/*@}*/
};

#endif
