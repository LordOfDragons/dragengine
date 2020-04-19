/* 
 * Drag[en]gine Basic Crash Recovery Module
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

// include only once
#ifndef _DECRBASIC_H_
#define _DECRBASIC_H_

// includes
#include "dragengine/systems/modules/crashrecovery/deBaseCrashRecoveryModule.h"

class deErrorTraceValue;
// predefinitions
class decrbCoreFault;



// basic crash recovery module
class deCRBasic : public deBaseCrashRecoveryModule{
private:
	bool pQuitEngine;
	
	decrbCoreFault *pCoreFault;
	
public:
	// constructor, destructor
	deCRBasic( deLoadableModule &loadableModule );
	~deCRBasic();
	// management
	bool Init();
	void CleanUp();
	// crash management
	bool RecoverFromError();
	// internal functions for module classes only
	void SetQuitEngine( bool quitEngine );
	
	/** \brief Log trace to console. */
	void LogTrace();
	
	/** \brief Log sub value error to console. */
	void LogTraceSubValues( const deErrorTraceValue &traceValue, const char *indent );
};

// end of include only once
#endif
