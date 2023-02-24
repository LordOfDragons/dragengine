/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seLoadSaveSynthesizer.h"
#include "seLoadSaveSystem.h"
#include "../gui/seWindowMain.h"
#include "../synthesizer/seSynthesizer.h"
#include "../synthesizer/controller/seController.h"
#include "../synthesizer/controller/seControllerTarget.h"
#include "../synthesizer/link/seLink.h"
#include "../synthesizer/source/seSourceSound.h"
#include "../synthesizer/source/seSourceWave.h"
#include "../synthesizer/source/seSourceChain.h"
#include "../synthesizer/source/seSourceSynthesizer.h"
#include "../synthesizer/source/seSourceGroup.h"
#include "../synthesizer/effect/seEffect.h"
#include "../synthesizer/effect/seEffectStretch.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerManager.h>
#include <dragengine/resources/synthesizer/deSynthesizerController.h>
#include <dragengine/resources/synthesizer/deSynthesizerControllerTarget.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>



// Definitions
////////////////

#define LOGSOURCE "Synthesizer Editor"



// Class seLoadSaveSynthesizer
////////////////////////////////

// Constructor, destructor
////////////////////////////

seLoadSaveSynthesizer::seLoadSaveSynthesizer( seLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource ) :
igdeBaseXML( logger, loggerSource ),
pLSSys( lssys ),
pName( "Drag[en]gine XML Synthesizer" ),
pPattern( ".desynth" )
{
	if( ! lssys ){
		DETHROW( deeInvalidParam );
	}
}



// Management
///////////////

void seLoadSaveSynthesizer::LoadSynthesizer( seSynthesizer &synthesizer, decBaseFileReader &reader ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "synthesizer" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadSynthesizer( *root, synthesizer );
}

void seLoadSaveSynthesizer::SaveSynthesizer( const seSynthesizer &synthesizer, decBaseFileWriter &writer ){
	decXmlWriter xmlWriter( &writer );
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteSynthesizer( xmlWriter, synthesizer );
}



// Private functions
//////////////////////

void seLoadSaveSynthesizer::pWriteSynthesizer( decXmlWriter &writer, const seSynthesizer &synthesizer ){
	int i, count;
	
	writer.WriteOpeningTag( "synthesizer", false, true );
	
	// write controllers
	const seControllerList &controllers = synthesizer.GetControllers();
	count = controllers.GetCount();
	for( i=0; i<count; i++ ){
		if( i > 0 ){
			writer.WriteNewline();
		}
		pWriteController( writer, *controllers.GetAt( i ) );
	}
	
	// write links
	const seLinkList &links = synthesizer.GetLinks();
	count = links.GetCount();
	for( i=0; i<count; i++ ){
		writer.WriteNewline();
		pWriteLink( writer, synthesizer, *links.GetAt( i ) );
	}
	
	// write sources
	const seSourceList &sources = synthesizer.GetSources();
	count = sources.GetCount();
	for( i=0; i<count; i++ ){
		writer.WriteNewline();
		pWriteSource( writer, synthesizer, *sources.GetAt( i ) );
	}
	
	writer.WriteDataTagInt( "channels", synthesizer.GetChannelCount() );
	writer.WriteDataTagInt( "sampleRate", synthesizer.GetSampleRate() );
	writer.WriteDataTagInt( "bytesPerSample", synthesizer.GetBytesPerSample() );
	writer.WriteDataTagInt( "sampleCount", synthesizer.GetSampleCount() );
	
	writer.WriteClosingTag( "synthesizer", true );
}

void seLoadSaveSynthesizer::pWriteController( decXmlWriter &writer,
const seController &controller ){
	writer.WriteOpeningTag( "controller" );
	
	writer.WriteDataTagString( "name", controller.GetName() );
	
	writer.WriteOpeningTagStart( "limits" );
	writer.WriteAttributeFloat( "min", controller.GetMinimumValue() );
	writer.WriteAttributeFloat( "max", controller.GetMaximumValue() );
	writer.WriteOpeningTagEnd( true );
	
	if( ! controller.GetClamp() ){
		writer.WriteDataTagBool( "clamp", false );
	}
	
	writer.WriteClosingTag( "controller" );
}

