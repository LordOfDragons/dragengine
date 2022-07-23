/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEBASEXML_H_
#define _IGDEBASEXML_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class decCurveBezierPoint;
class decCurveBezier;
class deException;
class decXmlWriter;
class decXmlElementTag;
class deLogger;



/**
 * \brief Basic XML Load/Save Class.
 * 
 * Class used as base class for xml load/save classes. Provides a set of helper
 * function required by all xml load/save classes.
 * 
 */
class igdeBaseXML{
private:
	deLogger *pLogger;
	decString pLoggerSource;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create base xml read/save. */
	igdeBaseXML( deLogger *logger, const char *loggerSource );
	
	/** \brief Clean up base xml read/save. */
	virtual ~igdeBaseXML();
	/*@}*/
	
	
	
protected:
	/** \name Management */
	/*@{*/
	/** \brief Logger. */
	inline deLogger *GetLogger() const{ return pLogger; }
	
	/** \brief Logger source. */
	inline const decString &GetLoggerSource() const{ return pLoggerSource; }
	
	
	
	/** \brief Attribute is present. */
	bool HasAttribute( const decXmlElementTag &tag, const char *name ) const;
	
	/** \brief Value of named attribute or throws an exception if absent. */
	const decString &GetAttributeString( const decXmlElementTag &tag, const char *name ) const;
	
	/** \brief Integer value of named attribute or throws an exception if absent. */
	int GetAttributeInt( const decXmlElementTag &tag, const char *name ) const;
	
	/** \brief Float value of named attribute or throws an exception if absent. */
	float GetAttributeFloat( const decXmlElementTag &tag, const char *name ) const;
	
	/** \brief Double value of named attribute or throws an exception if absent. */
	double GetAttributeDouble( const decXmlElementTag &tag, const char *name ) const;
	
	/** \brief Boolean value of named attribute or throws an exception if absent. */
	bool GetAttributeBool( const decXmlElementTag &tag, const char *name ) const;
	
	
	
	/** \brief Bool CDATA of tag or false if absent. */
	bool GetCDataBool( const decXmlElementTag &tag ) const;
	
	/** \brief Integer CDATA of tag or 0 if absent. */
	int GetCDataInt( const decXmlElementTag &tag ) const;
	
	/** \brief Float CDATA of tag or 0.0 if absent. */
	float GetCDataFloat( const decXmlElementTag &tag ) const;
	
	/** \brief Double CDATA of tag or 0.0 if absent. */
	double GetCDataDouble( const decXmlElementTag &tag ) const;
	
	/** \brief String CDATA of tag or an empty string if absent. */
	const char *GetCDataString( const decXmlElementTag &tag ) const;
	
	
	
	/**
	 * \brief Multi-line string or an empty string if absent.
	 * 
	 * Multi-line strings support br-tags to add line-breaks. Parsing stops if no more CDATA
	 * are present or a non-br tag is encountered.
	 */
	decString ReadMultilineString( const decXmlElementTag &tag );
	
	/** \brief Read double vector. */
	void ReadDVector( const decXmlElementTag &tag, decDVector &vector );
	
	/** \brief Read vector. */
	void ReadVector( const decXmlElementTag &tag, decVector &vector );
	
	/** \brief Read vector. */
	void ReadVector2( const decXmlElementTag &tag, decVector2 &vector );
	
	/** \brief Read bezier curve. */
	void ReadCurveBezier( const decXmlElementTag &root, decCurveBezier &curve );
	
	/** \brief Read bezier curve point. */
	void ReadCurveBezierPoint( const decXmlElementTag &root, decCurveBezier &curve );
	
	/** \brief Read color. */
	void ReadColor( const decXmlElementTag &tag, decColor &color );
	
	/** \brief Read color. */
	void ReadColor4( const decXmlElementTag &tag, decColor &color );
	
	/** \brief Read point. */
	void ReadPoint( const decXmlElementTag &tag, decPoint &point );
	
