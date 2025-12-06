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

#include "meLSXMLHeightTerrain.h"
#include "../meLoadSaveSystem.h"

#include "../../gui/meWindowMain.h"
#include "../../world/meWorld.h"
#include "../../world/decal/meDecal.h"
#include "../../world/heightterrain/meHTVegetationLayer.h"
#include "../../world/heightterrain/meHTVVariation.h"
#include "../../world/heightterrain/rules/meHTVRule.h"
#include "../../world/heightterrain/rules/meHTVRuleClosestProp.h"
#include "../../world/heightterrain/rules/meHTVRuleClosestVegetation.h"
#include "../../world/heightterrain/rules/meHTVRuleCombine.h"
#include "../../world/heightterrain/rules/meHTVRuleComponents.h"
#include "../../world/heightterrain/rules/meHTVRuleCurve.h"
#include "../../world/heightterrain/rules/meHTVRuleGeometry.h"
#include "../../world/heightterrain/rules/meHTVRuleMapping.h"
#include "../../world/heightterrain/rules/meHTVRuleMath.h"
#include "../../world/heightterrain/rules/meHTVRuleMultiMath.h"
#include "../../world/heightterrain/rules/meHTVRuleRandom.h"
#include "../../world/heightterrain/rules/meHTVRuleResult.h"
#include "../../world/heightterrain/rules/meHTVRuleVectorMath.h"
#include "../../world/heightterrain/rules/meHTVRuleConstant.h"
#include "../../world/heightterrain/rules/meHTVRulePropCount.h"
#include "../../world/heightterrain/rules/meHTVRLink.h"
#include "../../world/heightterrain/rules/meHTVRSlot.h"
#include "../../world/object/meObject.h"
#include "../../world/terrain/meHeightTerrainSector.h"
#include "../../world/terrain/meHeightTerrainTexture.h"
#include "../../world/terrain/meHeightTerrainPFLayer.h"
#include "../../world/terrain/meHeightTerrainPFType.h"
#include "../../world/terrain/meHeightTerrainNavSpace.h"
#include "../../world/terrain/meHeightTerrainNavSpaceType.h"
#include "../../world/terrain/meHeightTerrain.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meLSXMLHeightTerrain
///////////////////////////////

// Constructor, destructor
////////////////////////////

meLSXMLHeightTerrain::meLSXMLHeightTerrain( meLoadSaveSystem *lssys ) :
meLSHeightTerrain( lssys->GetWindowMain()->GetLogger(), "LSXMLHeightTerrain" )
{
	if( ! lssys ) DETHROW( deeInvalidParam );
	
	pLSSys = lssys;
	
	SetName( "Drag[en]gine XML Height Terrain Sector" );
	SetPattern( "*.hts.xml" );
}

meLSXMLHeightTerrain::~meLSXMLHeightTerrain(){
}



// Management
///////////////

void meLSXMLHeightTerrain::LoadFromFile( meHeightTerrain &heightTerrain, decBaseFileReader &file ){
	pLSSys->GetWindowMain()->GetLogger()->LogInfoFormat( LOGSOURCE,
		"XMLHeightTerrain: load height terrain %s", file.GetFilename() );
	
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser( pLSSys->GetWindowMain()->GetEnvironment().GetLogger() ).ParseXml( &file, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || root->GetName() != "heightTerrain" ){
		DETHROW( deeInvalidParam );
	}
	
	pLoadHeightTerrain( *root, heightTerrain );
}

void meLSXMLHeightTerrain::SaveToFile( meHeightTerrain &heightTerrain, decBaseFileWriter &file ){
	decXmlWriter writer( &file );
	writer.WriteXMLDeclaration();
	pSaveHeightTerrain( writer, heightTerrain );
}



// Private Functions
//////////////////////

void meLSXMLHeightTerrain::pSaveHeightTerrain( decXmlWriter &writer, meHeightTerrain &heightTerrain ){
	writer.WriteOpeningTagStart( "heightTerrain" );
	writer.WriteAttributeFloat( "sectorSize", heightTerrain.GetSectorSize() );
	writer.WriteAttributeInt( "sectorResolution", heightTerrain.GetSectorResolution() );
	writer.WriteOpeningTagEnd();
	
	writer.WriteDataTagFloat( "baseHeight", heightTerrain.GetBaseHeight() );
	writer.WriteDataTagFloat( "heightScaling", heightTerrain.GetHeightScaling() );
	
	const int sectorCount = heightTerrain.GetSectorCount();
	int i;
	for( i=0; i<sectorCount; i++ ){
		pSaveSector( writer, *heightTerrain.GetSectorAt( i ) );
	}
	
	// vegetation layers
	const int layerCount = heightTerrain.GetVLayerCount();
	for( i=0; i<layerCount; i++ ){
		pSaveVLayer( writer, *heightTerrain.GetVLayerAt( i ) );
	}
	
	writer.WriteClosingTag( "heightTerrain" );
}

void meLSXMLHeightTerrain::pSaveSector( decXmlWriter &writer, meHeightTerrainSector &sector ){
	writer.WriteOpeningTagStart( "sector" );
	writer.WriteAttributeInt( "x", sector.GetCoordinates().x );
	writer.WriteAttributeInt( "z", sector.GetCoordinates().y );
	writer.WriteOpeningTagEnd();
	
	int i;
	
	if( ! sector.GetPathHeightImage().IsEmpty() ){
		writer.WriteDataTagString( "heightImage", sector.GetPathHeightImage() );
	}
	if( ! sector.GetPathVisibilityImage().IsEmpty() ){
		writer.WriteDataTagString( "visibilityImage", sector.GetPathVisibilityImage() );
	}
	if( ! sector.GetPathPFCacheImage().IsEmpty() ){
		writer.WriteDataTagString( "pfcache", sector.GetPathPFCacheImage() );
	}
	
	const int textureCount = sector.GetTextureCount();
	for( i=0; i<textureCount; i++ ){
		pSaveTexture( writer, *sector.GetTextureAt( i ) );
	}
	
	const int navSpaceCount = sector.GetNavSpaceCount();
	for( i=0; i<navSpaceCount; i++ ){
		pSaveNavSpace( writer, *sector.GetNavSpaceAt( i ) );
	}
	
	writer.WriteClosingTag( "sector" );
}

void meLSXMLHeightTerrain::pSaveTexture( decXmlWriter &writer, const meHeightTerrainTexture &texture ){
	writer.WriteOpeningTagStart( "texture" );
	writer.WriteAttributeString( "name", texture.GetName() );
	writer.WriteOpeningTagEnd();
	
	writer.WriteDataTagString( "skin", texture.GetPathSkin() );
	
	writer.WriteDataTagInt( "typeNumber", texture.GetTypeNumber() );
	
	writer.WriteOpeningTagStart( "uvOffset" );
	writer.WriteAttributeFloat( "u", texture.GetProjectionOffsetU() );
	writer.WriteAttributeFloat( "v", texture.GetProjectionOffsetV() );
	writer.WriteOpeningTagEnd( true );
	
	writer.WriteOpeningTagStart( "uvScaling" );
	writer.WriteAttributeFloat( "u", texture.GetProjectionScalingU() );
	writer.WriteAttributeFloat( "v", texture.GetProjectionScalingV() );
	writer.WriteOpeningTagEnd( true );
	
	writer.WriteDataTagFloat( "uvRotation", texture.GetProjectionRotation() );
	if( ! texture.GetPathMask().IsEmpty() ){
		writer.WriteDataTagString( "mask", texture.GetPathMask() );
	}
	
	writer.WriteClosingTag( "texture" );
}

