/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglConfiguration.h"
#include "deoglLSConfiguration.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class deoglLSConfiguration
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLSConfiguration::deoglLSConfiguration( deGraphicOpenGl &ogl ) :
pOgl( ogl ){
}

deoglLSConfiguration::~deoglLSConfiguration(){
}



// Management
///////////////

void deoglLSConfiguration::LoadConfig( deoglConfiguration &configuration ){
	deVirtualFileSystem& vfs = pOgl.GetVFS();
	decBaseFileReader *file = NULL;
	decPath path;
	
	try{
		path.SetFromUnix( "/config/opengl.xml" );
		if( vfs.ExistsFile( path ) ){
			pOgl.LogInfo( "Loading configuration file 'opengl.xml'" );
			file = vfs.OpenFileForReading( path );
			if( ! file ){
				DETHROW( deeOutOfMemory );
			}
			
			pLoadConfigOpenGL( configuration, *file );
			
			file->FreeReference();
			file = NULL;
			
		}else{
			pOgl.LogInfo( "Configuration file 'opengl.xml' not found. Using defaults." );
		}
		
	}catch( const deException &e ){
		if( file ){
			file->FreeReference();
		}
		pOgl.LogException( e );
		throw;
	}
}

void deoglLSConfiguration::SaveConfig( const deoglConfiguration & ){
}



// Private Functions
//////////////////////

