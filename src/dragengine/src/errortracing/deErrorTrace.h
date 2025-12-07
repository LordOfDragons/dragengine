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

#ifndef _DEAERRORTRACE_H_
#define _DEAERRORTRACE_H_

#include "../common/string/decString.h"

class deErrorTracePoint;
class deLoadableModule;
class deLogger;


/**
 * \brief Error Trace.
 *
 * An Error Trace object contains a list of Error Trace Point objects.
 * describing the path of an error that occurred in the engine.
 */
class DE_DLL_EXPORT deErrorTrace{
private:
	decString pError;
	deErrorTracePoint **pPoints;
	int pPointCount;
	int pPointSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new error trace object. */
	deErrorTrace();
	
	/** \brief Clean up error trace object. */
	~deErrorTrace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Error. */
	inline const decString &GetError() const{ return pError; }
	
	/** \brief Set error. */
	void SetError(const char *error);
	
	/** \brief Clears the error trace. */
	void Clear();
	/*@}*/
	
	
	
	/** \name Trace Point Management */
	/*@{*/
	/** \brief Determines if there exist trace points. */
	inline bool HasPoints() const{ return pPointCount > 0; }
	
	/** \brief Trace is empty. */
	inline bool IsEmpty() const{ return pPointCount == 0; }
	
	/** \brief Count of trace points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	/** \brief Indexed trace point. */
	deErrorTracePoint *GetPoint(int index) const;
	
	/** \brief Adds a new trace point. */
	void AddPoint(deErrorTracePoint *point);
	
	/** \brief Removes all trace points. */
	void RemoveAllPoints();
	/*@}*/
	
	
	
	/** \name Convenience Functions */
	/*@{*/
	/**
	 * \brief Add given error to the trace and sets the error if there exists no trace point yet.
	 * 
	 * Returns the new trace point if added or NULL otherwise.
	 */
	deErrorTracePoint *AddAndSetIfEmpty(const char *error, deLoadableModule *sourceModule,
		const char *sourceFunc, int sourceLine);
	
	/** \brief Add new trace point with the given information. */
	deErrorTracePoint *AddPoint(deLoadableModule *sourceModule, const char *sourceFunc, int sourceLine);
	
	/**
	 * \brief Print error trace out to the given logger.
	 * 
	 * This is more of a temporary function and should not be used.
	 */
	void PrintTrace(deLogger &logger);
	/*@}*/
};

#endif
