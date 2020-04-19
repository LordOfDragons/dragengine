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

#ifndef _DEALWIDGETLOGFILE_H_
#define _DEALWIDGETLOGFILE_H_

#include "dealWidgetLayoutFlow.h"
#include "../../common/collection/decObjectList.h"
#include "../../common/string/decString.h"
#include "../../common/file/decPath.h"



/**
 * \brief Log history widget.
 */
class dealWidgetLogFile : public dealWidgetLayoutFlow{
private:
	/** \brief Log entry. */
	class LogEntry : public deObject{
	public:
		/** \brief Log types. */
		enum eLogTypes{
			/** \brief Information. */
			eltInfo,
			
			/** \brief Warning. */
			eltWarn,
			
			/** \brief Error. */
			eltError
		};
		
	private:
		/** \brief Type. */
		eLogTypes pType;
		
		/** \brief Source. */
		decString pSource;
		
		/** \brief Message. */
		decString pMessage;
		
	public:
		/** \brief Create log entry. */
		LogEntry( eLogTypes type, const char *source, const char *message );
		
		/** \brief Clean up log entry. */
		virtual ~LogEntry();
		
		/** \brief Type. */
		inline eLogTypes GetType() const{ return pType; }
		
		/** \brief Source. */
		inline const decString &GetSource() const{ return pSource; }
		
		/** \brief Message. */
		inline const decString &GetMessage() const{ return pMessage; }
	};
	
	
	
private:
	decColor pColorInfo;
	decColor pColorWarn;
	decColor pColorError;
	
	decColor pBgLinesAlternate;
	
	decObjectList pLogs;
	int pMaxLogCount;
	
	decString pFilename;
	decPath pPath;
	int pLastFileSize;
	
	int pTimerNextCheck;
	int pVisibleLineCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetLogFile( dealDisplay &display );
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetLogFile();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** \brief Set filename. */
	void SetFilename( const char *filename );
	
	/** \brief Check file and update content if required. */
	void CheckFile();
	
	/** \brief Add log line. */
	void AddLogLine( const decString &line );
	
	/** \brief Update content from logs. */
	void UpdateContent();
	
	
	
	/** \brief Number of lines to show at minimum size. */
	inline int GetVisibleLineCount() const{ return pVisibleLineCount; }
	
	/** \brief Set number of lines to show at minimum size. */
	void SetVisibleLineCount( int lineCount );
	
	
	
	/** \brief Minimum size of widget. */
	virtual decPoint GetMinimumSize();
	
	/** \brief Render content. */
	virtual void RenderContent( const sRenderContext &context );
	/*@}*/
};

#endif
