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
