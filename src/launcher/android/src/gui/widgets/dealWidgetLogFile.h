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
