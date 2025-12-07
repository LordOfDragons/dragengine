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

#ifndef _DEAERRORTRACEPOINT_H_
#define _DEAERRORTRACEPOINT_H_

#include "../common/string/decString.h"

class deErrorTraceValue;
class deLoadableModule;


/**
 * \brief Error Trace Point.
 *
 * An Error Trace object contains a list of Error Trace Point objects.
 * describing the path of an error that occurred in the engine.
 */
class DE_DLL_EXPORT deErrorTracePoint{
private:
	deLoadableModule * const pSourceModule;
	const decString pSourceFunc;
	const int pSourceLine;
	deErrorTraceValue **pValues;
	int pValueCount, pValueSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new error trace point object. */
	deErrorTracePoint(const char *sourceFunc, int sourceLine);
	
	/** \brief Create new error trace point object. */
	deErrorTracePoint(deLoadableModule *module, const char *sourceFunc, int sourceLine);
	
	/** \brief Clean up error trace object. */
	~deErrorTracePoint();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Source module or NULL if the error occurred in the engine itself. */
	inline deLoadableModule *GetSourceModule() const{ return pSourceModule; }
	
	/** \brief Source function. */
	inline const decString &GetSourceFunction() const{ return pSourceFunc; }
	
	/** \brief Source line. */
	inline int GetSourceLine() const{ return pSourceLine; }
	/*@}*/

	/** \name Trace Value Management */
	/*@{*/
	/** \brief Determines if there exist trace values. */
	inline bool HasValues() const{ return pValueCount > 0; }
	
	/** \brief Count of trace values. */
	inline int GetValueCount() const{ return pValueCount; }
	
	/** \brief Indexed trace value. */
	deErrorTraceValue *GetValue(int index) const;
	
	/** \brief Named trace value or NULL if not found. */
	deErrorTraceValue *FindValue(const char *name) const;
	
	/** \brief Adds a new trace value. */
	void AddValue(deErrorTraceValue *value);
	
	/** \brief Removes all trace values. */
	void RemoveAllValues();
	/*@}*/
	
	
	
	/** \name Convenience Functions */
	/*@{*/
	/** \brief Adds a new trace value with the given information. */
	deErrorTraceValue *AddValue(const char *name, const char *value);
	
	/** \brief Adds a new trace value with the given information. */
	deErrorTraceValue *AddValueInt(const char *name, int value);
	
	/** \brief Adds a new trace value with the given information. */
	deErrorTraceValue *AddValueFloat(const char *name, float);
	
	/** \brief Adds a new trace value with the given information. */
	deErrorTraceValue *AddValueBool(const char *name, bool value);
	/*@}*/
};

#endif
