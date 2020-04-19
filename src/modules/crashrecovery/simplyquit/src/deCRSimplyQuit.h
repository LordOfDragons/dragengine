/* 
 * Drag[en]gine Simply Quit Crash Recovery Module
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

#ifndef _DECRSIMPLYQUIT_H_
#define _DECRSIMPLYQUIT_H_

#include <dragengine/systems/modules/crashrecovery/deBaseCrashRecoveryModule.h>

class deErrorTracePoint;
class deErrorTraceValue;
class decrsqCoreFault;



/**
 * \brief Simply quit crash recovery module
 */
class deCRSimplyQuit : public deBaseCrashRecoveryModule{
private:
	decrsqCoreFault *pCoreFault;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deCRSimplyQuit( deLoadableModule &loadableModule );
	
	/** \brief Clean up core fault. */
	virtual ~deCRSimplyQuit();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Initialize module. */
	virtual bool Init();
	
	/** \brief Clean up module. */
	virtual void CleanUp();
	
	/** \brief Recover from error. */
	virtual bool RecoverFromError();
	/*@}*/
	
	
	
private:
	void pPrintTrace();
	void pPrintTracePoint( int number, deErrorTracePoint *point );
	void pPrintTraceValue( int level, deErrorTraceValue *value );
};

#endif