void meLSXMLHeightTerrain::pSaveNavSpace( decXmlWriter &writer, const meHeightTerrainNavSpace &navspace ){
	writer.WriteOpeningTagStart( "navigationSpace" );
	writer.WriteAttributeString( "name", navspace.GetName() );
	writer.WriteOpeningTagEnd();
	
	writer.WriteDataTagInt( "layer", navspace.GetLayer() );
	writer.WriteDataTagFloat( "snapDistance", navspace.GetSnapDistance() );
	writer.WriteDataTagFloat( "snapAngle", navspace.GetSnapAngle() );
	
	const int typeCount = navspace.GetTypeCount();
	int i;
	for( i=0; i<typeCount; i++ ){
		pSaveNavSpaceType( writer, *navspace.GetTypeAt( i ) );
	}
	
	if( ! navspace.GetPathNavSpace().IsEmpty() ){
		writer.WriteDataTagString( "path", navspace.GetPathNavSpace() );
	}
	
	writer.WriteClosingTag( "navigationSpace" );
}

void meLSXMLHeightTerrain::pSaveNavSpaceType( decXmlWriter &writer, const meHeightTerrainNavSpaceType &navtype ){
	writer.WriteOpeningTagStart( "type" );
	writer.WriteAttributeString( "name", navtype.GetName() );
	writer.WriteOpeningTagEnd();
	
	writer.WriteDataTagInt( "type", navtype.GetType() );
	
	writer.WriteOpeningTagStart( "color" );
	writer.WriteAttributeFloat( "r", navtype.GetColor().r );
	writer.WriteAttributeFloat( "g", navtype.GetColor().g );
	writer.WriteAttributeFloat( "b", navtype.GetColor().b );
	writer.WriteOpeningTagEnd( true );
	
	writer.WriteClosingTag( "type" );
}

void meLSXMLHeightTerrain::pSaveVLayer( decXmlWriter &writer, const meHTVegetationLayer &vlayer ){
	int v, variationCount = vlayer.GetVariationCount();
	int r, ruleCount = vlayer.GetRuleCount();
	int l, linkCount = vlayer.GetLinkCount();
	int ruleType;
	
	writer.WriteOpeningTag( "vegetationLayer" );
	
	if( ! vlayer.GetName().IsEmpty() ){
		writer.WriteDataTagString( "name", vlayer.GetName() );
	}
	
	const decVector2 &viewCenter = vlayer.GetViewCenter();
	writer.WriteOpeningTagStart( "viewCenter" );
	writer.WriteAttributeFloat( "x", viewCenter.x );
	writer.WriteAttributeFloat( "y", viewCenter.y );
	writer.WriteOpeningTagEnd( true );
	
	for( v=0; v<variationCount; v++ ){
		pSaveVLayerVariation( writer, *vlayer.GetVariationAt( v ) );
	}
	
	writer.WriteOpeningTag( "rules" );
	for( r=0; r<ruleCount; r++ ){
		const meHTVRule &rule = *vlayer.GetRuleAt( r );
		ruleType = rule.GetType();
		
		if( ruleType == meHTVRule::ertClosestProp ){
			pSaveVLayerRuleClosestProp( writer, ( meHTVRuleClosestProp& )rule );
			
		}else if( ruleType == meHTVRule::ertClosestVegetation ){
			pSaveVLayerRuleClosestVegetation( writer, ( meHTVRuleClosestVegetation& )rule );
			
		}else if( ruleType == meHTVRule::ertCombine ){
			pSaveVLayerRuleCombine( writer, ( meHTVRuleCombine& )rule );
			
		}else if( ruleType == meHTVRule::ertComponents ){
			pSaveVLayerRuleComponents( writer, ( meHTVRuleComponents& )rule );
			
		}else if( ruleType == meHTVRule::ertCurve ){
			pSaveVLayerRuleCurve( writer, ( meHTVRuleCurve& )rule );
			
		}else if( ruleType == meHTVRule::ertGeometry ){
			pSaveVLayerRuleGeometry( writer, ( meHTVRuleGeometry& )rule );
			
		}else if( ruleType == meHTVRule::ertMapping ){
			pSaveVLayerRuleMapping( writer, ( meHTVRuleMapping& )rule );
			
		}else if( ruleType == meHTVRule::ertMath ){
			pSaveVLayerRuleMath( writer, ( meHTVRuleMath& )rule );
			
		}else if( ruleType == meHTVRule::ertMultiMath ){
			pSaveVLayerRuleMultiMath( writer, ( meHTVRuleMultiMath& )rule );
			
		}else if( ruleType == meHTVRule::ertRandom ){
			pSaveVLayerRuleRandom( writer, ( meHTVRuleRandom& )rule );
			
		}else if( ruleType == meHTVRule::ertResult ){
			pSaveVLayerRuleResult( writer, ( meHTVRuleResult& )rule );
			
		}else if( ruleType == meHTVRule::ertVectorMath ){
			pSaveVLayerRuleVectorMath( writer, ( meHTVRuleVectorMath& )rule );
			
		}else if( ruleType == meHTVRule::ertConstant ){
			pSaveVLayerRuleConstant( writer, ( meHTVRuleConstant& )rule );
			
		}else if( ruleType == meHTVRule::ertPropCount ){
			pSaveVLayerRulePropCount( writer, ( meHTVRulePropCount& )rule );
			
		}else{
			DETHROW( deeInvalidParam );
		}
	}
	writer.WriteClosingTag( "rules" );
	
	for( l=0; l<linkCount; l++ ){
		pSaveVLayerLink( writer, vlayer, *vlayer.GetLinkAt( l ) );
	}
	
	writer.WriteClosingTag( "vegetationLayer" );
}

void meLSXMLHeightTerrain::pSaveVLayerVariation( decXmlWriter &writer, const meHTVVariation &variation ){
	writer.WriteOpeningTag( "variation" );
	
	if( ! variation.GetPathModel().IsEmpty() ){
		writer.WriteDataTagString( "model", variation.GetPathModel() );
	}
	
	if( ! variation.GetPathSkin().IsEmpty() ){
		writer.WriteDataTagString( "skin", variation.GetPathSkin() );
	}
	
	writer.WriteDataTagFloat( "rotationPerForce", variation.GetRotationPerForce() );
	writer.WriteDataTagFloat( "restitution", variation.GetRestitution() );
	
	writer.WriteOpeningTagStart( "randomRotation" );
	writer.WriteAttributeFloat( "min", variation.GetMinimumRandomRotation() );
	writer.WriteAttributeFloat( "max", variation.GetMaximumRandomRotation() );
	writer.WriteOpeningTagEnd( true );
	
	writer.WriteOpeningTagStart( "randomScaling" );
	writer.WriteAttributeFloat( "min", variation.GetMinimumRandomScaling() );
	writer.WriteAttributeFloat( "max", variation.GetMaximumRandomScaling() );
	writer.WriteOpeningTagEnd( true );
	
	writer.WriteClosingTag( "variation" );
}

void meLSXMLHeightTerrain::pSaveVLayerRule( decXmlWriter &writer, const meHTVRule &rule ){
	if( ! rule.GetName().IsEmpty() ){
		writer.WriteDataTagString( "name", rule.GetName() );
	}
	
	const decVector2 &position = rule.GetPosition();
	writer.WriteOpeningTagStart( "position" );
	writer.WriteAttributeFloat( "x", position.x );
	writer.WriteAttributeFloat( "y", position.y );
	writer.WriteOpeningTagEnd( true );
	
	writer.WriteDataTagInt( "showParameters", rule.GetShowParameters() ? 1 : 0 );
}

