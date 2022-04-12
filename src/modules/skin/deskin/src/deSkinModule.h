/* 
 * Drag[en]gine Skin Module
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

#ifndef _DESKINMODULE_H_
#define _DESKINMODULE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>
#include <dragengine/systems/modules/skin/deBaseSkinModule.h>

class decXmlElementTag;
class decXmlAttValue;
class decXmlWriter;
class deSkinTexture;
class deSkinProperty;
class deSkinPropertyConstructed;
class decPath;
class decCurveBezier;
class decCurveBezierPoint;
class deSkinPropertyNode;
class deSkinPropertyNodeGroup;
class deSkinPropertyNodeImage;
class deSkinPropertyNodeShape;
class deSkinPropertyNodeText;


// dragengine skin module
class deSkinModule : public deBaseSkinModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deSkinModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deSkinModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load skin. */
	virtual void LoadSkin( decBaseFileReader &reader, deSkin &skin );
	
	/** \brief Save skin. */
	virtual void SaveSkin( decBaseFileWriter &writer, const deSkin &skin );
	
	/**
	 * \brief File is compressed.
	 * \version 1.12
	 * \note Call has to be thread safe.
	 */
	virtual bool IsCompressed( decBaseFileReader &reader );
	/*@}*/
	
private:
	decXmlElementTag *pGetTagAt( const decXmlElementTag &tag, int index );
	decXmlAttValue *pFindAttribute( const decXmlElementTag &tag, const char *name );
	const char *pGetAttributeString( const decXmlElementTag &tag, const char *name );
	int pGetAttributeInt( const decXmlElementTag &tag, const char *name );
	float pGetAttributeFloat( const decXmlElementTag &tag, const char *name );
	bool pGetAttributeBool( const decXmlElementTag &tag, const char *name );
	
	void pParseSkin( const decXmlElementTag &root, deSkin &skin );
	deSkinTexture *pParseTexture( const decXmlElementTag &root, decPath &basePath, deSkin &skin );
	void pParsePropertyMapped( const decXmlElementTag &root, deSkinPropertyMapped &property );
	void pParsePropertyMappedComponent( const decXmlElementTag &root, deSkinPropertyMapped::cComponent &component );
	void pParsePropertyMappedCurve( const decXmlElementTag &root, decCurveBezier &curve );
	void pParsePropertyMappedCurvePoint( const decXmlElementTag &root, decCurveBezier &curve );
	void pParsePropertyConstructed( const decXmlElementTag &root, deSkinPropertyConstructed &property );
	
	deSkinPropertyNode *pParsePropertyNode( const decXmlElementTag &tag );
	bool pParsePropertyNodeCommon( const decXmlElementTag &tag, deSkinPropertyNode &node );
	void pParsePropertyNodeGroup( const decXmlElementTag &root, deSkinPropertyNodeGroup &group );
	void pParsePropertyNodeImage( const decXmlElementTag &root, deSkinPropertyNodeImage &group );
	void pParsePropertyNodeShape( const decXmlElementTag &root, deSkinPropertyNodeShape &group );
	void pParsePropertyNodeText( const decXmlElementTag &root, deSkinPropertyNodeText &group );
	
	decColor pParseColor( const decXmlElementTag &root );
	void pReadVector2( const decXmlElementTag &tag, decVector2 &vector );
	
	void pWriteSkin( decXmlWriter &writer, const deSkin &skin );
	void pWriteTexture( decXmlWriter &writer, const deSkin &skin, const deSkinTexture &texture );
	void pWriteProperty( decXmlWriter& writer, deSkinProperty &property );
};

#endif
