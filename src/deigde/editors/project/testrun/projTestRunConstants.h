/* 
 * DEIGDE Project
 *
 * Copyright (C) 2018, Plüss Roland ( roland@rptd.ch )
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is projributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _PROJTESTRUNCONSTANTS_H_
#define _PROJTESTRUNCONSTANTS_H_


/**
 * \brief Test run constants.
 */
namespace projTestRunConstants{
	/** \brief Commands. */
	enum eCommands{
		/** \brief Quit process. */
		ecQuit
	};
	
	/** \brief Result Codes. */
	enum eResultCodes{
		/** \brief Command successful. */
		ercSuccess,
		
		/** \brief Command failed. */
		ercFailed,
		
		/** \brief Game exited. */
		ercGameExited
	};
};

#endif