void meLSXMLHeightTerrain::pSaveVLayerRuleClosestProp( decXmlWriter &writer, const meHTVRuleClosestProp &rule ){
	writer.WriteOpeningTag( "closestProp" );
	
	pSaveVLayerRule( writer, rule );
	
	if( ! rule.GetPropClass().IsEmpty() ){
		writer.WriteDataTagString( "propClass", rule.GetPropClass() );
	}
	
	writer.WriteDataTagFloat( "searchRadius", rule.GetSearchRadius() );
	
	writer.WriteClosingTag( "closestProp" );
}

void meLSXMLHeightTerrain::pSaveVLayerRuleClosestVegetation( decXmlWriter &writer, const meHTVRuleClosestVegetation &rule ){
	writer.WriteOpeningTag( "closestVegetation" );
	
	pSaveVLayerRule( writer, rule );
	
	if( ! rule.GetVegetationType().IsEmpty() ){
		writer.WriteDataTagString( "vegetationType", rule.GetVegetationType() );
	}
	
	writer.WriteDataTagFloat( "searchRadius", rule.GetSearchRadius() );
	
	writer.WriteClosingTag( "closestVegetation" );
}

void meLSXMLHeightTerrain::pSaveVLayerRuleCombine( decXmlWriter &writer, const meHTVRuleCombine &rule ){
	writer.WriteOpeningTag( "combine" );
	
	pSaveVLayerRule( writer, rule );
	
	writer.WriteDataTagFloat( "x", rule.GetX() );
	writer.WriteDataTagFloat( "y", rule.GetY() );
	writer.WriteDataTagFloat( "z", rule.GetZ() );
	
	writer.WriteClosingTag( "combine" );
}

void meLSXMLHeightTerrain::pSaveVLayerRuleComponents( decXmlWriter &writer, const meHTVRuleComponents &rule ){
	writer.WriteOpeningTag( "components" );
	
	pSaveVLayerRule( writer, rule );
	
	const decVector &vector = rule.GetVector();
	writer.WriteOpeningTagStart( "vector" );
	writer.WriteAttributeFloat( "x", vector.x );
	writer.WriteAttributeFloat( "y", vector.y );
	writer.WriteAttributeFloat( "z", vector.z );
	writer.WriteOpeningTagEnd( true );
	
	writer.WriteClosingTag( "components" );
}

void meLSXMLHeightTerrain::pSaveVLayerRuleCurve( decXmlWriter &writer, const meHTVRuleCurve &rule ){
	WriteCurveBezier( writer, "curve", rule.GetCurve() );
}

void meLSXMLHeightTerrain::pSaveVLayerRuleGeometry( decXmlWriter &writer, const meHTVRuleGeometry &rule ){
	writer.WriteOpeningTag( "geometry" );
	
	pSaveVLayerRule( writer, rule );
	
	writer.WriteClosingTag( "geometry" );
}

void meLSXMLHeightTerrain::pSaveVLayerRuleMapping( decXmlWriter &writer, const meHTVRuleMapping &rule ){
	writer.WriteOpeningTag( "mapping" );
	
	pSaveVLayerRule( writer, rule );
	
	writer.WriteDataTagFloat( "lower", rule.GetLower() );
	writer.WriteDataTagFloat( "upper", rule.GetUpper() );
	writer.WriteDataTagFloat( "value", rule.GetValue() );
	
	writer.WriteDataTagInt( "inversed", rule.GetInversed() ? 1 : 0 );
	
	writer.WriteClosingTag( "mapping" );
}

void meLSXMLHeightTerrain::pSaveVLayerRuleMath( decXmlWriter &writer, const meHTVRuleMath &rule ){
	int oper = rule.GetOperator();
	
	writer.WriteOpeningTag( "math" );
	
	pSaveVLayerRule( writer, rule );
	
	if( oper == meHTVRuleMath::eopAdd ){
		writer.WriteDataTagString( "operator", "add" );
		
	}else if( oper == meHTVRuleMath::eopSubtract ){
		writer.WriteDataTagString( "operator", "subtract" );
		
	}else if( oper == meHTVRuleMath::eopMultiply ){
		writer.WriteDataTagString( "operator", "multiply" );
		
	}else if( oper == meHTVRuleMath::eopDivide ){
		writer.WriteDataTagString( "operator", "divide" );
		
	}else if( oper == meHTVRuleMath::eopSine ){
		writer.WriteDataTagString( "operator", "sine" );
		
	}else if( oper == meHTVRuleMath::eopCosine ){
		writer.WriteDataTagString( "operator", "cosine" );
		
	}else if( oper == meHTVRuleMath::eopTangent ){
		writer.WriteDataTagString( "operator", "tangent" );
		
	}else if( oper == meHTVRuleMath::eopArcSine ){
		writer.WriteDataTagString( "operator", "arcsine" );
		
	}else if( oper == meHTVRuleMath::eopArcCosine ){
		writer.WriteDataTagString( "operator", "arccosine" );
		
	}else if( oper == meHTVRuleMath::eopArcTangent ){
		writer.WriteDataTagString( "operator", "arctangent" );
		
	}else if( oper == meHTVRuleMath::eopPower ){
		writer.WriteDataTagString( "operator", "power" );
		
	}else if( oper == meHTVRuleMath::eopExponential ){
		writer.WriteDataTagString( "operator", "exponential" );
		
	}else if( oper == meHTVRuleMath::eopLogarithm ){
		writer.WriteDataTagString( "operator", "logarithm" );
		
	}else if( oper == meHTVRuleMath::eopMinimum ){
		writer.WriteDataTagString( "operator", "minimum" );
		
	}else if( oper == meHTVRuleMath::eopMaximum ){
		writer.WriteDataTagString( "operator", "maximum" );
		
	}else if( oper == meHTVRuleMath::eopRound ){
		writer.WriteDataTagString( "operator", "round" );
		
	}else if( oper == meHTVRuleMath::eopLessThan ){
		writer.WriteDataTagString( "operator", "lessthan" );
		
	}else if( oper == meHTVRuleMath::eopGreaterThan ){
		writer.WriteDataTagString( "operator", "greaterthan" );
		
	}else if( oper == meHTVRuleMath::eopEqual ){
		writer.WriteDataTagString( "operator", "equal" );
		
	}else if( oper == meHTVRuleMath::eopNotEqual ){
		writer.WriteDataTagString( "operator", "notequal" );
		
	}else{ // oper == meHTVRuleMath::eopAverage
		writer.WriteDataTagString( "operator", "average" );
	}
	
	writer.WriteDataTagFloat( "value1", rule.GetValueA() );
	writer.WriteDataTagFloat( "value2", rule.GetValueB() );
	
	writer.WriteClosingTag( "math" );
}

void meLSXMLHeightTerrain::pSaveVLayerRuleMultiMath( decXmlWriter &writer, const meHTVRuleMultiMath &rule ){
	int oper = rule.GetOperator();
	
	writer.WriteOpeningTag( "multimath" );
	
	pSaveVLayerRule( writer, rule );
	
	if( oper == meHTVRuleMultiMath::eopAdd ){
		writer.WriteDataTagString( "operator", "add" );
		
	}else if( oper == meHTVRuleMultiMath::eopMultiply ){
		writer.WriteDataTagString( "operator", "multiply" );
		
	}else if( oper == meHTVRuleMultiMath::eopMinimum ){
		writer.WriteDataTagString( "operator", "minimum" );
		
	}else if( oper == meHTVRuleMultiMath::eopMaximum ){
		writer.WriteDataTagString( "operator", "maximum" );
		
	}else{ // oper == meHTVRuleMultiMath::eopAverage
		writer.WriteDataTagString( "operator", "average" );
	}
	
	writer.WriteClosingTag( "multimath" );
}

