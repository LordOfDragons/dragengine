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

#ifndef _CEWPSTOPTOPIC_H_
#define _CEWPSTOPTOPIC_H_

#include "ceWPAction.h"

class ceCAStopTopic;



/**
 * \brief Stop topic conversation action property window.
 */
class ceWPAStopTopic : public ceWPAction{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create conversation action property window. */
	ceWPAStopTopic( ceWPTopic &parentPanel );
	
	/** \brief Clean up conversation action property window. */
	virtual ~ceWPAStopTopic();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Active action or \em NULL if none is active. */
	ceCAStopTopic *GetAction() const;
	
	/** \brief Update active action. */
	void UpdateAction();
	/*@}*/
};

#endif