	/** \brief Read point. */
	void ReadPoint3( const decXmlElementTag &tag, decPoint3 &point );
	
	
	
	/** \brief Write multi-line string. */
	void WriteMultilineString( decXmlWriter &writer, const char *name, const char *string );
	
	/** \brief Write multi-line string. */
	void WriteTextMultilineString( decXmlWriter &writer, const char *string );
	
	/** \brief Write double vector. */
	void WriteDVector( decXmlWriter &writer, const char *name, const decDVector &vector );
	
	/** \brief Write vector. */
	void WriteVector( decXmlWriter &writer, const char *name, const decVector &vector );
	
	/** \brief Read vector. */
	void WriteVector2( decXmlWriter &writer, const char *name, const decVector2 &vector );
	
	/** \brief Write bezier curve. */
	void WriteCurveBezier( decXmlWriter &writer, const char *name, const decCurveBezier &curve );
	
	/** \brief Write bezier curve point. */
	void WriteCurveBezierPoint( decXmlWriter &writer, const char *name, const decCurveBezierPoint &point );
	
	/** \brief Write bezier curve point without handles. */
	void WriteCurveBezierPointNoHandles( decXmlWriter &writer, const char *name, const decCurveBezierPoint &point );
	
	/** \brief Write color. */
	void WriteColor( decXmlWriter &writer, const char *name, const decColor &color );
	
	/** \brief Write color. */
	void WriteColor4( decXmlWriter &writer, const char *name, const decColor &color );
	
	
	
	/** \brief Log warning about an unknown tag. */
	void LogWarnUnknownTag( const decXmlElementTag &root, const decXmlElementTag &tag );
	
	/** \brief Log error about an unknown tag and throws an exception. */
	void LogErrorUnknownTag( const decXmlElementTag &root, const decXmlElementTag &tag );
	
	/** \brief Log warning about an unknown value. */
	void LogWarnUnknownValue( const decXmlElementTag &tag, const char *value );
	
	/** \brief Log error about an unknown value and throws an exception. */
	void LogErrorUnknownValue( const decXmlElementTag &tag, const char *value );
	
	/** \brief Log warning about an generic problem with a tag. */
	void LogWarnGenericProblem( const decXmlElementTag &tag, const char *problem );
	
	/** \brief Log error about an generic problem with a value and throws an exception. */
	void LogErrorGenericProblem( const decXmlElementTag &tag, const char *problem );
	
	/** \brief Log warning about an generic problem with a value. */
	void LogWarnGenericProblemValue( const decXmlElementTag &tag, const char *value, const char *problem );
	
	/** \brief Log error about an generic problem with a value and throws an exception. */
	void LogErrorGenericProblemValue( const decXmlElementTag &tag, const char *value, const char *problem );
	
	/** \brief \brief Log warning about an exception while processing a tag. */
	void LogWarnExceptionTag( const decXmlElementTag &tag, const deException &exception );
	
	/** \brief Log error about an exception while processing a tag. */
	void LogErrorExceptionTag( const decXmlElementTag &tag, const deException &exception );
	
	
	
	/** \brief Log warning about a missing tag. */
	void LogWarnMissingTag( const decXmlElementTag &root, const char *tagName );
	
	/** \brief Log error about a missing tag and throws an exception. */
	void LogErrorMissingTag( const decXmlElementTag &root, const char *tagName );
	
	/** \brief Log warning about a duplicate tag. */
	void LogWarnDuplicateTag( const decXmlElementTag &root, const char *tagName );
	
	/** \brief Log error about a duplicate tag and throws an exception. */
	void LogErrorDuplicateTag( const decXmlElementTag &root, const char *tagName );
	
	/** \brief Log warning about a generic problem with a tag. */
	void LogWarnGenericProblemTag( const decXmlElementTag &root, const char *tagName, const char *problem );
	
	/** \brief Log error about a generic problem with a tag and throws an exception. */
	void LogErrorGenericProblemTag( const decXmlElementTag &root, const char *tagName, const char *problem );
	/*@}*/
};

#endif