void seLoadSaveSynthesizer::pWriteLink( decXmlWriter &writer,
const seSynthesizer &synthesizer, const seLink &link ){
	int controllerIndex = -1;
	
	if( link.GetController() ){
		controllerIndex = synthesizer.GetControllers().IndexOf( link.GetController() );
	}
	
	writer.WriteOpeningTag( "link" );
	
	writer.WriteDataTagString( "name", link.GetName() );
	writer.WriteDataTagInt( "controller", controllerIndex );
	if( link.GetRepeat() != 1 ){
		writer.WriteDataTagInt( "repeat", link.GetRepeat() );
	}
	
	WriteCurveBezier( writer, "curve", link.GetCurve() );
	
	writer.WriteClosingTag( "link" );
}

void seLoadSaveSynthesizer::pWriteSourceCommon( decXmlWriter &writer,
const seSynthesizer &synthesizer, const seSource &source ){
	writer.WriteDataTagString( "name", source.GetName() );
	writer.WriteDataTagBool( "enabled", source.GetEnabled() );
	
	switch( source.GetMixMode() ){
	case deSynthesizerSource::emmBlend:
		writer.WriteDataTagString( "mixMode", "blend" );
		break;
		
	case deSynthesizerSource::emmAdd:
		writer.WriteDataTagString( "mixMode", "add" );
		break;
	}
	
	if( fabsf( source.GetBlendFactor() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "blendFactor", source.GetBlendFactor() );
	}
	if( fabsf( source.GetMinVolume() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "minVolume", source.GetMinVolume() );
	}
	if( fabsf( source.GetMaxVolume() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "maxVolume", source.GetMaxVolume() );
	}
	if( fabsf( source.GetMinPanning() - 0.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "minPanning", source.GetMinPanning() );
	}
	if( fabsf( source.GetMaxPanning() - 0.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "maxPanning", source.GetMaxPanning() );
	}
	
	pWriteControllerTarget( writer, synthesizer, source.GetTargetBlendFactor(), "blendFactor" );
	pWriteControllerTarget( writer, synthesizer, source.GetTargetVolume(), "volume" );
	pWriteControllerTarget( writer, synthesizer, source.GetTargetPanning(), "panning" );
	
	const seEffectList &effects = source.GetEffects();
	const int effectCount = effects.GetCount();
	int i;
	
	for( i=0; i<effectCount; i++ ){
		pWriteEffect( writer, synthesizer, *effects.GetAt( i ) );
	}
}

void seLoadSaveSynthesizer::pWriteControllerTarget( decXmlWriter &writer,
const seSynthesizer &synthesizer, const seControllerTarget &target, const char *name ){
	const seLinkList &links = synthesizer.GetLinks();
	const int linkCount = target.GetLinkCount();
	int i;
	
	if( linkCount == 0 ){
		return;
	}
	
	writer.WriteOpeningTagStart( "target" );
	writer.WriteAttributeString( "name", name );
	writer.WriteOpeningTagEnd();
	
	for( i=0; i<linkCount; i++ ){
		writer.WriteDataTagInt( "link", links.IndexOf( target.GetLinkAt( i ) ) );
	}
	
	writer.WriteClosingTag( "target" );
}

