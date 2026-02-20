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

#include "../dragengine_export.h"
#include "../common/string/decString.h"
#include "../common/collection/decTUniqueList.h"


/**
 * \brief Error Trace Value.
 *
 * Error Trace Value objects contain additional information about the values in
 * use by the failing function.
 */
class DE_DLL_EXPORT deErrorTraceValue{
public:
	/** \brief Reference. */
	using Ref = deTUniqueReference<deErrorTraceValue>;
	
	/** \brief List. */
	using List = decTUniqueList<deErrorTraceValue>;
	
private:
	decString pName;
	decString pValue;
	List pSubValues;
	
	
	
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
	/** \brief Sub values. */
	inline const List &GetSubValues() const{ return pSubValues; }
	
	/** \brief Determines if the value has sub values. */
	inline bool HasSubValues() const{ return pSubValues.IsNotEmpty(); }
	
	/** \brief Count of trace values. */
	inline int GetSubValueCount() const{ return pSubValues.GetCount(); }
	
	/** \brief Indexed trace value. */
	const deErrorTraceValue::Ref &GetSubValue(int index) const;
	
	/** \brief Named trace value or NULL if not found. */
	deErrorTraceValue *FindSubValue(const char *name) const;
	
	/** \brief Add a new sub value. */
	void AddSubValue(deErrorTraceValue::Ref &&value);
	
	/** \brief Removes all trace values. */
	void RemoveAllSubValues();
	/*@}*/
	
	
	
	/** \name Convenience Functions */
	/*@{*/
	/** \brief Adds a new trace value with the given information. */
	const deErrorTraceValue::Ref &AddSubValue(const char *name, const char *value);
	
	/** \brief Adds a new trace value with the given information. */
	const deErrorTraceValue::Ref &AddSubValueInt(const char *name, int value);
	
	/** \brief Adds a new trace value with the given information. */
	const deErrorTraceValue::Ref &AddSubValueFloat(const char *name, float);
	
	/** \brief Adds a new trace value with the given information. */
	const deErrorTraceValue::Ref &AddSubValueBool(const char *name, bool value);
	/*@}*/
};

#endif
