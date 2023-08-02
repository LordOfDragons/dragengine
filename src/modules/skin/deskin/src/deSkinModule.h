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
#include <dragengine/resources/skin/deSkinMapped.h>
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
class deSkinPropertyMapped;
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
	/*@}*/
	
private:
	decXmlElementTag *pGetTagAt( const decXmlElementTag &tag, int index );
	decXmlAttValue *pFindAttribute( const decXmlElementTag &tag, const char *name );
	const char *pGetAttributeString( const decXmlElementTag &tag, const char *name );
	int pGetAttributeInt( const decXmlElementTag &tag, const char *name );
	float pGetAttributeFloat( const decXmlElementTag &tag, const char *name );
	bool pGetAttributeBool( const decXmlElementTag &tag, const char *name );
	
	void pParseSkin( const decXmlElementTag &root, deSkin &skin );
	
	deSkinMapped::Ref pParseMapped( const decXmlElementTag &root, const char *forceName = nullptr );
	void pParseMappedCurve( const decXmlElementTag &root, decCurveBezier &curve );
	void pParseMappedCurvePoint( const decXmlElementTag &root, decCurveBezier &curve );
	
	deSkinTexture *pParseTexture( const decXmlElementTag &root, decPath &basePath, deSkin &skin );
	void pParsePropertyMapped( const decXmlElementTag &root, deSkin &skin, deSkinPropertyMapped &property );
	void pParsePropertyConstructed( const decXmlElementTag &root, const deSkin &skin, deSkinPropertyConstructed &property );
	
	deSkinPropertyNode *pParsePropertyNode( const decXmlElementTag &tag, const deSkin &skin );
	bool pParsePropertyNodeCommon( const decXmlElementTag &tag, const deSkin &skin, deSkinPropertyNode &node );
	void pParsePropertyNodeGroup( const decXmlElementTag &root, const deSkin &skin, deSkinPropertyNodeGroup &group );
	void pParsePropertyNodeImage( const decXmlElementTag &root, const deSkin &skin, deSkinPropertyNodeImage &group );
	void pParsePropertyNodeShape( const decXmlElementTag &root, const deSkin &skin, deSkinPropertyNodeShape &group );
	void pParsePropertyNodeText( const decXmlElementTag &root, const deSkin &skin, deSkinPropertyNodeText &group );
	
	decColor pParseColor( const decXmlElementTag &root );
	void pReadVector2( const decXmlElementTag &tag, decVector2 &vector );
	
	void pWriteSkin( decXmlWriter &writer, const deSkin &skin );
	void pWriteMapped( decXmlWriter &writer, const deSkinMapped &mapped );
	void pWriteTexture( decXmlWriter &writer, const deSkin &skin, const deSkinTexture &texture );
	void pWriteProperty( decXmlWriter& writer, const deSkin &skin, deSkinProperty &property );
};

#endif