void seLoadSaveSynthesizer::pWriteSource( decXmlWriter &writer,
const seSynthesizer &synthesizer, const seSource &source ){
	switch( source.GetType() ){
	case deSynthesizerSourceVisitorIdentify::estSound:
		pWriteSourceSound( writer, synthesizer, ( seSourceSound& )source );
		break;
		
	case deSynthesizerSourceVisitorIdentify::estWave:
		pWriteSourceWave( writer, synthesizer, ( seSourceWave& )source );
		break;
		
	case deSynthesizerSourceVisitorIdentify::estChain:
		pWriteSourceChain( writer, synthesizer, ( seSourceChain& )source );
		break;
		
	case deSynthesizerSourceVisitorIdentify::estGroup:
		pWriteSourceGroup( writer, synthesizer, ( seSourceGroup& )source );
		break;
		
	case deSynthesizerSourceVisitorIdentify::estSynthesizer:
		pWriteSourceSynthesizer( writer, synthesizer, ( seSourceSynthesizer& )source );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void seLoadSaveSynthesizer::pWriteSourceSound( decXmlWriter &writer,
const seSynthesizer &synthesizer, const seSourceSound &source ){
	writer.WriteOpeningTag( "sourceSound" );
	
	pWriteSourceCommon( writer, synthesizer, source );
	
	writer.WriteDataTagString( "sound", source.GetPathSound() );
	if( fabsf( source.GetMinSpeed() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "minSpeed", source.GetMinSpeed() );
	}
	if( fabsf( source.GetMaxSpeed() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "maxSpeed", source.GetMaxSpeed() );
	}
	if( source.GetLooping() ){
		writer.WriteDataTagBool( "looping", true );
	}
	
	pWriteControllerTarget( writer, synthesizer, source.GetTargetSpeed(), "speed" );
	pWriteControllerTarget( writer, synthesizer, source.GetTargetPlay(), "play" );
	
	writer.WriteClosingTag( "sourceSound" );
}

void seLoadSaveSynthesizer::pWriteSourceWave( decXmlWriter &writer,
const seSynthesizer &synthesizer, const seSourceWave &source ){
	writer.WriteOpeningTag( "sourceWave" );
	
	pWriteSourceCommon( writer, synthesizer, source );
	
	switch( source.GetWaveType() ){
	case deSynthesizerSourceWave::ewtSine:
		writer.WriteDataTagString( "type", "sine" );
		break;
		
	case deSynthesizerSourceWave::ewtSquare:
		writer.WriteDataTagString( "type", "square" );
		break;
		
	case deSynthesizerSourceWave::ewtSawTooth:
		writer.WriteDataTagString( "type", "sawtooth" );
		break;
		
	case deSynthesizerSourceWave::ewtTriangle:
		writer.WriteDataTagString( "type", "triangle" );
		break;
	}
	
	writer.WriteDataTagFloat( "minFrequency", source.GetMinFrequency() );
	writer.WriteDataTagFloat( "maxFrequency", source.GetMaxFrequency() );
	
	pWriteControllerTarget( writer, synthesizer, source.GetTargetFrequency(), "frequency" );
	
	writer.WriteClosingTag( "sourceWave" );
}

void seLoadSaveSynthesizer::pWriteSourceChain( decXmlWriter &writer,
const seSynthesizer &synthesizer, const seSourceChain &source ){
	writer.WriteOpeningTag( "sourceChain" );
	
	pWriteSourceCommon( writer, synthesizer, source );
	
	const decStringList &pathSounds = source.GetPathSounds();
	const int count = pathSounds.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		writer.WriteDataTagString( "sound", pathSounds.GetAt( i ) );
	}
	
	if( fabsf( source.GetMinSpeed() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "minSpeed", source.GetMinSpeed() );
	}
	if( fabsf( source.GetMaxSpeed() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "maxSpeed", source.GetMaxSpeed() );
	}
	
	pWriteControllerTarget( writer, synthesizer, source.GetTargetSpeed(), "speed" );
	pWriteControllerTarget( writer, synthesizer, source.GetTargetSelect(), "selection" );
	pWriteControllerTarget( writer, synthesizer, source.GetTargetPlay(), "play" );
	
	writer.WriteClosingTag( "sourceChain" );
}

void seLoadSaveSynthesizer::pWriteSourceGroup( decXmlWriter &writer,
const seSynthesizer &synthesizer, const seSourceGroup &source ){
	writer.WriteOpeningTag( "sourceGroup" );
	
	pWriteSourceCommon( writer, synthesizer, source );
	
	const int count = source.GetSources().GetCount();
	int i;
	for( i=0; i<count; i++ ){
		writer.WriteNewline();
		pWriteSource( writer, synthesizer, *source.GetSources().GetAt( i ) );
	}
	
	switch( source.GetApplicationType() ){
	case deSynthesizerSourceGroup::eatAll:
		writer.WriteDataTagString( "type", "all" );
		break;
		
	case deSynthesizerSourceGroup::eatSelect:
		writer.WriteDataTagString( "type", "selection" );
		break;
		
	case deSynthesizerSourceGroup::eatSolo:
		writer.WriteDataTagString( "type", "solo" );
		break;
	}
	
	pWriteControllerTarget( writer, synthesizer, source.GetTargetSelect(), "selection" );
	
	writer.WriteClosingTag( "sourceGroup" );
}

void seLoadSaveSynthesizer::pWriteSourceSynthesizer( decXmlWriter &writer,
const seSynthesizer &synthesizer, const seSourceSynthesizer &source ){
	const seControllerList &controllers = synthesizer.GetControllers();
	const int connectionCount = source.GetConnectionCount();
	int i;
	
	writer.WriteOpeningTag( "sourceSynthesizer" );
	
	pWriteSourceCommon( writer, synthesizer, source );
	
	writer.WriteDataTagString( "synthesizer", source.GetPathSynthesizer() );
	writer.WriteDataTagInt( "connectionCount", connectionCount );
	
	for( i=0; i<connectionCount; i++ ){
		seController * const controller = source.GetControllerAt( i );
		
		writer.WriteOpeningTagStart( "connection" );
		writer.WriteAttributeInt( "target", i );
		
		if( controller ){
			writer.WriteAttributeInt( "controller", controllers.IndexOf( controller ) );
			
		}else{
			writer.WriteAttributeInt( "controller", -1 );
		}
		
		writer.WriteOpeningTagEnd( true );
	}
	
	writer.WriteClosingTag( "sourceSynthesizer" );
}

void seLoadSaveSynthesizer::pWriteEffectCommon( decXmlWriter &writer,
const seSynthesizer &synthesizer, const seEffect &effect ){
	writer.WriteDataTagBool( "enabled", effect.GetEnabled() );
	
	if( fabsf( effect.GetStrength() - 1.0f ) > FLOAT_SAFE_EPSILON ){
		writer.WriteDataTagFloat( "strength", effect.GetStrength() );
	}
	
	pWriteControllerTarget( writer, synthesizer, effect.GetTargetStrength(), "strength" );
}

void seLoadSaveSynthesizer::pWriteEffect( decXmlWriter &writer,
const seSynthesizer &synthesizer, const seEffect &effect ){
	switch( effect.GetType() ){
	case deSynthesizerEffectVisitorIdentify::eetStretch:
		pWriteEffectStretch( writer, synthesizer, ( seEffectStretch& )effect );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void seLoadSaveSynthesizer::pWriteEffectStretch( decXmlWriter &writer,
const seSynthesizer &synthesizer, const seEffectStretch &effect ){
	writer.WriteOpeningTag( "effectStretch" );
	
	pWriteEffectCommon( writer, synthesizer, effect );
	
	writer.WriteDataTagFloat( "minTime", effect.GetMinTime() );
	writer.WriteDataTagFloat( "maxTime", effect.GetMaxTime() );
	writer.WriteDataTagFloat( "minPitch", effect.GetMinPitch() );
	writer.WriteDataTagFloat( "maxPitch", effect.GetMaxPitch() );
	
	pWriteControllerTarget( writer, synthesizer, effect.GetTargetTime(), "time" );
	pWriteControllerTarget( writer, synthesizer, effect.GetTargetPitch(), "pitch" );
	
	writer.WriteClosingTag( "effectStretch" );
}



void seLoadSaveSynthesizer::pReadSynthesizer( const decXmlElementTag &root, seSynthesizer &synthesizer ){
	int i;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "controller" ){
			pReadController( *tag, synthesizer );
			
		}else if( tagName == "link" ){
			pReadLink( *tag, synthesizer );
			
		}else if( tagName == "channels" ){
			synthesizer.SetChannelCount( GetCDataInt( *tag ) );
			
		}else if( tagName == "sampleRate" ){
			synthesizer.SetSampleRate( GetCDataInt( *tag ) );
			
		}else if( tagName == "bytesPerSample" ){
			synthesizer.SetBytesPerSample( GetCDataInt( *tag ) );
			
		}else if( tagName == "sampleCount" ){
			synthesizer.SetSampleCount( GetCDataInt( *tag ) );
			
		}else{
			seSource * const source = pReadSource( *tag, synthesizer );
			
			if( source ){
				synthesizer.AddSource( source );
				source->FreeReference();
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
	}
}

void seLoadSaveSynthesizer::pReadController( const decXmlElementTag &root, seSynthesizer &synthesizer ){
	seController *controller = NULL;
	int i;
	
	try{
		controller = new seController;
		synthesizer.AddController( controller );
		controller->FreeReference();
		
	}catch( const deException & ){
		if( controller ){
			controller->FreeReference();
		}
		throw;
	}
	
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "name" ){
			controller->SetName( GetCDataString( *tag ) );
			
		}else if( tagName == "clamp" ){
			controller->SetClamp( GetCDataBool( *tag ) );
			
		}else if( tagName == "limit" ){
			pReadControllerLimit( *tag, *controller );
			
		}else if( tagName == "limits" ){
			controller->SetMinimumValue( GetAttributeFloat( *tag, "min" ) );
			controller->SetMaximumValue( GetAttributeFloat( *tag, "max" ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void seLoadSaveSynthesizer::pReadControllerLimit(
const decXmlElementTag &root, seController &controller ){
	int i;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "minimum" ){
			controller.SetMinimumValue( GetCDataFloat( *tag ) );
			
		}else if( tagName == "maximum" ){
			controller.SetMaximumValue( GetCDataFloat( *tag ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void seLoadSaveSynthesizer::pReadLink( const decXmlElementTag &root, seSynthesizer &synthesizer ){
	seLink *link = NULL;
	int i;
	
	try{
		link = new seLink;
		
		for( i=0; i<root.GetElementCount(); i++ ){
			decXmlElementTag * const tag = root.GetElementIfTag( i );
			if( ! tag ){
				continue;
			}
			
			const decString tagName( tag->GetName() );
			
			if( tagName == "name" ){
				link->SetName( GetCDataString( *tag ) );
				
			}else if( tagName == "controller" ){
				const int index = GetCDataInt( *tag );
				
				if( index == -1 ){
					link->SetController( NULL );
					
				}else{
					link->SetController( synthesizer.GetControllers().GetAt( index ) );
				}
				
			}else if( tagName == "repeat" ){
				link->SetRepeat( GetCDataInt( *tag ) );
				
			}else if( tagName == "curve" ){
				ReadCurveBezier( *tag, link->GetCurve() );
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
		
		link->UpdateCurve();
		synthesizer.AddLink( link );
		
		link->FreeReference();
		
	}catch( const deException & ){
		if( link ){
			link->FreeReference();
		}
		throw;
	}
}

seSource *seLoadSaveSynthesizer::pReadSource( const decXmlElementTag &root, seSynthesizer &synthesizer ){
	const decString tagName( root.GetName() );
	
	if( tagName == "sourceSound" ){
		return pReadSourceSound( root, synthesizer );
		
	}else if( tagName == "sourceWave" ){
		return pReadSourceWave( root, synthesizer );
		
	}else if( tagName == "sourceChain" ){
		return pReadSourceChain( root, synthesizer );
		
	}else if( tagName == "sourceGroup" ){
		return pReadSourceGroup( root, synthesizer );
		
	}else if( tagName == "sourceSynthesizer" ){
		return pReadSourceSynthesizer( root, synthesizer );
	}
	
	return NULL;
}

seSource *seLoadSaveSynthesizer::pReadSourceSound( const decXmlElementTag &root, seSynthesizer &synthesizer ){
	seSourceSound *source = NULL;
	int i;
	
	try{
		source = new seSourceSound( synthesizer.GetEngine() );
		
		for( i=0; i<root.GetElementCount(); i++ ){
			decXmlElementTag * const tag = root.GetElementIfTag( i );
			if( ! tag ){
				continue;
			}
			
			if( pReadSourceCommon( *tag, synthesizer, *source ) ){
				continue;
			}
			
			const decString tagName( tag->GetName() );
			
			if( tagName == "sound" ){
				source->SetPathSound( GetCDataString( *tag ) );
				
			}else if( tagName == "minSpeed" ){
				source->SetMinSpeed( GetCDataFloat( *tag ) );
				
			}else if( tagName == "maxSpeed" ){
				source->SetMaxSpeed( GetCDataFloat( *tag ) );
				
			}else if( tagName == "looping" ){
				source->SetLooping( GetCDataBool( *tag ) );
				
			}else if( tagName == "target" ){
				const decString name( GetAttributeString( *tag, "name" ) );
				
				if( name == "blendFactor" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetBlendFactor() );
					
				}else if( name == "volume" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetVolume() );
					
				}else if( name == "panning" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetPanning() );
					
				}else if( name == "speed" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetSpeed() );
					
				}else if( name == "play" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetPlay() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
		
	}catch( const deException & ){
		if( source ){
			source->FreeReference();
		}
		throw;
	}
	
	return source;
}

seSource *seLoadSaveSynthesizer::pReadSourceWave( const decXmlElementTag &root, seSynthesizer &synthesizer ){
	seSourceWave *source = NULL;
	int i;
	
	try{
		source = new seSourceWave;
		
		for( i=0; i<root.GetElementCount(); i++ ){
			decXmlElementTag * const tag = root.GetElementIfTag( i );
			if( ! tag ){
				continue;
			}
			
			if( pReadSourceCommon( *tag, synthesizer, *source ) ){
				continue;
			}
			
			const decString tagName( tag->GetName() );
			
			if( tagName == "type" ){
				const decString type( GetCDataString( *tag ) );
				
				if( type == "sine" ){
					source->SetWaveType( deSynthesizerSourceWave::ewtSine );
					
				}else if( type == "square" ){
					source->SetWaveType( deSynthesizerSourceWave::ewtSquare );
					
				}else if( type == "sawtooth" ){
					source->SetWaveType( deSynthesizerSourceWave::ewtSawTooth );
					
				}else if( type == "triangle" ){
					source->SetWaveType( deSynthesizerSourceWave::ewtTriangle );
					
				}else{
					LogErrorUnknownValue( *tag, type );
				}
				
			}else if( tagName == "minFrequency" ){
				source->SetMinFrequency( GetCDataFloat( *tag ) );
				
			}else if( tagName == "maxFrequency" ){
				source->SetMaxFrequency( GetCDataFloat( *tag ) );
				
			}else if( tagName == "target" ){
				const decString name( GetAttributeString( *tag, "name" ) );
				
				if( name == "blendFactor" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetBlendFactor() );
					
				}else if( name == "volume" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetVolume() );
					
				}else if( name == "panning" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetPanning() );
					
				}else if( name == "frequency" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetFrequency() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
		
	}catch( const deException & ){
		if( source ){
			source->FreeReference();
		}
		throw;
	}
	
	return source;
}

seSource *seLoadSaveSynthesizer::pReadSourceChain( const decXmlElementTag &root, seSynthesizer &synthesizer ){
	seSourceChain *source = NULL;
	int i;
	
	try{
		source = new seSourceChain( synthesizer.GetEngine() );
		
		for( i=0; i<root.GetElementCount(); i++ ){
			decXmlElementTag * const tag = root.GetElementIfTag( i );
			if( ! tag ){
				continue;
			}
			
			if( pReadSourceCommon( *tag, synthesizer, *source ) ){
				continue;
			}
			
			const decString tagName( tag->GetName() );
			
			if( tagName == "sound" ){
				source->AddPathSound( GetCDataString( *tag ) );
				
			}else if( tagName == "minSpeed" ){
				source->SetMinSpeed( GetCDataFloat( *tag ) );
				
			}else if( tagName == "maxSpeed" ){
				source->SetMaxSpeed( GetCDataFloat( *tag ) );
				
			}else if( tagName == "target" ){
				const decString name( GetAttributeString( *tag, "name" ) );
				
				if( name == "blendFactor" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetBlendFactor() );
					
				}else if( name == "volume" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetVolume() );
					
				}else if( name == "panning" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetPanning() );
					
				}else if( name == "speed" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetSpeed() );
					
				}else if( name == "selection" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetSelect() );
					
				}else if( name == "play" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetPlay() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
		
	}catch( const deException & ){
		if( source ){
			source->FreeReference();
		}
		throw;
	}
	
	return source;
}

seSource *seLoadSaveSynthesizer::pReadSourceGroup( const decXmlElementTag &root, seSynthesizer &synthesizer ){
	const int elementCount = root.GetElementCount();
	seSourceGroup *source = NULL;
	int i;
	
	try{
		source = new seSourceGroup;
		
		for( i=0; i<elementCount; i++ ){
			decXmlElementTag * const tag = root.GetElementIfTag( i );
			if( ! tag ){
				continue;
			}
			
			if( pReadSourceCommon( *tag, synthesizer, *source ) ){
				continue;
			}
			
			const decString tagName( tag->GetName() );
			
			if( tagName == "type" ){
				const decString name( GetCDataString( *tag ) );
				
				if( name == "all" ){
					source->SetApplicationType( deSynthesizerSourceGroup::eatAll );
					
				}else if( name == "selection" ){
					source->SetApplicationType( deSynthesizerSourceGroup::eatSelect );
					
				}else if( name == "solo" ){
					source->SetApplicationType( deSynthesizerSourceGroup::eatSolo );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else if( tagName == "target" ){
				const decString name( GetAttributeString( *tag, "name" ) );
				
				if( name == "blendFactor" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetBlendFactor() );
					
				}else if( name == "volume" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetVolume() );
					
				}else if( name == "panning" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetPanning() );
					
				}else if( name == "selection" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetSelect() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				seSource * const childSource = pReadSource( *tag, synthesizer );
				
				if( childSource ){
					source->AddSource( childSource );
					childSource->FreeReference();
					
				}else{
					LogWarnUnknownTag( root, *tag );
				}
			}
		}
		
	}catch( const deException & ){
		if( source ){
			source->FreeReference();
		}
		throw;
	}
	
	return source;
}

seSource *seLoadSaveSynthesizer::pReadSourceSynthesizer( const decXmlElementTag &root, seSynthesizer &synthesizer ){
	seSourceSynthesizer *source = NULL;
	int i;
	
	try{
		source = new seSourceSynthesizer( synthesizer.GetEngine() );
		
		for( i=0; i<root.GetElementCount(); i++ ){
			decXmlElementTag * const tag = root.GetElementIfTag( i );
			if( ! tag ){
				continue;
			}
			
			if( pReadSourceCommon( *tag, synthesizer, *source ) ){
				continue;
			}
			
			const decString tagName( tag->GetName() );
			
			if( tagName == "synthesizer" ){
				source->SetPathSynthesizer( GetCDataString( *tag ) );
				
			}else if( tagName == "connectionCount" ){
				source->SetConnectionCount( GetCDataInt( *tag ) );
				
			}else if( tagName == "connection" ){
				const int target = GetAttributeInt( *tag, "target" );
				const int controller = GetAttributeInt( *tag, "controller" );
				
				if( controller == -1 ){
					source->SetControllerAt( target, NULL );
					
				}else{
					source->SetControllerAt( target, synthesizer.GetControllers().GetAt( controller ) );
				}
				
			}else if( strcmp( tag->GetName(), "target" ) == 0 ){
				const decString name( GetAttributeString( *tag, "name" ) );
				
				if( name == "blendFactor" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetBlendFactor() );
					
				}else if( name == "volume" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetVolume() );
					
				}else if( name == "panning" ){
					pReadControllerTarget( *tag, synthesizer, source->GetTargetPanning() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
		
	}catch( const deException & ){
		if( source ){
			source->FreeReference();
		}
		throw;
	}
	
	return source;
}

bool seLoadSaveSynthesizer::pReadSourceCommon( const decXmlElementTag &root,
seSynthesizer &synthesizer, seSource &source ){
	const decString tagName( root.GetName() );
	
	if( tagName == "name" ){
		source.SetName( GetCDataString( root ) );
		return true;
		
	}else if( tagName == "enabled" ){
		source.SetEnabled( GetCDataBool( root ) );
		return true;
		
	}else if( tagName == "mixMode" ){
		const decString name( GetCDataString( root ) );
		
		if( name == "blend" ){
			source.SetMixMode( deSynthesizerSource::emmBlend );
			
		}else if( name == "add" ){
			source.SetMixMode( deSynthesizerSource::emmAdd );
			
		}else{
			LogErrorUnknownValue( root, name );
		}
		
		return true;
		
	}else if( tagName == "blendFactor" ){
		source.SetBlendFactor( GetCDataFloat( root ) );
		return true;
		
	}else if( tagName == "minVolume" ){
		source.SetMinVolume( GetCDataFloat( root ) );
		return true;
		
	}else if( tagName == "maxVolume" ){
		source.SetMaxVolume( GetCDataFloat( root ) );
		return true;
		
	}else if( tagName == "minPanning" ){
		source.SetMinPanning( GetCDataFloat( root ) );
		return true;
		
	}else if( tagName == "maxPanning" ){
		source.SetMaxPanning( GetCDataFloat( root ) );
		return true;
		
	}else{
		seEffect * const effect = pReadEffect( root, synthesizer );
		if( effect ){
			source.AddEffect( effect );
			effect->FreeReference();
			return true;
		}
	}
	
	return false;
}

void seLoadSaveSynthesizer::pReadControllerTarget( const decXmlElementTag &root,
seSynthesizer &synthesizer, seControllerTarget &target ){
	int i;
	
	for( i=0; i<root.GetElementCount(); i++ ){
		decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString tagName( tag->GetName() );
		
		if( tagName == "link" ){
			target.AddLink( synthesizer.GetLinks().GetAt( GetCDataInt( *tag ) ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

bool seLoadSaveSynthesizer::pReadEffectCommon( const decXmlElementTag &root,
seSynthesizer &synthesizer, seEffect &effect ){
	const decString tagName( root.GetName() );
	
	if( tagName == "enabled" ){
		effect.SetEnabled( GetCDataBool( root ) );
		return true;
		
	}else if( tagName == "strength" ){
		effect.SetStrength( GetCDataFloat( root ) );
		return true;
	}
	
	return false;
}

seEffect *seLoadSaveSynthesizer::pReadEffect( const decXmlElementTag &root, seSynthesizer &synthesizer ){
	const decString tagName( root.GetName() );
	
	if( tagName == "effectStretch" ){
		return pReadEffectStretch( root, synthesizer );
	}
	
	return NULL;
}

seEffect *seLoadSaveSynthesizer::pReadEffectStretch( const decXmlElementTag &root, seSynthesizer &synthesizer ){
	seEffectStretch *effect = NULL;
	int i;
	
	try{
		effect = new seEffectStretch;
		
		for( i=0; i<root.GetElementCount(); i++ ){
			decXmlElementTag * const tag = root.GetElementIfTag( i );
			if( ! tag ){
				continue;
			}
			
			if( pReadEffectCommon( *tag, synthesizer, *effect ) ){
				continue;
			}
			
			const decString tagName( tag->GetName() );
			
			if( tagName == "minTime" ){
				effect->SetMinTime( GetCDataFloat( *tag ) );
				
			}else if( tagName == "maxTime" ){
				effect->SetMaxTime( GetCDataFloat( *tag ) );
				
			}else if( tagName == "minPitch" ){
				effect->SetMinPitch( GetCDataFloat( *tag ) );
				
			}else if( tagName == "maxPitch" ){
				effect->SetMaxPitch( GetCDataFloat( *tag ) );
				
			}else if( tagName == "target" ){
				const decString name( GetAttributeString( *tag, "name" ) );
				
				if( name == "strength" ){
					pReadControllerTarget( *tag, synthesizer, effect->GetTargetStrength() );
					
				}else if( name == "time" ){
					pReadControllerTarget( *tag, synthesizer, effect->GetTargetTime() );
					
				}else if( name == "pitch" ){
					pReadControllerTarget( *tag, synthesizer, effect->GetTargetPitch() );
					
				}else{
					LogErrorUnknownValue( *tag, name );
				}
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
		
	}catch( const deException & ){
		if( effect ){
			effect->FreeReference();
		}
		throw;
	}
	
	return effect;
}
