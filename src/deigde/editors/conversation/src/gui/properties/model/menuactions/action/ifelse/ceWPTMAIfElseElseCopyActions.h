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

#ifndef _CEWPTMAIFELSEELSECOPYACTIONS_H_
#define _CEWPTMAIFELSEELSECOPYACTIONS_H_

#include "../ceWPTMACopyActions.h"

class ceCAIfElse;



/**
 * \brief Menu action copy all topic actions.
 */
class ceWPTMAIfElseElseCopyActions : public ceWPTMACopyActions{
protected:
	/** \brief FOX constructor. */
	ceWPTMAIfElseElseCopyActions();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMAIfElseElseCopyActions( ceWindowMain &windowMain,
		const ceCAIfElse &ifElse );
	/*@}*/
};

#endif
