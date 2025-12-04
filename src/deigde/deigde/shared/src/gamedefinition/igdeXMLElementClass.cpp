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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeXMLElementClass.h"
#include "igdeGameDefinition.h"
#include "class/igdeGDClass.h"
#include "class/igdeGDClassInherit.h"
#include "class/igdeGDClassManager.h"
#include "class/component/igdeGDCComponent.h"
#include "class/component/igdeGDCComponentList.h"
#include "class/component/igdeGDCCTexture.h"
#include "class/component/igdeGDCCTextureList.h"
#include "../codec/igdeCodecPropertyString.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deFileSearchVisitor.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>



// Class igdeXMLElementClass
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeXMLElementClass::igdeXMLElementClass( deLogger *logger ) :
igdeBaseXML( logger, "XMLElementClass" ){
}

igdeXMLElementClass::~igdeXMLElementClass(){
}



// Loading
////////////

igdeGDClass *igdeXMLElementClass::LoadElementClass(decBaseFileReader &reader,
const char *filename){
	try{
		decXmlDocumentReference document;
		document.TakeOver( new decXmlDocument );
		
		decXmlParser( GetLogger() ).ParseXml( &reader, document );
		
		document->StripComments();
		document->CleanCharData();
		
		decXmlElementTag * const root = document->GetRoot();
		if( ! root || root->GetName() != "elementClass" ){
			DETHROW( deeInvalidParam );
		}
		
		return pReadElementClass(*root, filename);
		
	}catch( const deException &e ){
		GetLogger()->LogInfoFormat( GetLoggerSource(),
			"Failed loading XML Element Class from file '%s'", reader.GetFilename() );
		GetLogger()->LogException( GetLoggerSource(), e );
		throw;
	}
}

class igdeXMLElementClassProcessEClasses : public deFileSearchVisitor{
private:
	igdeXMLElementClass &pOwner;
	const char * const pPattern;
	igdeGDClassManager &pClasses;
	deLogger &pLogger;
	const decString &pLoggerSource;
	decBaseFileReader::Ref pReader;
	igdeGDClass::Ref pClass;
	
public:
	igdeXMLElementClassProcessEClasses(igdeXMLElementClass &owner, const char *pattern,
		igdeGDClassManager &classes, deLogger &logger, const decString &loggerSource) :
	pOwner(owner), pPattern(pattern), pClasses(classes), pLogger(logger), pLoggerSource(loggerSource){
	}
	
	virtual bool VisitFile( const deVirtualFileSystem &vfs, const decPath &path ){
		if(!path.GetLastComponent().MatchesPattern(pPattern)){
			return true;
		}
		
		try{
			pReader.TakeOver(vfs.OpenFileForReading(path));
			
		}catch(const deException &e){
			pLogger.LogException(pLoggerSource, e);
			return true;
		}
		
		pClass.TakeOver(pOwner.LoadElementClass(pReader, path.GetPathUnix()));
		
		if(pClasses.HasNamed(pClass->GetName())){
			pLogger.LogInfoFormat(pLoggerSource, "Ignore duplicate element class '%s'",
				pClass->GetName().GetString());
			
		}else{
			pClasses.Add(pClass);
		}
		
		return true;
	}
	
	virtual bool VisitDirectory( const deVirtualFileSystem &vfs, const decPath &path ){
		vfs.SearchFiles( path, *this );
		return true;
	}
};

void igdeXMLElementClass::LoadElementClasses(igdeGDClassManager &classes, deVirtualFileSystem &vfs,
const decPath &directory, const char *pattern, bool recursive){
	igdeXMLElementClassProcessEClasses process(*this, pattern, classes, *GetLogger(), GetLoggerSource());
	vfs.SearchFiles(directory, process);
}



// Protected Functions
////////////////////////

decBoundary igdeXMLElementClass::ReadBorderSize( const decXmlElementTag &root ){
	const decXmlAttValue *value;
	decBoundary bs;
	
	value = root.FindAttribute( "all" );
	if( value ){
		bs.x1 = bs.y1 = bs.x2 = bs.y2 = value->GetValue().ToInt();
		return bs;
	}
	
	value = root.FindAttribute( "left" );
	if( value ){
		bs.x1 = value->GetValue().ToInt();
	}
	
	value = root.FindAttribute( "top" );
	if( value ){
		bs.y1 = value->GetValue().ToInt();
	}
	
	value = root.FindAttribute( "right" );
	if( value ){
		bs.x2 = value->GetValue().ToInt();
	}
	
	value = root.FindAttribute( "bottom" );
	if( value ){
		bs.y2 = value->GetValue().ToInt();
	}
	
	return bs;
}

