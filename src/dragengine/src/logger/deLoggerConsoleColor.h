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

#ifndef _DELOGGERCONSOLECOLOR_H_
#define _DELOGGERCONSOLECOLOR_H_

#include "deLogger.h"
#include "../threading/deMutex.h"


/**
 * \brief Logs to the standard output stream (aka the console in most cases) using colors.
 * 
 * In contrary to \ref LogConsole this class supports using text attributes and
 * text color codes to highlight info, warning and error log entries better. The
 * used coloring can be adjusted.
 * 
 * \note Logger color console is thread safe.
 */
class DE_DLL_EXPORT deLoggerConsoleColor : public deLogger{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deLoggerConsoleColor> Ref;
	
	
public:
	/** \brief Text attributes. */
	enum eTextAttributes{
		etaDefault,
		etaBright,
		etaDim,
		etaUnderline,
		etaBlink
	};
	
	/** \brief Text colors. */
	enum eTextColors{
		etcDefault,
		etcBlack,
		etcRed,
		etcGreen,
		etcYellow,
		etcBlue,
		etcMagenta,
		etcCyan,
		etcWhite
	};
	
	
	
private:
	eTextAttributes pInfoAttribute;
	eTextColors pInfoColorForeground;
	eTextColors pInfoColorBackground;
	
	eTextAttributes pWarnAttribute;
	eTextColors pWarnColorForeground;
	eTextColors pWarnColorBackground;
	
	eTextAttributes pErrorAttribute;
	eTextColors pErrorColorForeground;
	eTextColors pErrorColorBackground;
	
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create console color logger. */
	deLoggerConsoleColor();
	
protected:
	/** \brief Clean up console color logger. */
	virtual ~deLoggerConsoleColor();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Info text attribute. */
	eTextAttributes GetInfoAttribute();
	
	/** \brief Set info text attribute. */
	void SetInfoAttribute(eTextAttributes attribute);
	
	/** \brief Info text foreground color. */
	eTextColors GetInfoColorForeground();
	
	/** \brief Set info text foreground color. */
	void SetInfoColorForeground(eTextColors color);
	
	/** \brief Info text background color. */
	eTextColors GetInfoColorBackground();
	
	/** \brief Set info text background color. */
	void SetInfoColorBackground(eTextColors color);
	
	
	
	/** \brief Warn text attribute. */
	eTextAttributes GetWarnAttribute();
	
	/** \brief Set warn text attribute. */
	void SetWarnAttribute(eTextAttributes attribute);
	
	/** \brief Warn text foreground color. */
	eTextColors GetWarnColorForeground();
	
	/** \brief Set warn text foreground color. */
	void SetWarnColorForeground(eTextColors color);
	
	/** \brief Warn text background color. */
	eTextColors GetWarnColorBackground();
	
	/** \brief Set warn text background color. */
	void SetWarnColorBackground(eTextColors color);
	
	
	
	/** \brief Error text attribute. */
	eTextAttributes GetErrorAttribute();
	
	/** \brief Set error text attribute. */
	void SetErrorAttribute(eTextAttributes attribute);
	
	/** \brief Error text foreground color. */
	eTextColors GetErrorColorForeground();
	
	/** \brief Set error text foreground color. */
	void SetErrorColorForeground(eTextColors color);
	
	/** \brief Error text background color. */
	eTextColors GetErrorColorBackground();
	
	/** \brief Set error text background color. */
	void SetErrorColorBackground(eTextColors color);
	
	
	
	/** \brief Log information message. */
	virtual void LogInfo(const char *source, const char *message);
	
	/** \brief Log warning message. */
	virtual void LogWarn(const char *source, const char *message);
	
	/** \brief Log error message. */
	virtual void LogError(const char *source, const char *message);
	/*@}*/
	
	
	
protected:
	/**
	 * \brief Print message to console using color codes.
	 * \note This call is not thread safe.
	 */
	void pPrintMessage(const char *source, const char *message,
		eTextAttributes attribute, eTextColors colorForeground,
		eTextColors colorBackground, char lineEnd) const;
	
	void LogMessage(const char *source, const char *message,
		eTextAttributes attribute, eTextColors colorForeground,
		eTextColors colorBackground);
	
	inline deMutex &GetMutex(){ return pMutex; }
};

#endif