void meLSXMLHeightTerrain::pSaveVLayerRuleRandom( decXmlWriter &writer, const meHTVRuleRandom &rule ){
	writer.WriteOpeningTag( "random" );
	
	pSaveVLayerRule( writer, rule );
	
	writer.WriteClosingTag( "random" );
}

void meLSXMLHeightTerrain::pSaveVLayerRuleResult( decXmlWriter &writer, const meHTVRuleResult &rule ){
	writer.WriteOpeningTag( "result" );
	
	pSaveVLayerRule( writer, rule );
	
	writer.WriteDataTagFloat( "probability", rule.GetProbability() );
	writer.WriteDataTagInt( "variation", rule.GetVariation() );
	
	writer.WriteClosingTag( "result" );
}

void meLSXMLHeightTerrain::pSaveVLayerRuleVectorMath( decXmlWriter &writer, const meHTVRuleVectorMath &rule ){
	int oper = rule.GetOperator();
	
	writer.WriteOpeningTag( "vectormath" );
	
	pSaveVLayerRule( writer, rule );
	
	if( oper == meHTVRuleVectorMath::eopAdd ){
		writer.WriteDataTagString( "operator", "add" );
		
	}else if( oper == meHTVRuleVectorMath::eopSubtract ){
		writer.WriteDataTagString( "operator", "subtract" );
		
	}else if( oper == meHTVRuleVectorMath::eopAverage ){
		writer.WriteDataTagString( "operator", "average" );
		
	}else if( oper == meHTVRuleVectorMath::eopNormalize ){
		writer.WriteDataTagString( "operator", "normalize" );
		
	}else if( oper == meHTVRuleVectorMath::eopDot ){
		writer.WriteDataTagString( "operator", "dot" );
		
	}else{ // oper == meHTVRuleVectorMath::eopCross
		writer.WriteDataTagString( "operator", "cross" );
	}
	
	const decVector &vectorA = rule.GetVectorA();
	writer.WriteOpeningTagStart( "vector1" );
	writer.WriteAttributeFloat( "x", vectorA.x );
	writer.WriteAttributeFloat( "y", vectorA.y );
	writer.WriteAttributeFloat( "z", vectorA.z );
	writer.WriteOpeningTagEnd( true );
	
	const decVector &vectorB = rule.GetVectorB();
	writer.WriteOpeningTagStart( "vector2" );
	writer.WriteAttributeFloat( "x", vectorB.x );
	writer.WriteAttributeFloat( "y", vectorB.y );
	writer.WriteAttributeFloat( "z", vectorB.z );
	writer.WriteOpeningTagEnd( true );
	
	writer.WriteClosingTag( "vectormath" );
}

void meLSXMLHeightTerrain::pSaveVLayerRuleConstant( decXmlWriter &writer, const meHTVRuleConstant &rule ){
	writer.WriteOpeningTag( "constant" );
	
	pSaveVLayerRule( writer, rule );
	
	const decVector &vector = rule.GetVector();
	writer.WriteOpeningTagStart( "vector" );
	writer.WriteAttributeFloat( "x", vector.x );
	writer.WriteAttributeFloat( "y", vector.y );
	writer.WriteAttributeFloat( "z", vector.z );
	writer.WriteOpeningTagEnd( true );
	
	writer.WriteClosingTag( "constant" );
}

void meLSXMLHeightTerrain::pSaveVLayerRulePropCount( decXmlWriter &writer, const meHTVRulePropCount &rule ){
	writer.WriteOpeningTag( "propCount" );
	
	pSaveVLayerRule( writer, rule );
	
	if( ! rule.GetPropClass().IsEmpty() ){
		writer.WriteDataTagString( "propClass", rule.GetPropClass() );
	}
	
	writer.WriteDataTagFloat( "searchRadius", rule.GetSearchRadius() );
	
	writer.WriteClosingTag( "propCount" );
}

void meLSXMLHeightTerrain::pSaveVLayerLink( decXmlWriter &writer, const meHTVegetationLayer &vlayer, const meHTVRLink &link ){
	meHTVRule * const ruleDestination = link.GetDestinationRule();
	meHTVRule * const ruleSource = link.GetSourceRule();
	
	writer.WriteOpeningTagStart( "link" );
	writer.WriteAttributeInt( "source", vlayer.IndexOfRule( ruleSource ) );
	writer.WriteAttributeString( "output", pSlotOutputNameForNumber( ruleSource->GetType(), link.GetSourceSlot() ) );
	writer.WriteAttributeInt( "destination", vlayer.IndexOfRule( ruleDestination ) );
	writer.WriteAttributeString( "input", pSlotInputNameForNumber( ruleDestination->GetType(), link.GetDestinationSlot() ) );
	writer.WriteOpeningTagEnd( true );
}



void meLSXMLHeightTerrain::pLoadHeightTerrain( decXmlElementTag &root, meHeightTerrain &heightTerrain ){
	heightTerrain.RemoveAllSectors();
	heightTerrain.RemoveAllVLayers();
	heightTerrain.SetSectorSize( GetAttributeFloat( root, "sectorSize" ) );
	heightTerrain.SetSectorResolution( GetAttributeInt( root, "sectorResolution" ) );
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "baseHeight" ){
			heightTerrain.SetBaseHeight( GetCDataFloat( *tag ) );
			
		}else if( tagName == "heightScaling" ){
			heightTerrain.SetHeightScaling( GetCDataFloat( *tag ) );
			
		}else if( tagName == "sector" ){
			const meHeightTerrainSector::Ref htsector(meHeightTerrainSector::Ref::NewWith(
				heightTerrain.GetWorld().GetEngine(), decPoint(
					GetAttributeInt(*tag, "x"), GetAttributeInt(*tag, "z"))));
			heightTerrain.AddSector(htsector);
			pLoadSector(*tag, htsector);
			
		}else if( tagName == "vegetationLayer" ){
			pLoadVLayer( *tag, heightTerrain );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	// reset changed flags
	const int sectorCount = heightTerrain.GetSectorCount();
	for( i=0; i<sectorCount; i++ ){
		meHeightTerrainSector &sector = *heightTerrain.GetSectorAt( i );
		sector.ValidateAllPropFields();
		sector.SetPFCacheChanged( false );
	}
	
	heightTerrain.SetDepChanged( false );
	heightTerrain.SetSaved( true );
	heightTerrain.SetChanged( false );
	
	// load prop field caches if present
	const decString baseDir( heightTerrain.GetBaseDirectory() );
	
	for( i=0; i<sectorCount; i++ ){
		meHeightTerrainSector &sector = *heightTerrain.GetSectorAt( i );
		if( sector.GetPathPFCacheImage().IsEmpty() ){
			continue;
		}
		
		const decString path( decPath::AbsolutePathUnix(
			sector.GetPathPFCacheImage(), baseDir ).GetPathUnix() );
		try{
			pLSSys->LoadPFCache( sector, path );
			
		}catch( const deException &e ){
			sector.InvalidateAllPropFields();
			GetLogger()->LogErrorFormat( GetLoggerSource(),
				"Failed loading prop field cache from '%s',", path.GetString() );
			GetLogger()->LogException( GetLoggerSource(), e );
		}
	}
}