decBoundary igdeXMLElementClass::ReadRectArea( const decXmlElementTag &root ){
	const decXmlAttValue *value;
	decBoundary ra;
	
	value = root.FindAttribute( "x1" );
	if( value ){
		ra.x1 = value->GetValue().ToInt();
	}
	
	value = root.FindAttribute( "y1" );
	if( value ){
		ra.y1 = value->GetValue().ToInt();
	}
	
	value = root.FindAttribute( "x2" );
	if( value ){
		ra.x2 = value->GetValue().ToInt();
	}
	
	value = root.FindAttribute( "y2" );
	if( value ){
		ra.y2 = value->GetValue().ToInt();
	}
	
	return ra;
}

decVector4 igdeXMLElementClass::ReadFloatRectArea( const decXmlElementTag &root ){
	const decXmlAttValue *value;
	decVector4 fra;
	
	value = root.FindAttribute( "x1" );
	if( value ){
		fra.x = value->GetValue().ToFloat();
	}
	
	value = root.FindAttribute( "y1" );
	if( value ){
		fra.y = value->GetValue().ToFloat();
	}
	
	value = root.FindAttribute( "x2" );
	if( value ){
		fra.z = value->GetValue().ToFloat();
	}
	
	value = root.FindAttribute( "y2" );
	if( value ){
		fra.w = value->GetValue().ToFloat();
	}
	
	return fra;
}



// Private Functions
//////////////////////

igdeXMLElementClass::cMap::cMap(){ }
igdeXMLElementClass::cMap::~cMap(){ }

igdeGDClass *igdeXMLElementClass::pReadElementClass(const decXmlElementTag &root, const char *filename){
	decStringDictionary properties;
	deObjectReference refMap;
	int i;
	
	const igdeGDClass::Ref gdClass(igdeGDClass::Ref::New(
		new igdeGDClass(GetAttributeString(root, "name"))));
	gdClass->AddInheritClass(igdeGDClassInherit::Ref::New(
		new igdeGDClassInherit(GetAttributeString(root, "class"))));
	gdClass->SetPathEClass(filename);
	
	decPath basePath( decPath::CreatePathUnix( filename ) );
	basePath.RemoveLastComponent();
	const decString basePathStr( basePath.GetPathUnix() );
	
	refMap.TakeOver( new cMap );
	cMap &map = ( cMap& )( deObject& )refMap;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if(tag->GetName() == "behavior"){
			pReadBehavior(*tag, gdClass, filename, basePathStr);
			continue;
		}
		
		const decString propertyName( GetAttributeString( *tag, "name" ) );
		decString propertyValue;
		
		map.map.RemoveAll();
		
		if( ! pReadPropertyValue( *tag, propertyValue, &map, filename ) ){
			continue;
		}
		
		if( propertyName == "textureReplacements"
		|| propertyName.GetRight( 20 ) == ".textureReplacements" ){
			pProcessTextureReplacements( map, gdClass, basePathStr );
			
		}else{
			properties.SetAt( propertyName, propertyValue );
		}
	}
	
	gdClass->SetPropertyValues( properties );
	
	gdClass->AddReference(); // caller takes over reference
	return gdClass;
}

void igdeXMLElementClass::pReadBehavior(const decXmlElementTag &root,
igdeGDClass &gdClass, const char *filename, const decString &basePathStr){
	const igdeGDClassInherit::Ref inherit(igdeGDClassInherit::Ref::New(
		new igdeGDClassInherit(GetAttributeString(root, "type"))));
	inherit->SetUseAutoPropertyPrefixId(true);
	if(HasAttribute(root, "id")){
		inherit->SetAutoPropertyPrefixId(GetAttributeString(root, "id"));
	}
	
	decStringDictionary &properties = inherit->GetAutoPrefixProperties();
	const cMap::Ref map(cMap::Ref::New(new cMap));
	int i;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString propertyName(GetAttributeString(*tag, "name"));
		if(!propertyName.BeginsWith(".")){
			continue;
		}
		
		decString propertyValue;
		
		map->map.RemoveAll();
		
		if(!pReadPropertyValue(*tag, propertyValue, map, filename)){
			continue;
		}
		
		if(propertyName.EndsWith(".textureReplacements")){
			pProcessTextureReplacements(map, gdClass, basePathStr);
			
		}else{
			properties.SetAt(propertyName.GetMiddle(1), propertyValue);
		}
	}
	
	gdClass.AddInheritClass(inherit);
}

