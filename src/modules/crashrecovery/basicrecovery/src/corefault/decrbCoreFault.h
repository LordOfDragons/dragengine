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
#ifndef _DECRBCOREFAULT_H_
#define _DECRBCOREFAULT_H_

// includes

// predefinitions
class deCRBasic;



/**
 * @brief Core Fault.
 *
 * Takes over in the case of a serious core fault. Such faults can not
 * be handled anymore returning to a sane state and have to result in
 * the application exiting if possible. Since such faults are not very
 * informative this class tries to gather as much information as
 * possible before jumping the knife. For this an additional process
 * is spawned which idles waiting for a problem. If a core fault happens
 * the other process is still in a sane state and takes over while this
 * process tries to gather information until it exits or gets killed
 * otherwise. Communication between the processes is done using files.
 */
class decrbCoreFault{
public:
	/** Error codes. */
	enum eErrorCodes{
		/** Access to not allocated memory. */
		eecMemoryNotAllocated,
		/** Access to memory nor permitted. */
		eecMemoryNoPermision,
		/** Unknown memory error. */
		eecMemoryUnknown
	};
	
private:
	deCRBasic *pModule;
	
	static decrbCoreFault *pGlobalCoreFault;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new object. */
	decrbCoreFault( deCRBasic *module );
	/** Cleans up the object. */
	~decrbCoreFault();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the global core fault. */
	static decrbCoreFault *GetGlobalCoreFault();
	
	/** Retrieves the module. */
	inline deCRBasic *GetModule() const{ return pModule; }
	/** Handle a segmentation fault. */
	void HandleSegFault( int error, void *memoryLocation, void *ptrContext );
	/** Handle an unhandled exception. */
	void HandleAbort( void *ptrContext );
	/*@}*/
	
private:
	void pCleanUp();
	
	void pRegisterHandlers();
	void pUnregisterHandlers();
};

// end of include only once
#endif
