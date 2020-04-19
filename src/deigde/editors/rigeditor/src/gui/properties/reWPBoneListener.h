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

#ifndef _REWPBONELISTENER_H_
#define _REWPBOELISTENER_H_

#include "../../rig/reRigNotifier.h"

class reWPBone;



/**
 * \brief Bone panel listenerer.
 */
class reWPBoneListener : public reRigNotifier{
private:
	reWPBone &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	reWPBoneListener( reWPBone &panel );
	
	/** \brief Clean up listener. */
	virtual ~reWPBoneListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief A bone changed. */
	void BoneChanged( reRig *rig, reRigBone *bone );
	
	/** \brief The active bone changed. */
	void ActiveBoneChanged( reRig *rig );
	/*@}*/
};

#endif