bool igdeXMLElementClass::pReadPropertyValue( const decXmlElementTag &root,
decString &value, cMap *map, const char *filename ){
	const decString &tagName = root.GetName();
	
	if( tagName == "string" ){
		value = GetCDataString( root );
		
	}else if( tagName == "float" ){
		value.Format( "%g", GetCDataFloat( root ) );
		
	}else if( tagName == "integer" ){
		value.Format( "%d", GetCDataInt( root ) );
		
	}else if( tagName == "boolean" ){
		value = GetCDataBool( root ) ? "1" : "0";
		
	}else if( tagName == "vector" ){
		decVector vector;
		ReadVector( root, vector );
		igdeCodecPropertyString().EncodeVector( vector, value );
		
	}else if( tagName == "vector2" ){
		decVector2 vector;
		ReadVector2( root, vector );
		igdeCodecPropertyString().EncodeVector2( vector, value );
		
	}else if( tagName == "point" ){
		decPoint point;
		ReadPoint( root, point );
		value.Format( "%d %d", point.x, point.y );
		
	}else if( tagName == "borderSize" ){
		const decBoundary bs( ReadBorderSize( root ) );
		value.Format( "%d %d %d %d", bs.x1, bs.y1, bs.x2, bs.y2 );
		
	}else if( tagName == "rectArea" ){
		const decBoundary ra( ReadRectArea( root ) );
		value.Format( "%d %d %d %d", ra.x1, ra.y1, ra.x2, ra.y2 );
		
	}else if( tagName == "floatRectArea" ){
		const decVector4 fra( ReadFloatRectArea( root ) );
		value.Format( "%g %g %g %g", fra.x, fra.y, fra.z, fra.w );
		
	}else if( tagName == "color" ){
		decColor color;
		pReadECColor( root, color, filename );
		if(color.a == 1.0f){
			igdeCodecPropertyString().EncodeColor3(color, value);
			
		}else{
			igdeCodecPropertyString().EncodeColor4(color, value);
		}
		
	}else if( tagName == "null" ){
		value.Empty();
		
	}else if( tagName == "list" ){
		decStringList list;
		pReadList( root, list, filename );
		igdeCodecPropertyString().EncodeStringList( list, value );
		
	}else if( tagName == "map" ){
		// maps only are used for element class properties and not for object properties.
		// for this reason it is skipped. special formats like shape and shape list are
		// handled using encoded strings.
		// 
		// there is one exception and this are texture replacements. we handle them
		// explicitly here since we know in this case how they are typically formatted.
		// for this reason maps are read into a string dictionary for this specific case
		if( map ){
			pReadMap( root, *map, filename );
			
		}else{
			return false;
		}
		
	}else{
		GetLogger()->LogWarnFormat( GetLoggerSource(), "%s:%s(%i:%i): Unknown Value %s",
			filename, root.GetName().GetString(), root.GetLineNumber(),
			root.GetPositionNumber(), tagName.GetString() );
		return false;
	}
	
	return true;
}

void igdeXMLElementClass::pReadList( const decXmlElementTag &root, decStringList &list,
const char *filename ){
	// NOTE list-in-list is not well supported and will end up with strange results.
	//      in general though base classes will never use such strange properties so
	//      throwing this result away later on is fine enough
	int i;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		decString propertyValue;
		if( pReadPropertyValue( *tag, propertyValue, NULL, filename ) ){
			list.Add( propertyValue );
		}
	}
}

void igdeXMLElementClass::pReadMap( const decXmlElementTag &root, cMap &map, const char *filename ){
	int i;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const char * const key = GetAttributeString( *tag, "key" );
		deObjectReference refChild;
		refChild.TakeOver( new cMap );
		cMap &child = ( cMap& )( deObject& )refChild;
		
		if( pReadPropertyValue( *tag, child.value, &child, filename ) ){
			map.map.SetAt( key, refChild );
		}
	}
}

