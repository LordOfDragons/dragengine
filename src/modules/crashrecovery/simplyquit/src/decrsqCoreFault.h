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

#ifndef _DECRSQCOREFAULT_H_
#define _DECRSQCOREFAULT_H_

class deCRSimplyQuit;


/**
 * \brief Core fault.
 *
 * Takes over in the case of a serious core fault. Such faults can not
 * be handled anymore returning to a sane state and have to result in
 * the application exiting if possible. Since such faults are not very
 * informative this class tries to gather as much information as
 * possible before jumping the knife. For this an additional process
 * is spawned which idles waiting for a problem. If a core fault happens
 * the other process is still in a sane state and takes over while this
 * process tries to gather informations until it exits or gets killed
 * otherwise. Communication between the processes is done using files.
 */
class decrsqCoreFault{
public:
	/** \brief Error codes. */
	enum eErrorCodes{
		/** \brief Access to not allocated memory. */
		eecMemoryNotAllocated,
		
		/** \brief Access to memory nor permitted. */
		eecMemoryNoPermision,
		
		/** \brief Unknown memory error. */
		eecMemoryUnknown
	};
	
	
	
private:
	deCRSimplyQuit &pModule;
	
	static decrsqCoreFault *pGlobalCoreFault;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create core fault. */
	decrsqCoreFault( deCRSimplyQuit &module );
	
	/** \brief Clean up core fault. */
	~decrsqCoreFault();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Global core fault. */
	static decrsqCoreFault *GetGlobalCoreFault();
	
	/** \brief Module. */
	inline deCRSimplyQuit &GetModule() const{ return pModule; }
	
	/** \brief Handle segmentation fault. */
	void HandleSegFault( int error, void *memoryLocation, void *ptrContext );
	
	/** \brief Handle bus error fault. */
	void HandleBusError( int error, void *memoryLocation, void *ptrContext );
	
	/** \brief Handle unhandled exception. */
	void HandleAbort( void *ptrContext );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pRegisterHandlers();
	void pUnregisterHandlers();
};

#endif
