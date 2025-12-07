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

#ifndef _DEAERRORTRACEVALUE_H_
#define _DEAERRORTRACEVALUE_H_

#include "../common/string/decString.h"


/**
 * \brief Error Trace Value.
 *
 * Error Trace Value objects contain additional information about the values in
 * use by the failing function.
 */
class DE_DLL_EXPORT deErrorTraceValue{
private:
	decString pName;
	decString pValue;
	deErrorTraceValue **pSubValues;
	int pSubValueCount, pSubValueSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new error trace value object with the given name and value. */
	deErrorTraceValue(const char *name, const char *value);
	
	/** \brief Clean up error trace object. */
	~deErrorTraceValue();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name of the value. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Value. */
	inline const decString &GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue(const char *value);
	
	/** \brief Set value from a numeric value. */
	void SetValueFrom(float value);
	/*@}*/

	/** \name Trace SubValue Management */
	/*@{*/
	/** \brief Determines if there exist trace values. */
	inline bool HasSubValues() const{ return pSubValueCount > 0; }
	
	/** \brief Count of trace values. */
	inline int GetSubValueCount() const{ return pSubValueCount; }
	
	/** \brief Indexed trace value. */
	deErrorTraceValue *GetSubValue(int index) const;
	
	/** \brief Named trace value or NULL if not found. */
	deErrorTraceValue *FindSubValue(const char *name) const;
	
	/** \brief Adds a new trace value. */
	void AddSubValue(deErrorTraceValue *value);
	
	/** \brief Removes all trace values. */
	void RemoveAllSubValues();
	/*@}*/
	
	
	
	/** \name Convenience Functions */
	/*@{*/
	/** \brief Adds a new trace value with the given information. */
	deErrorTraceValue *AddSubValue(const char *name, const char *value);
	
	/** \brief Adds a new trace value with the given information. */
	deErrorTraceValue *AddSubValueInt(const char *name, int value);
	
	/** \brief Adds a new trace value with the given information. */
	deErrorTraceValue *AddSubValueFloat(const char *name, float);
	
	/** \brief Adds a new trace value with the given information. */
	deErrorTraceValue *AddSubValueBool(const char *name, bool value);
	/*@}*/
};

#endif