void igdeXMLElementClass::pReadECColor( const decXmlElementTag &root, decColor &color,
const char *filename ){
	if( HasAttribute( root, "hex" ) ){
		const decString &hexValue = GetAttributeString( root, "hex" );
		
		const char bufferR[ 3 ] = { hexValue[ 0 ], hexValue[ 1 ], '\0' };
		color.r = ( float )strtol( bufferR, NULL, 16 ) / 255.0f;
		
		const char bufferG[ 3 ] = { hexValue[ 2 ], hexValue[ 3 ], '\0' };
		color.g = ( float )strtol( bufferG, NULL, 16 ) / 255.0f;
		
		const char bufferB[ 3 ] = { hexValue[ 4 ], hexValue[ 5 ], '\0' };
		color.b = ( float )strtol( bufferB, NULL, 16 ) / 255.0f;
		
	}else{
		if( HasAttribute( root, "r" ) ){
			color.r = GetAttributeFloat( root, "r" );
		}
		if( HasAttribute( root, "g" ) ){
			color.g = GetAttributeFloat( root, "g" );
		}
		if( HasAttribute( root, "b" ) ){
			color.b = GetAttributeFloat( root, "b" );
		}
		
		if( HasAttribute( root, "ir" ) ){
			color.r = ( float )GetAttributeInt( root, "ir" ) / 255.0f;
		}
		if( HasAttribute( root, "ig" ) ){
			color.g = ( float )GetAttributeInt( root, "ig" ) / 255.0f;
		}
		if( HasAttribute( root, "ib" ) ){
			color.b = ( float )GetAttributeInt( root, "ib" ) / 255.0f;
		}
	}
	
	// tag versions are deprecated
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "red" ){
			color.r = GetCDataFloat( *tag );
			
		}else if( tagName == "green" ){
			color.g = GetCDataFloat( *tag );
			
		}else if( tagName == "blue" ){
			color.b = GetCDataFloat( *tag );
			
		}else if( tagName == "alpha" ){
			//color.a = GetCDataFloat( *tag );
			
		}else if( tagName == "value" ){
			color.b = color.g = color.r = GetCDataFloat( *tag );
			
		}else if( tagName == "intRed" ){
			color.r = ( float )GetCDataInt( *tag ) / 255.0f;
			
		}else if( tagName == "intGreen" ){
			color.g = ( float )GetCDataInt( *tag ) / 255.0f;
			
		}else if( tagName == "intBlue" ){
			color.b = ( float )GetCDataInt( *tag ) / 255.0f;
			
		}else if( tagName == "intAlpha" ){
			//color.a = ( float )GetCDataInt( *tag ) / 255.0f;
			
		}else if( tagName == "intValue" ){
			color.b = color.g = color.r = ( float )GetCDataInt( *tag ) / 255.0f;
			
		}else{
			GetLogger()->LogWarnFormat( GetLoggerSource(), "%s:%s(%i:%i): Unknown Value %s",
				filename, root.GetName().GetString(), root.GetLineNumber(),
				root.GetPositionNumber(), tagName.GetString() );
		}
	}
}

void igdeXMLElementClass::pProcessTextureReplacements( const cMap &map,
igdeGDClass &gdClass, const char *basePath ){
	const decStringList keys( map.map.GetKeys() );
	const int count = keys.GetCount();
	if( count == 0 ){
		return;
	}
	
	igdeGDCCTextureList &textures = gdClass.GetComponentTextures();
	igdeCodecPropertyString codec;
	deObjectReference refTexture;
	deObject* object;
	int i;
	
	for( i=0; i<count; i++ ){
		const decString &key = keys.GetAt( i );
		const cMap &child = *( ( cMap* )map.map.GetAt( key ) );
		
		refTexture.TakeOver( new igdeGDCCTexture );
		igdeGDCCTexture &texture = ( igdeGDCCTexture& )( deObject& )refTexture;
		texture.SetName( key );
		
		if( child.map.GetAt( "skin", &object ) ){
			texture.SetPathSkin( decPath::AbsolutePathUnix(
				( ( cMap* )object )->value, basePath ).GetPathUnix() );
		}
		
		if( child.map.GetAt( "tint", &object ) ){
			decColor color;
			codec.DecodeColor3( ( ( cMap* )object )->value, color );
			texture.SetColorTint( color );
		}
		
		if( child.map.GetAt( "transform", &object ) ){
			const cMap &child2 = *( ( cMap* )object );
			
			decVector2 translate;
			if( child2.map.GetAt( "translate", &object ) ){
				codec.DecodeVector2( ( ( cMap* )object )->value, translate );
			}
			texture.SetOffset( translate );
			
			decVector2 scale( 1.0f, 1.0f );
			if( child2.map.GetAt( "scale", &object ) ){
				codec.DecodeVector2( ( ( cMap* )object )->value, scale );
			}
			texture.SetScale( scale );
			
			float rotate = 0.0f;
			if( child2.map.GetAt( "rotate", &object ) ){
				rotate = ( ( cMap* )object )->value.ToFloat();
			}
			texture.SetRotation( rotate );
		}
		
		textures.Add( &texture );
	}
}

/*
igdeGDCComponent &igdeXMLElementClass::pGetLoadedComponent( igdeGDClass &gdClass ){
	if( gdClass.GetComponentList().GetCount() > 0 ){
		return *gdClass.GetComponentList().GetAt( 0 );
	}
	
	deObjectReference refComponent;
	refComponent.TakeOver( new igdeGDCComponent );
	
	igdeGDCComponent &component = ( igdeGDCComponent& )( deObject& )refComponent;
	gdClass.AddComponent( &component );
	return component;
}
*/
