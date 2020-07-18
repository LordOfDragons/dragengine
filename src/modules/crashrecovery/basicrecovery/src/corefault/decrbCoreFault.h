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