void meLSXMLHeightTerrain::pLoadSector( decXmlElementTag &root, meHeightTerrainSector &sector ){
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "heightImage" ){
			sector.SetPathHeightImage( GetCDataString( *tag ), true );
			sector.SetHeightImageChanged( false );
			sector.SetHeightImageSaved( true );
			
		}else if( tagName == "texture" ){
			pLoadTexture( *tag, sector );
			
		}else if( tagName == "navigationSpace" ){
			pLoadNavSpace( *tag, sector );
			
		}else if( tagName == "visibilityImage" ){
			sector.SetPathVisibilityImage( GetCDataString( *tag ) );
			sector.LoadVisibilityFromImage();
			if( sector.GetVisibilitySaved() ){
				sector.SetVisibilityChanged( false );
			}
			
		}else if( tagName == "pfcache" ){
			sector.SetPathPFCache( GetCDataString( *tag ) );
			sector.SetPFCacheChanged( false );
			sector.SetPFCacheSaved( true );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void meLSXMLHeightTerrain::pLoadTexture( decXmlElementTag &root, meHeightTerrainSector &sector ){
	const meHeightTerrainTexture::Ref texture(meHeightTerrainTexture::Ref::NewWith(
		pLSSys->GetWindowMain()->GetEngine(), GetAttributeString(root, "name")));
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "skin" ){
			texture->SetPathSkin( GetCDataString( *tag ) );
			
		}else if( tagName == "typeNumber" ){
			texture->SetTypeNumber( GetCDataInt( *tag ) );
			
		}else if( tagName == "uvOffset" ){
			texture->SetProjectionOffsetU( GetAttributeFloat( *tag, "u" ) );
			texture->SetProjectionOffsetV( GetAttributeFloat( *tag, "v" ) );
			
		}else if( tagName == "uvScaling" ){
			texture->SetProjectionScalingU( GetAttributeFloat( *tag, "u" ) );
			texture->SetProjectionScalingV( GetAttributeFloat( *tag, "v" ) );
			
		}else if( tagName == "uvRotation" ){
			texture->SetProjectionRotation( GetCDataFloat( *tag ) );
			
		}else if( tagName == "mask" ){
			texture->SetPathMask( GetCDataString( *tag ), true );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	sector.AddTexture(texture);
}

void meLSXMLHeightTerrain::pLoadNavSpace( decXmlElementTag &root, meHeightTerrainSector &sector ){
	meHeightTerrainNavSpace::Ref navSpace(meHeightTerrainNavSpace::Ref::NewWith(*pLSSys->GetWindowMain()->GetEngine()));
	
	navspace.SetName( GetAttributeString( root, "name" ) );
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "layer" ){
			navspace.SetLayer( GetCDataInt( *tag ) );
			
		}else if( tagName == "snapDistance" ){
			navspace.SetSnapDistance( GetCDataFloat( *tag ) );
			
		}else if( tagName == "snapAngle" ){
			navspace.SetSnapAngle( GetCDataFloat( *tag ) );
			
		}else if( tagName == "type" ){
			pLoadNavSpaceType( *tag, navspace );
			
		}else if( tagName == "path" ){
			navspace.SetPathNavSpace( GetCDataString( *tag ), false ); // we can not load here yet
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	sector.AddNavSpace( &navspace );
	
	navspace.LoadNavSpaceFromFile(); // because we can only load properly after being added
}

void meLSXMLHeightTerrain::pLoadNavSpaceType( decXmlElementTag &root, meHeightTerrainNavSpace &navspace ){
	const meHeightTerrainNavSpaceType::Ref navtype(meHeightTerrainNavSpaceType::Ref::NewWith());
	
	navtype->SetName( GetAttributeString( root, "name" ) );
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "type" ){
			navtype->SetType( GetCDataInt( *tag ) );
			
		}else if( tagName == "color" ){
			navtype->SetColor( decColor( GetAttributeFloat( *tag, "r" ),
				GetAttributeFloat( *tag, "g" ), GetAttributeFloat( *tag, "b" ) ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	navspace.AddType(navtype);
}

void meLSXMLHeightTerrain::pLoadVLayer( decXmlElementTag &root, meHeightTerrain &heightTerrain ){
	meHTVegetationLayer::Ref vLayer(meHTVegetationLayer::Ref::NewWith(pLSSys->GetWindowMain()->GetEngine()));
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "name" ){
			vlayer.SetName( GetCDataString( *tag ) );
			
		}else if( tagName == "viewCenter" ){
			decVector2 vector;
			ReadVector2( *tag, vector );
			vlayer.SetViewCenter( vector );
			
		}else if( tagName == "variation" ){
			pLoadVLayerVariation( *tag, vlayer );
			
		}else if( tagName == "rules" ){
			pLoadVLayerRules( *tag, vlayer );
			
		}else if( tagName == "link" ){
			pLoadVLayerLink( *tag, vlayer );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	heightTerrain.AddVLayer( &vlayer );
}

void meLSXMLHeightTerrain::pLoadVLayerVariation( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	meHTVVariation::Ref variation(meHTVVariation::Ref::NewWith(pLSSys->GetWindowMain()->GetEngine()));
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "model" ){
			variation.SetPathModel( GetCDataString( *tag ) );
			
		}else if( tagName == "skin" ){
			variation.SetPathSkin( GetCDataString( *tag ) );
			
		}else if( tagName == "rotationPerForce" ){
			variation.SetRotationPerForce( GetCDataFloat( *tag ) );
			
		}else if( tagName == "restitution" ){
			variation.SetRestitution( GetCDataFloat( *tag ) );
			
		}else if( tagName == "randomRotation" ){
			variation.SetMinimumRandomRotation( GetAttributeFloat( *tag, "min" ) );
			variation.SetMaximumRandomRotation( GetAttributeFloat( *tag, "max" ) );
			
		}else if( tagName == "randomScaling" ){
			variation.SetMinimumRandomScaling( GetAttributeFloat( *tag, "min" ) );
			variation.SetMaximumRandomScaling( GetAttributeFloat( *tag, "max" ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	vlayer.AddVariation( &variation );
}

void meLSXMLHeightTerrain::pLoadVLayerRules( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "closestProp" ){
			pLoadVLayerRuleClosestProp( *tag, vlayer );
			
		}else if( tagName == "closestVegetation" ){
			pLoadVLayerRuleClosestVegetation( *tag, vlayer );
			
		}else if( tagName == "combine" ){
			pLoadVLayerRuleCombine( *tag, vlayer );
			
		}else if( tagName == "components" ){
			pLoadVLayerRuleComponents( *tag, vlayer );
			
		}else if( tagName == "curve" ){
			pLoadVLayerRuleCurve( *tag, vlayer );
			
		}else if( tagName == "geometry" ){
			pLoadVLayerRuleGeometry( *tag, vlayer );
			
		}else if( tagName == "mapping" ){
			pLoadVLayerRuleMapping( *tag, vlayer );
			
		}else if( tagName == "math" ){
			pLoadVLayerRuleMath( *tag, vlayer );
			
		}else if( tagName == "multimath" ){
			pLoadVLayerRuleMultiMath( *tag, vlayer );
			
		}else if( tagName == "random" ){
			pLoadVLayerRuleRandom( *tag, vlayer );
			
		}else if( tagName == "result" ){
			pLoadVLayerRuleResult( *tag, vlayer );
			
		}else if( tagName == "vectormath" ){
			pLoadVLayerRuleVectorMath( *tag, vlayer );
			
		}else if( tagName == "constant" ){
			pLoadVLayerRuleConstant( *tag, vlayer );
			
		}else if( tagName == "propCount" ){
			pLoadVLayerRulePropCount( *tag, vlayer );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

bool meLSXMLHeightTerrain::pLoadVLayerRule( decXmlElementTag &tag, meHTVRule &rule ){
	const decString &tagName = tag.GetName();
	if( tagName == "name" ){
		rule.SetName( GetCDataString( tag ) );
		return true;
		
	}else if( tagName == "position" ){
		decVector2 vector;
		ReadVector2( tag, vector );
		rule.SetPosition( vector );
		return true;
		
	}else if( tagName == "showParameters" ){
		rule.SetShowParameters( GetCDataInt( tag ) != 0 );
		return true;
	}
	
	return false;
}

void meLSXMLHeightTerrain::pLoadVLayerRuleClosestProp( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	const meHTVRuleClosestProp::Ref rule(meHTVRuleClosestProp::Ref::NewWith());
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "propClass" ){
			rule->SetPropClass( GetCDataString( *tag ) );
			
		}else if( tagName == "searchRadius" ){
			rule->SetSearchRadius( GetCDataFloat( *tag ) );
			
		}else if( ! pLoadVLayerRule( *tag, rule ) ){
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	vlayer.AddRule(rule);
}

void meLSXMLHeightTerrain::pLoadVLayerRuleClosestVegetation( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	const meHTVRuleClosestVegetation::Ref rule(meHTVRuleClosestVegetation::Ref::NewWith());
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "vegetationType" ){
			rule->SetVegetationType( GetCDataString( *tag ) );
			
		}else if( tagName == "searchRadius" ){
			rule->SetSearchRadius( GetCDataFloat( *tag ) );
			
		}else if( ! pLoadVLayerRule( *tag, rule ) ){
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	vlayer.AddRule(rule);
}

void meLSXMLHeightTerrain::pLoadVLayerRuleCombine( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	const meHTVRuleCombine::Ref rule(meHTVRuleCombine::Ref::NewWith());
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "x" ){
			rule->SetX( GetCDataFloat( *tag ) );
			
		}else if( tagName == "y" ){
			rule->SetY( GetCDataFloat( *tag ) );
			
		}else if( tagName == "z" ){
			rule->SetZ( GetCDataFloat( *tag ) );
			
		}else if( ! pLoadVLayerRule( *tag, rule ) ){
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	vlayer.AddRule(rule);
}

void meLSXMLHeightTerrain::pLoadVLayerRuleComponents( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	const meHTVRuleComponents::Ref rule(meHTVRuleComponents::Ref::NewWith());
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "vector" ){
			decVector vector;
			ReadVector( *tag, vector );
			rule->SetVector( vector );
			
		}else if( ! pLoadVLayerRule( *tag, rule ) ){
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	vlayer.AddRule(rule);
}

void meLSXMLHeightTerrain::pLoadVLayerRuleCurve( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	decCurveBezier curve;
	ReadCurveBezier( root, curve );
	
	const meHTVRuleCurve::Ref rule(meHTVRuleCurve::Ref::NewWith());
	rule->SetCurve(curve);
	vlayer.AddRule(rule);
}

void meLSXMLHeightTerrain::pLoadVLayerRuleGeometry( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	const meHTVRuleGeometry::Ref rule(meHTVRuleGeometry::Ref::NewWith());
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( ! pLoadVLayerRule( *tag, rule ) ){
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	vlayer.AddRule(rule);
}

void meLSXMLHeightTerrain::pLoadVLayerRuleMapping( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	const meHTVRuleMapping::Ref rule(meHTVRuleMapping::Ref::NewWith());
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "lower" ){
			rule->SetLower( GetCDataFloat( *tag ) );
			
		}else if( tagName == "upper" ){
			rule->SetUpper( GetCDataFloat( *tag ) );
			
		}else if( tagName == "value" ){
			rule->SetValue( GetCDataFloat( *tag ) );
			
		}else if( tagName == "inversed" ){
			rule->SetInversed( GetCDataInt( *tag ) != 0 );
			
		}else if( ! pLoadVLayerRule( *tag, rule ) ){
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	vlayer.AddRule(rule);
}

void meLSXMLHeightTerrain::pLoadVLayerRuleMath( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	const meHTVRuleMath::Ref rule(meHTVRuleMath::Ref::NewWith());
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "operator" ){
			const char * const keyword = GetCDataString( *tag );
			
			if( strcmp( keyword, "add" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopAdd );
				
			}else if( strcmp( keyword, "subtract" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopSubtract );
				
			}else if( strcmp( keyword, "multiply" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopMultiply );
				
			}else if( strcmp( keyword, "divide" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopDivide );
				
			}else if( strcmp( keyword, "sine" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopSine );
				
			}else if( strcmp( keyword, "cosine" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopCosine );
				
			}else if( strcmp( keyword, "tangent" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopTangent );
				
			}else if( strcmp( keyword, "arcsine" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopArcSine );
				
			}else if( strcmp( keyword, "arccosine" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopArcCosine );
				
			}else if( strcmp( keyword, "arctangent" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopArcTangent );
				
			}else if( strcmp( keyword, "power" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopPower );
				
			}else if( strcmp( keyword, "exponential" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopExponential );
				
			}else if( strcmp( keyword, "logarithm" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopLogarithm );
				
			}else if( strcmp( keyword, "minimum" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopMinimum );
				
			}else if( strcmp( keyword, "maximum" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopMaximum );
				
			}else if( strcmp( keyword, "round" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopRound );
				
			}else if( strcmp( keyword, "lessthan" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopLessThan );
				
			}else if( strcmp( keyword, "greaterthan" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopGreaterThan );
				
			}else if( strcmp( keyword, "equal" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopEqual );
				
			}else if( strcmp( keyword, "notequal" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopNotEqual );
				
			}else if( strcmp( keyword, "average" ) == 0 ){
				rule->SetOperator( meHTVRuleMath::eopAverage );
				
			}else{
				LogWarnUnknownValue( *tag, keyword );
			}
			
		}else if( tagName == "value1" ){
			rule->SetValueA( GetCDataFloat( *tag ) );
			
		}else if( tagName == "value2" ){
			rule->SetValueB( GetCDataFloat( *tag ) );
			
		}else if( ! pLoadVLayerRule( *tag, rule ) ){
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	vlayer.AddRule(rule);
}

void meLSXMLHeightTerrain::pLoadVLayerRuleMultiMath( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	const meHTVRuleMultiMath::Ref rule(meHTVRuleMultiMath::Ref::NewWith());
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "operator" ){
			const char * const keyword = GetCDataString( *tag );
			
			if( strcmp( keyword, "add" ) == 0 ){
				rule->SetOperator( meHTVRuleMultiMath::eopAdd );
				
			}else if( strcmp( keyword, "multiply" ) == 0 ){
				rule->SetOperator( meHTVRuleMultiMath::eopMultiply );
				
			}else if( strcmp( keyword, "minimum" ) == 0 ){
				rule->SetOperator( meHTVRuleMultiMath::eopMinimum );
				
			}else if( strcmp( keyword, "maximum" ) == 0 ){
				rule->SetOperator( meHTVRuleMultiMath::eopMaximum );
				
			}else if( strcmp( keyword, "average" ) == 0 ){
				rule->SetOperator( meHTVRuleMultiMath::eopAverage );
				
			}else{
				LogWarnUnknownValue( *tag, keyword );
			}
			
		}else if( ! pLoadVLayerRule( *tag, rule ) ){
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	vlayer.AddRule(rule);
}

void meLSXMLHeightTerrain::pLoadVLayerRuleRandom( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	const meHTVRuleRandom::Ref rule(meHTVRuleRandom::Ref::NewWith());
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( ! pLoadVLayerRule( *tag, rule ) ){
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	vlayer.AddRule(rule);
}

void meLSXMLHeightTerrain::pLoadVLayerRuleResult( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	const meHTVRuleResult::Ref rule(meHTVRuleResult::Ref::NewWith());
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "probability" ){
			rule->SetProbability( GetCDataFloat( *tag ) );
			
		}else if( tagName == "variation" ){
			rule->SetVariation( GetCDataInt( *tag ) );
			
		}else if( ! pLoadVLayerRule( *tag, rule ) ){
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	vlayer.AddRule(rule);
}

void meLSXMLHeightTerrain::pLoadVLayerRuleVectorMath( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	const meHTVRuleVectorMath::Ref rule(meHTVRuleVectorMath::Ref::NewWith());
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "operator" ){
			const char * const keyword = GetCDataString( *tag );
			
			if( strcmp( keyword, "add" ) == 0 ){
				rule->SetOperator( meHTVRuleVectorMath::eopAdd );
				
			}else if( strcmp( keyword, "subtract" ) == 0 ){
				rule->SetOperator( meHTVRuleVectorMath::eopSubtract );
				
			}else if( strcmp( keyword, "average" ) == 0 ){
				rule->SetOperator( meHTVRuleVectorMath::eopAverage );
				
			}else if( strcmp( keyword, "normalize" ) == 0 ){
				rule->SetOperator( meHTVRuleVectorMath::eopNormalize );
				
			}else if( strcmp( keyword, "dot" ) == 0 ){
				rule->SetOperator( meHTVRuleVectorMath::eopDot );
				
			}else if( strcmp( keyword, "cross" ) == 0 ){
				rule->SetOperator( meHTVRuleVectorMath::eopCross );
				
			}else{
				LogWarnUnknownValue( *tag, keyword );
			}
			
		}else if( tagName == "vector1" ){
			decVector vector;
			ReadVector( *tag, vector );
			rule->SetVectorA( vector );
			
		}else if( tagName == "vector2" ){
			decVector vector;
			ReadVector( *tag, vector );
			rule->SetVectorB( vector );
			
		}else if( ! pLoadVLayerRule( *tag, rule ) ){
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	vlayer.AddRule(rule);
}

void meLSXMLHeightTerrain::pLoadVLayerRuleConstant( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	const meHTVRuleConstant::Ref rule(meHTVRuleConstant::Ref::NewWith());
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "vector" ){
			decVector vector;
			ReadVector( *tag, vector );
			rule->SetVector( vector );
			
		}else if( ! pLoadVLayerRule( *tag, rule ) ){
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	vlayer.AddRule(rule);
}

void meLSXMLHeightTerrain::pLoadVLayerRulePropCount( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	const meHTVRulePropCount::Ref rule(meHTVRulePropCount::Ref::NewWith());
	
	const int count = root.GetElementCount();
	int i;
	for( i=0; i<count; i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "propClass" ){
			rule->SetPropClass( GetCDataString( *tag ) );
			
		}else if( tagName == "searchRadius" ){
			rule->SetSearchRadius( GetCDataFloat( *tag ) );
			
		}else if( ! pLoadVLayerRule( *tag, rule ) ){
			LogWarnUnknownTag( root, *tag );
		}
	}
	
	vlayer.AddRule(rule);
}

void meLSXMLHeightTerrain::pLoadVLayerLink( decXmlElementTag &root, meHTVegetationLayer &vlayer ){
	meHTVRule *destinationRule;
	meHTVRule *sourceRule;
	int destinationSlot;
	const char *keyword;
	int sourceSlot;
	int index;
	
	index = GetAttributeInt( root, "source" );
	if( index < 0 || index >= vlayer.GetRuleCount() ){
		pLSSys->GetWindowMain()->GetLogger()->LogErrorFormat( LOGSOURCE,
			"%s(%i:%i): Source index %i out of bounds", root.GetName().GetString(),
			root.GetLineNumber(), root.GetPositionNumber(), index );
		DETHROW( deeInvalidParam );
	}
	sourceRule = vlayer.GetRuleAt( index );
	
	keyword = GetAttributeString( root, "output" );
	sourceSlot = pSlotOutputNumberForName( sourceRule->GetType(), keyword );
	if( sourceSlot == -1 ){
		pLSSys->GetWindowMain()->GetLogger()->LogWarnFormat( LOGSOURCE,
			"%s(%i:%i): Unknown output slot '%s'", root.GetName().GetString(),
			root.GetLineNumber(), root.GetPositionNumber(), keyword );
		DETHROW( deeInvalidParam );
	}
	
	index = GetAttributeInt( root, "destination" );
	if( index < 0 || index >= vlayer.GetRuleCount() ){
		pLSSys->GetWindowMain()->GetLogger()->LogWarnFormat( LOGSOURCE,
			"%s(%i:%i): Destination index %i out of bounds", root.GetName().GetString(),
				root.GetLineNumber(), root.GetPositionNumber(), index );
		DETHROW( deeInvalidParam );
	}
	destinationRule = vlayer.GetRuleAt( index );
	
	keyword = GetAttributeString( root, "input" );
	destinationSlot = pSlotInputNumberForName( destinationRule->GetType(), keyword );
	if( destinationSlot == -1 ){
		pLSSys->GetWindowMain()->GetLogger()->LogErrorFormat( LOGSOURCE,
			"%s(%i:%i): Unknown input slot '%s'", root.GetName().GetString(),
			root.GetLineNumber(), root.GetPositionNumber(), keyword );
		DETHROW( deeInvalidParam );
	}
	
	const meHTVRLink::Ref refLink(meHTVRLink::Ref::NewWith(sourceRule, sourceSlot, destinationRule, destinationSlot));
	meHTVRLink * const link = refLink;
	
	sourceRule->GetSlotAt( sourceSlot ).AddLink( link );
	destinationRule->GetSlotAt( destinationSlot ).AddLink( link );
	
	vlayer.AddLink( link );
}



const char *meLSXMLHeightTerrain::pSlotInputNameForNumber( int ruleType, int slot ) const{
	if( ruleType == meHTVRule::ertCombine ){
		if( slot == meHTVRuleCombine::eisX ) return "x";
		if( slot == meHTVRuleCombine::eisY ) return "y";
		if( slot == meHTVRuleCombine::eisZ ) return "z";
		
	}else if( ruleType == meHTVRule::ertComponents ){
		if( slot == meHTVRuleComponents::eisVector ) return "vector";
		
	}else if( ruleType == meHTVRule::ertCurve ){
		if( slot == meHTVRuleCurve::eisValue ) return "value";
		
	}else if( ruleType == meHTVRule::ertMapping ){
		if( slot == meHTVRuleMapping::eisLower ) return "lower";
		if( slot == meHTVRuleMapping::eisUpper ) return "upper";
		if( slot == meHTVRuleMapping::eisValue ) return "value";
		
	}else if( ruleType == meHTVRule::ertMath ){
		if( slot == meHTVRuleMath::eisValueA ) return "value1";
		if( slot == meHTVRuleMath::eisValueB ) return "value2";
		
	}else if( ruleType == meHTVRule::ertMultiMath ){
		if( slot == meHTVRuleMultiMath::eisValues ) return "values";
		
	}else if( ruleType == meHTVRule::ertResult ){
		if( slot == meHTVRuleResult::eisProbability ) return "probability";
		if( slot == meHTVRuleResult::eisVariation ) return "variation";
		
	}else if( ruleType == meHTVRule::ertVectorMath ){
		if( slot == meHTVRuleVectorMath::eisVectorA ) return "vector1";
		if( slot == meHTVRuleVectorMath::eisVectorB ) return "vector2";
	}
	
	return NULL;
}

const char *meLSXMLHeightTerrain::pSlotOutputNameForNumber( int ruleType, int slot ) const{
	if( ruleType == meHTVRule::ertClosestProp ){
		if( slot == meHTVRuleClosestProp::eosDirection ) return "direction";
		if( slot == meHTVRuleClosestProp::eosDistance ) return "distance";
		
	}else if( ruleType == meHTVRule::ertClosestVegetation ){
		if( slot == meHTVRuleClosestVegetation::eosDirection ) return "direction";
		if( slot == meHTVRuleClosestVegetation::eosDistance ) return "distance";
		
	}else if( ruleType == meHTVRule::ertCombine ){
		if( slot == meHTVRuleCombine::eosVector ) return "vector";
		
	}else if( ruleType == meHTVRule::ertComponents ){
		if( slot == meHTVRuleComponents::eosX ) return "x";
		if( slot == meHTVRuleComponents::eosY ) return "y";
		if( slot == meHTVRuleComponents::eosZ ) return "z";
		
	}else if( ruleType == meHTVRule::ertCurve ){
		if( slot == meHTVRuleCurve::eosValue ) return "value";
		
	}else if( ruleType == meHTVRule::ertGeometry ){
		if( slot == meHTVRuleGeometry::eosHeight ) return "height";
		if( slot == meHTVRuleGeometry::eosNormal ) return "normal";
		if( slot == meHTVRuleGeometry::eosTerrainType ) return "terrainType";
		
	}else if( ruleType == meHTVRule::ertMapping ){
		if( slot == meHTVRuleMapping::eosValue ) return "value";
		
	}else if( ruleType == meHTVRule::ertMath ){
		if( slot == meHTVRuleMath::eosResult ) return "result";
		
	}else if( ruleType == meHTVRule::ertMultiMath ){
		if( slot == meHTVRuleMultiMath::eosResult ) return "result";
		
	}else if( ruleType == meHTVRule::ertRandom ){
		if( slot == meHTVRuleRandom::eosRandom ) return "random";
		
	}else if( ruleType == meHTVRule::ertVectorMath ){
		if( slot == meHTVRuleVectorMath::eosValue ) return "value";
		if( slot == meHTVRuleVectorMath::eosVector ) return "vector";
		
	}else if( ruleType == meHTVRule::ertConstant ){
		if( slot == meHTVRuleConstant::eosVector ) return "vector";
		if( slot == meHTVRuleConstant::eosX ) return "x";
		if( slot == meHTVRuleConstant::eosY ) return "y";
		if( slot == meHTVRuleConstant::eosZ ) return "z";
		
	}else if( ruleType == meHTVRule::ertPropCount ){
		if( slot == meHTVRulePropCount::eosCount ) return "count";
	}
	
	return NULL;
}

int meLSXMLHeightTerrain::pSlotInputNumberForName( int ruleType, const char *name ) const{
	if( ruleType == meHTVRule::ertCombine ){
		if( strcmp( name, "x" ) == 0 ) return meHTVRuleCombine::eisX;
		if( strcmp( name, "y" ) == 0 ) return meHTVRuleCombine::eisY;
		if( strcmp( name, "z" ) == 0 ) return meHTVRuleCombine::eisZ;
		
	}else if( ruleType == meHTVRule::ertComponents ){
		if( strcmp( name, "vector" ) == 0 ) return meHTVRuleComponents::eisVector;
		
	}else if( ruleType == meHTVRule::ertCurve ){
		if( strcmp( name, "value" ) == 0 ) return meHTVRuleCurve::eisValue;
		
	}else if( ruleType == meHTVRule::ertMapping ){
		if( strcmp( name, "lower" ) == 0 ) return meHTVRuleMapping::eisLower;
		if( strcmp( name, "upper" ) == 0 ) return meHTVRuleMapping::eisUpper;
		if( strcmp( name, "value" ) == 0 ) return meHTVRuleMapping::eisValue;
		
	}else if( ruleType == meHTVRule::ertMath ){
		if( strcmp( name, "value1" ) == 0 ) return meHTVRuleMath::eisValueA;
		if( strcmp( name, "value2" ) == 0 ) return meHTVRuleMath::eisValueB;
		
	}else if( ruleType == meHTVRule::ertMultiMath ){
		if( strcmp( name, "values" ) == 0 ) return meHTVRuleMultiMath::eisValues;
		
	}else if( ruleType == meHTVRule::ertResult ){
		if( strcmp( name, "probability" ) == 0 ) return meHTVRuleResult::eisProbability;
		if( strcmp( name, "variation" ) == 0 ) return meHTVRuleResult::eisVariation;
		
	}else if( ruleType == meHTVRule::ertVectorMath ){
		if( strcmp( name, "vector1" ) == 0 ) return meHTVRuleVectorMath::eisVectorA;
		if( strcmp( name, "vector2" ) == 0 ) return meHTVRuleVectorMath::eisVectorB;
	}
	
	return -1;
}

int meLSXMLHeightTerrain::pSlotOutputNumberForName( int ruleType, const char *name ) const{
	if( ruleType == meHTVRule::ertClosestProp ){
		if( strcmp( name, "direction" ) == 0 ) return meHTVRuleClosestProp::eosDirection;
		if( strcmp( name, "distance" ) == 0 ) return meHTVRuleClosestProp::eosDistance;
		
	}else if( ruleType == meHTVRule::ertClosestVegetation ){
		if( strcmp( name, "direction" ) == 0 ) return meHTVRuleClosestVegetation::eosDirection;
		if( strcmp( name, "distance" ) == 0 ) return meHTVRuleClosestVegetation::eosDistance;
		
	}else if( ruleType == meHTVRule::ertCombine ){
		if( strcmp( name, "vector" ) == 0 ) return meHTVRuleCombine::eosVector;
		
	}else if( ruleType == meHTVRule::ertComponents ){
		if( strcmp( name, "x" ) == 0 ) return meHTVRuleComponents::eosX;
		if( strcmp( name, "y" ) == 0 ) return meHTVRuleComponents::eosY;
		if( strcmp( name, "z" ) == 0 ) return meHTVRuleComponents::eosZ;
		
	}else if( ruleType == meHTVRule::ertCurve ){
		if( strcmp( name, "value" ) == 0 ) return meHTVRuleCurve::eosValue;
		
	}else if( ruleType == meHTVRule::ertGeometry ){
		if( strcmp( name, "height" ) == 0 ) return meHTVRuleGeometry::eosHeight;
		if( strcmp( name, "normal" ) == 0 ) return meHTVRuleGeometry::eosNormal;
		if( strcmp( name, "terrainType" ) == 0 ) return meHTVRuleGeometry::eosTerrainType;
		
	}else if( ruleType == meHTVRule::ertMapping ){
		if( strcmp( name, "value" ) == 0 ) return meHTVRuleMapping::eosValue;
		
	}else if( ruleType == meHTVRule::ertMath ){
		if( strcmp( name, "result" ) == 0 ) return meHTVRuleMath::eosResult;
		
	}else if( ruleType == meHTVRule::ertMultiMath ){
		if( strcmp( name, "result" ) == 0 ) return meHTVRuleMultiMath::eosResult;
		
	}else if( ruleType == meHTVRule::ertRandom ){
		if( strcmp( name, "random" ) == 0 ) return meHTVRuleRandom::eosRandom;
		
	}else if( ruleType == meHTVRule::ertVectorMath ){
		if( strcmp( name, "value" ) == 0 ) return meHTVRuleVectorMath::eosValue;
		if( strcmp( name, "vector" ) == 0 ) return meHTVRuleVectorMath::eosVector;
		
	}else if( ruleType == meHTVRule::ertConstant ){
		if( strcmp( name, "vector" ) == 0 ) return meHTVRuleConstant::eosVector;
		if( strcmp( name, "x" ) == 0 ) return meHTVRuleConstant::eosX;
		if( strcmp( name, "y" ) == 0 ) return meHTVRuleConstant::eosY;
		if( strcmp( name, "z" ) == 0 ) return meHTVRuleConstant::eosZ;
		
	}else if( ruleType == meHTVRule::ertPropCount ){
		if( strcmp( name, "count" ) == 0 ) return meHTVRulePropCount::eosCount;
	}
	
	return -1;
}
