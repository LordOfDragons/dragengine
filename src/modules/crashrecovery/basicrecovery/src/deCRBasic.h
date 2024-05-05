/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