void deoglLSConfiguration::pLoadConfigOpenGL( deoglConfiguration &configuration, decBaseFileReader &file ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	const char *name;
	int i;
	
	// parse xml
	decXmlParser( pOgl.GetGameEngine()->GetLogger() ).ParseXml( &file, xmlDoc );
	
	// some cleanup visiting
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	// interpretate xml
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "config" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	// read properties
	for( i=0; i<root->GetElementCount(); i++ ){
		decXmlElementTag * const tag = pGetTagAt( root, i );
		if( tag ){
			if( strcmp( tag->GetName(), "property" ) == 0 ){
				name = pGetAttributeString( tag, "name" );
				
				if( strcmp( name, "logLevel" ) == 0 ){
					const decString &value = tag->GetFirstData()->GetData();
					if( value == "error" ){
						configuration.SetLogLevel( deoglConfiguration::ellError );
						
					}else if( value == "warning" ){
						configuration.SetLogLevel( deoglConfiguration::ellWarning );
						
					}else if( value == "info" ){
						configuration.SetLogLevel( deoglConfiguration::ellInfo );
						
					}else if( value == "debug" ){
						configuration.SetLogLevel( deoglConfiguration::ellDebug );
						
					}else{
						pOgl.LogWarnFormat( "opengl.xml %s(%i:%i): Invalid value '%s' for property %s.",
							tag->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), value.GetString(), name );
					}
					
				}else if( strcmp( name, "debugShowCB" ) == 0 ){
					configuration.SetDebugShowCB( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "reducedLighting" ) == 0 ){
					configuration.SetUseReducedLighting( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "skyBodyDensity" ) == 0 ){
					configuration.SetSkyBodyDensity( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "wireframeMode" ) == 0 ){
					configuration.SetDebugWireframe( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "debugUVSplitter" ) == 0 ){
					configuration.SetDebugUVSplitter( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "debugPrintSkyUpdate" ) == 0 ){
					configuration.SetDebugPrintSkyUpdate( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "showLightCB" ) == 0 ){
					configuration.SetShowLightCB( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "mapFaceSplitThreshold" ) == 0 ){
					configuration.SetMapFaceSplitThreshold( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "shadowQuality" ) == 0 ){
					const decString value( tag->GetFirstData()->GetData() );
					if( value == "veryHigh" ){
						configuration.SetShadowQuality( deoglConfiguration::esqVeryHigh );
						
					}else if( value == "high" ){
						configuration.SetShadowQuality( deoglConfiguration::esqHigh );
						
					}else if( value == "medium" ){
						configuration.SetShadowQuality( deoglConfiguration::esqMedium );
						
					}else if( value == "low" ){
						configuration.SetShadowQuality( deoglConfiguration::esqLow );
						
					}else if( value == "veryLow" ){
						configuration.SetShadowQuality( deoglConfiguration::esqVeryLow );
						
					}else if( value == "off" ){
						configuration.SetShadowQuality( deoglConfiguration::esqOff );
						
					}else{
						pOgl.LogWarnFormat( "opengl.xml %s(%i:%i): Invalid property value %s.",
							tag->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), value.GetString() );
					}
					
				}else if( strcmp( name, "shadowMapOffsetScale" ) == 0 ){
					configuration.SetShadowMapOffsetScale( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "shadowMapOffsetBias" ) == 0 ){
					configuration.SetShadowMapOffsetBias( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "shadowCubePCFSize" ) == 0 ){
					configuration.SetShadowCubePCFSize( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "occlusionReduction" ) == 0 ){
					configuration.SetOcclusionReduction( (int)strtol( tag->GetFirstData()->GetData(), NULL, 10 ) );
					
				}else if( strcmp( name, "distShadowScale" ) == 0 ){
					configuration.SetDistShadowScale( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "distShadowBias" ) == 0 ){
					configuration.SetDistShadowBias( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "textOffsetU" ) == 0 ){
					configuration.SetTextOffsetU( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "textOffsetV" ) == 0 ){
					configuration.SetTextOffsetV( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "useDepthPass" ) == 0 ){
					configuration.SetUseDepthPass( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "useOneFBO" ) == 0 ){
					configuration.SetUseOneFBO( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "useEncodeDepth" ) == 0 ){
					configuration.SetUseEncodeDepth( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "useShadowCubeEncodeDepth" ) == 0 ){
					configuration.SetUseShadowCubeEncodeDepth( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "disableStencil" ) == 0 ){
					configuration.SetDisableStencil( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "stencilOnlyOnRenderbuffer" ) == 0 ){
					configuration.SetStencilOnlyOnRB( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "lightCutOffIntensity" ) == 0 ){
					configuration.SetLightCutOffIntensity( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "useTextureCompression" ) == 0 ){
					configuration.SetUseTextureCompression( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "defRenEncDepth" ) == 0 ){
					configuration.SetDefRenEncDepth( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "defRenUsePOTs" ) == 0 ){
					configuration.SetDefRenUsePOTs( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "defRenSizeLimit" ) == 0 ){
					configuration.SetDefRenSizeLimit( (int)strtol( tag->GetFirstData()->GetData(), NULL, 10 ) );
					
				}else if( strcmp( name, "useHDRR" ) == 0 ){
					configuration.SetUseHDRR( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "hdrrMaximumIntensity" ) == 0 ){
					configuration.SetHDRRMaximumIntensity( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "renderDownScale" ) == 0 ){
					configuration.SetRenderDownScale( tag->GetFirstData()->GetData().ToInt() );
					
				}else if( strcmp( name, "useInverseDepth" ) == 0 ){
					configuration.SetUseInverseDepth( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "gammaCorrection" ) == 0 ){
					configuration.SetGammaCorrection( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "contrast" ) == 0 ){
					configuration.SetContrast( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "brightness" ) == 0 ){
					configuration.SetBrightness( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "decalOffsetScale" ) == 0 ){
					configuration.SetDecalOffsetScale( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "decalOffsetBias" ) == 0 ){
					configuration.SetDecalOffsetBias( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "lodMaxPixelError" ) == 0 ){
					configuration.SetLODMaxPixelError( (int)strtol( tag->GetFirstData()->GetData(), NULL, 10 ) );
					
				}else if( strcmp( name, "lodMaxErrorPerLevel" ) == 0 ){
					configuration.SetLODMaxErrorPerLevel( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
					
					
				}else if( strcmp( name, "normalRoughnessCorrectionStrength" ) == 0 ){
					configuration.SetNormalRoughnessCorrectionStrength( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
					
					
				}else if( strcmp( name, "ssrEnable" ) == 0 ){
					configuration.SetSSREnable( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "ssrStepCount" ) == 0 ){
					configuration.SetSSRStepCount( (int)strtol( tag->GetFirstData()->GetData(), NULL, 10 ) );
					
				}else if( strcmp( name, "ssrMaxRayLength" ) == 0 ){
					configuration.SetSSRMaxRayLength( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "ssrReduction" ) == 0 ){
					configuration.SetSSRReduction( (int)strtol( tag->GetFirstData()->GetData(), NULL, 10 ) );
					
				}else if( strcmp( name, "ssrCoverageEdgeSize" ) == 0 ){
					configuration.SetSSRCoverageEdgeSize( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "ssrCoveragePowerEdge" ) == 0 ){
					configuration.SetSSRCoveragePowerEdge( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "ssrCoveragePowerRayLength" ) == 0 ){
					configuration.SetSSRCoveragePowerRayLength( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "ssrMethod" ) == 0 ){
					configuration.SetSSRMethod( (int)strtol( tag->GetFirstData()->GetData(), NULL, 10 ) );
					
					
					
				}else if( strcmp( name, "ssaoEnable" ) == 0 ){
					configuration.SetSSAOEnable( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "ssaoTapCount" ) == 0 ){
					configuration.SetSSAOTapCount( (int)strtol( tag->GetFirstData()->GetData(), NULL, 10 ) );
					
				}else if( strcmp( name, "ssaoTurnCount" ) == 0 ){
					configuration.SetSSAOTurnCount( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "ssaoSelfOcclusionAngle" ) == 0 ){
					configuration.SetSSAOSelfOcclusionAngle( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "ssaoRadius" ) == 0 ){
					configuration.SetSSAORadius( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "ssaoRadiusLimit" ) == 0 ){
					configuration.SetSSAORadiusLimit( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "ssaoMipMapBase" ) == 0 ){
					configuration.SetSSAOMipMapBase( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
				}else if( strcmp( name, "ssaoEdgeBlurThreshold" ) == 0 ){
					configuration.SetSSAOEdgeBlurThreshold( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
					
					
				}else if( strcmp( name, "aoSelfShadowEnable" ) == 0 ){
					configuration.SetAOSelfShadowEnable( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "aoSelfShadowSmoothAngle" ) == 0 ){
					configuration.SetAOSelfShadowSmoothAngle( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
					
					
				}else if( strcmp( name, "sssssEnable" ) == 0 ){
					configuration.SetSSSSSEnable( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
					
					
				}else if( strcmp( name, "frameRateLimit" ) == 0 ){
					configuration.SetFrameRateLimit( (int)strtol( tag->GetFirstData()->GetData(), NULL, 10 ) );
					
				}else if( strcmp( name, "asyncRenderSkipSyncTimeRatio" ) == 0 ){
					configuration.SetAsyncRenderSkipSyncTimeRatio( strtof( tag->GetFirstData()->GetData(), NULL ) );
					
					
					
				}else if( strcmp( name, "envMapEnable" ) == 0 ){
					configuration.SetEnvMapEnable( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "bugNo2ComponentFBOTex" ) == 0 ){
					configuration.SetBugNo2ComponentFBOTex( (int)strtol( tag->GetFirstData()->GetData(), NULL, 10 ) );
					
				}else if( strcmp( name, "occlusionTestMode" ) == 0 ){
					configuration.SetOcclusionTestMode( ( deoglConfiguration::eOcclusionTestModes )
						strtol( tag->GetFirstData()->GetData(), NULL, 10 ) );
					
				}else if( strcmp( name, "disableCubeMapLinearFiltering" ) == 0 ){
					configuration.SetDisableCubeMapLinearFiltering( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
					
					
				}else if( strcmp( name, "debugContext" ) == 0 ){
					configuration.SetDebugContext( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "asyncRendering" ) == 0 ){
					configuration.SetAsyncRendering( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
				}else if( strcmp( name, "enableRetainImageOptimization" ) == 0 ){
					configuration.SetEnableRetainImageOptimization( strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
					
					
				}else if( strcmp( name, "maxSPBIndexCount" ) == 0 ){
					configuration.SetMaxSPBIndexCount( (int)strtol( tag->GetFirstData()->GetData(), NULL, 10 ) != 0 );
					
					
					
				}else if( strcmp( name, "giQuality" ) == 0 ){
					const decString value( tag->GetFirstData()->GetData() );
					if( value == "veryHigh" ){
						configuration.SetGIQuality( deoglConfiguration::egiqVeryHigh );
						
					}else if( value == "high" ){
						configuration.SetGIQuality( deoglConfiguration::egiqHigh );
						
					}else if( value == "medium" ){
						configuration.SetGIQuality( deoglConfiguration::egiqMedium );
						
					}else if( value == "low" ){
						configuration.SetGIQuality( deoglConfiguration::egiqLow );
						
					}else if( value == "veryLow" ){
						configuration.SetGIQuality( deoglConfiguration::egiqVeryLow );
						
					}else if( value == "off" ){
						configuration.SetGIQuality( deoglConfiguration::egiqOff );
						
					}else{
						pOgl.LogWarnFormat( "opengl.xml %s(%i:%i): Invalid property value %s.",
							tag->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), value.GetString() );
					}
					
				}else if( strcmp( name, "giUpdateSpeed" ) == 0 ){
					const decString value( tag->GetFirstData()->GetData() );
					if( value == "veryHigh" ){
						configuration.SetGIUpdateSpeed( deoglConfiguration::egiusVeryHigh );
						
					}else if( value == "high" ){
						configuration.SetGIUpdateSpeed( deoglConfiguration::egiusHigh );
						
					}else if( value == "medium" ){
						configuration.SetGIUpdateSpeed( deoglConfiguration::egiusMedium );
						
					}else if( value == "low" ){
						configuration.SetGIUpdateSpeed( deoglConfiguration::egiusLow );
						
					}else if( value == "veryLow" ){
						configuration.SetGIUpdateSpeed( deoglConfiguration::egiusVeryLow );
						
					}else{
						pOgl.LogWarnFormat( "opengl.xml %s(%i:%i): Invalid property value %s.",
							tag->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), value.GetString() );
					}
					
				}else{
					pOgl.LogWarnFormat( "opengl.xml %s(%i:%i): Invalid property name %s.",
						tag->GetName().GetString(), tag->GetLineNumber(),
						tag->GetPositionNumber(), name );
				}
				
			}else if( tag->GetName() == "disableExtension" ){
				configuration.GetDisableExtensions().Add( tag->GetFirstData()->GetData() );
				
			}else{
				pOgl.LogWarnFormat( "opengl.xml %s(%i:%i): Unknown Tag %s.",
					root->GetName().GetString(), tag->GetLineNumber(),
					tag->GetPositionNumber(), tag->GetName().GetString() );
			}
		}
	}
}

decXmlElementTag *deoglLSConfiguration::pGetTagAt( decXmlElementTag *tag, int index ){
	decXmlElement *element = tag->GetElementAt( index );
	if( element->CanCastToElementTag() ){
		return element->CastToElementTag();
	}else{
		return NULL;
	}
}

decXmlAttValue *deoglLSConfiguration::pFindAttribute( decXmlElementTag *tag, const char *name ){
	decXmlAttValue *value;
	decXmlElement *element;
	int i;
	for( i=0; i<tag->GetElementCount(); i++ ){
		element = tag->GetElementAt( i );
		if( element->CanCastToAttValue() ){
			value = element->CastToAttValue();
			if( strcmp( value->GetName(), name ) == 0 ){
				return value;
			}
		}
	}
	return NULL;
}

const char *deoglLSConfiguration::pGetAttributeString( decXmlElementTag *tag, const char *name ){
	decXmlAttValue *value = pFindAttribute( tag, name );
	if( value ){
		return value->GetValue();
	}else{
		pOgl.LogErrorFormat( "Missing Attribute '%s' in tag '%s'.",
			name, tag->GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

int deoglLSConfiguration::pGetAttributeInt( decXmlElementTag *tag, const char *name ){
	decXmlAttValue *value = pFindAttribute( tag, name );
	if( value ){
		return (int)strtol( value->GetValue(), NULL, 10 );
	}else{
		pOgl.LogErrorFormat( "Missing Attribute '%s' in tag '%s'.",
			name, tag->GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

float deoglLSConfiguration::pGetAttributeFloat( decXmlElementTag *tag, const char *name ){
	decXmlAttValue *value = pFindAttribute( tag, name );
	if( value ){
		return strtof( value->GetValue(), NULL );
	}else{
		pOgl.LogErrorFormat( "Missing Attribute '%s' in tag '%s'.",
			name, tag->GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}
