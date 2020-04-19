/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DELOGGERFILE_H_
#define _DELOGGERFILE_H_

#include "deLogger.h"
#include "../threading/deMutex.h"

class decBaseFileWriter;



/**
 * \brief Logs to file.
 * 
 * Adds a prefix depending on the log message type. The file is given by a file writer.
 * The file writer object is owned by the file logger. It is therefore deleted when
 * the file logger is freed.
 * 
 * \note Logger console is thread safe. To avoid torn logs the entire text line is
 *       formated in memory and send as one write call then fflush is called.
 *       Be careful with the use of the file writer outside the logger. deObject
 *       reference counting is not thread safe.
 */
class deLoggerFile : public deLogger{
private:
	decBaseFileWriter *pWriter;
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file logger. */
	deLoggerFile( decBaseFileWriter *writer );
	
protected:
	/**
	 * \brief Clean up file logger.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deLoggerFile();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief File writer. */
	inline decBaseFileWriter *GetWriter() const{ return pWriter; }
	
	
	
	/** \brief Log information message. */
	virtual void LogInfo( const char *source, const char *message );
	
	/** \brief Log warning message. */
	virtual void LogWarn( const char *source, const char *message );
	
	/** \brief Log error message. */
	virtual void LogError( const char *source, const char *message );
	/*@}*/
	
	
	
protected:
	inline deMutex &GetMutex(){ return pMutex; }
};

#endif
