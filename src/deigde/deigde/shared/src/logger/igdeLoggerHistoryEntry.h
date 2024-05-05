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

#ifndef _IGDELOGGERHISTORYENTRY_H_
#define _IGDELOGGERHISTORYENTRY_H_

#include <dragengine/common/string/decString.h>



/**
 * \brief Logger History Entry.
 * 
 * Message entry in a logger history.
 */
class DE_DLL_EXPORT igdeLoggerHistoryEntry{
public:
	/** \brief Message types. */
	enum eMessageTypes{
		/** \brief Information. */
		emtInfo,
		
		/** \brief Warning. */
		emtWarn,
		
		/** \brief Error. */
		emtError
	};
	
	
	
private:
	// time stamp...
	int pType;
	decString pSource;
	decString pMessage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create logger history entry. */
	igdeLoggerHistoryEntry();
	
	/** \brief Clean up logger history entry. */
	~igdeLoggerHistoryEntry();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Type. */
	inline int GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( int type );
	
	/** \brief Source. */
	inline const decString &GetSource() const{ return pSource; }
	inline decString &GetSource(){ return pSource; }
	
	/** \brief Message. */
	inline const decString &GetMessage() const{ return pMessage; }
	inline decString &GetMessage(){ return pMessage; }
	
	/** \brief Clears the entry. */
	void Clear();
	
	/** \brief Clean up message entry by removing extra blanks and newlines at the end. */
	void CleanUpMessage();
	/*@}*/
};

#endif
