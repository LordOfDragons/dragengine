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

#include "deoglDeveloperModeStats.h"
#include "../deoglBasics.h"
#include "../configuration/deoglConfiguration.h"
#include "../capabilities/deoglCapabilities.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../extensions/deoglExtensions.h"
#include "../sptree/deoglSPTree.h"
#include "../skin/deoglSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/combinedTexture/deoglCombinedTexture.h"
#include "../skin/combinedTexture/deoglCombinedTextureList.h"
#include "../skin/shader/deoglSkinShaderManager.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../skin/deoglRSkin.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderSources.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderDefines.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/deoglImage.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/cubemap/deoglRenderableColorCubeMap.h"
#include "../texture/cubemap/deoglRenderableColorCubeMapManager.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMapManager.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../texture/texture2d/deoglRenderableColorTexture.h"
#include "../texture/texture2d/deoglRenderableColorTextureManager.h"
#include "../texture/texture2d/deoglRenderableDepthTexture.h"
#include "../texture/texture2d/deoglRenderableDepthTextureManager.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../texture/texsamplerconfig/deoglTexSamplerConfig.h"
#include "../texture/deoglRImage.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglSharedVBO.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../vbo/deoglSharedVBOList.h"
#include "../vbo/deoglSharedVBOListList.h"
#include "../vbo/deoglVBOAttribute.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTTexture.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/exceptions.h>



// Class deoglDeveloperModeStats
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDeveloperModeStats::deoglDeveloperModeStats( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
}

deoglDeveloperModeStats::~deoglDeveloperModeStats(){
}



// Management
///////////////

void deoglDeveloperModeStats::Help( decUnicodeString &answer ){
	answer.SetFromUTF8( "dm_stats <mode>\n" );
	answer.AppendFromUTF8( "where <mode> can be:\n" );
	answer.AppendFromUTF8( "skins => Displays skin stats ( potentially huge list ).\n" );
	answer.AppendFromUTF8( "combined_textures => Displays combined textures stats ( large list ).\n" );
	answer.AppendFromUTF8( "shader_sources => Displays shader sources stats ( potentially huge list ).\n" );
	answer.AppendFromUTF8( "shader_programs => Displays shader programs stats ( potentially huge list ).\n" );
	answer.AppendFromUTF8( "skin_shaders => Displays skin shader stats ( potentially huge list ).\n" );
	answer.AppendFromUTF8( "renderables_color_texture => Displays renderable color textures stats.\n" );
	answer.AppendFromUTF8( "renderables_depth_texture => Displays renderable depth textures stats.\n" );
	answer.AppendFromUTF8( "renderables_color_cubemap => Displays renderable color cubemaps stats.\n" );
	answer.AppendFromUTF8( "renderables_depth_cubemap => Displays renderable depth cubemaps stats.\n" );
	answer.AppendFromUTF8( "shared_vbos => Displays shared vbo stats (large list).\n" );
	answer.AppendFromUTF8( "texture_units_configuration {verbose} => Displays texture units configurations stats (large list).\n" );
}

void deoglDeveloperModeStats::Stats( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() > 1 ){
		if( command.MatchesArgumentAt( 1, "skins" ) ){
			Skins( command, answer );
			
		}else if( command.MatchesArgumentAt( 1, "combined_textures" ) ){
			CombinedTextures( command, answer );
			
		}else if( command.MatchesArgumentAt( 1, "shader_sources" ) ){
			ShaderSources( command, answer );
			
		}else if( command.MatchesArgumentAt( 1, "shader_programs" ) ){
			ShaderPrograms( command, answer);
			
		}else if( command.MatchesArgumentAt( 1, "skin_shaders" ) ){
			SkinShaders( command, answer);
			
		}else if( command.MatchesArgumentAt( 1, "renderables_color_texture" ) ){
			RenderablesTexturesColor( command, answer );
			
		}else if( command.MatchesArgumentAt( 1, "renderables_depth_texture" ) ){
			RenderablesTexturesDepth( command, answer );
			
		}else if( command.MatchesArgumentAt( 1, "renderables_color_cubemap" ) ){
			RenderablesCubeColor( command, answer );
			
		}else if( command.MatchesArgumentAt( 1, "renderables_depth_cubemap" ) ){
			RenderablesCubeDepth( command, answer );
			
		}else if( command.MatchesArgumentAt( 1, "shared_vbos" ) ){
			SharedVBOs( command, answer );
			
		}else if( command.MatchesArgumentAt( 1, "texture_units_configuration" ) || command.MatchesArgumentAt( 1, "tuc" ) ){
			TextureUnitsConfigurations( command, answer );
			
		}else{
			Help( answer );
		}
		
	}else{
		Help( answer );
	}
}



void deoglDeveloperModeStats::Skins( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	//deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	deSkinManager *skinMgr = pRenderThread.GetOgl().GetGameEngine()->GetSkinManager();
	deSkin *skin = skinMgr->GetRootSkin();
	int c, t, textureCount;
	deoglRSkin *oglSkin;
	decString text;
	
	while( skin ){
		oglSkin = ( ( deoglSkin* )skin->GetPeerGraphic() )->GetRSkin();
		
		text.Format( "- Skin '%s' ( renderables=%d dynamic=%d solid=%d hasHoles=%d ):\n",
			skin->GetFilename().GetString(),
			oglSkin->GetHasRenderables() ? 1 : 0, oglSkin->GetHasDynamicChannels() ? 1 : 0,
			oglSkin->GetIsSolid() ? 1 : 0, oglSkin->GetHasHoles() ? 1 : 0 );
		answer.AppendFromUTF8( text.GetString() );
		
		textureCount = oglSkin->GetTextureCount();
		for( t=0; t<textureCount; t++ ){
			deoglSkinTexture &oglSkinTexture = oglSkin->GetTextureAt( t );
			
			text.Format( "   - Texture '%s' ( maskedSolidity=%d solid=%d hasHoles=%d ):\n", oglSkinTexture.GetName().GetString(),
				oglSkinTexture.GetSolidityMasked() ? 1 : 0, oglSkinTexture.GetSolid() ? 1 : 0, oglSkinTexture.GetHasHoles() ? 1 : 0 );
			answer.AppendFromUTF8( text.GetString() );
			
			for( c=0; c<deoglSkinChannel::CHANNEL_COUNT; c++ ){
				if( oglSkinTexture.IsChannelEnabled( ( deoglSkinChannel::eChannelTypes )c ) ){
					const deoglSkinChannel &oglSkinChannel = *oglSkinTexture.GetChannelAt( ( deoglSkinChannel::eChannelTypes )c );
					
					text.Format( "      - Channel '%s' ( size=%dx%dx%d uniform=%d dynamic=%d",
						deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )c ),
						oglSkinChannel.GetSize().x, oglSkinChannel.GetSize().y, oglSkinChannel.GetSize().z,
						oglSkinChannel.GetUniform() ? 1 : 0, oglSkinChannel.GetDynamic() ? 1 : 0 );
					
					if( oglSkinChannel.GetImage() ){
						text.Append( " image" );
						
					}else if( oglSkinChannel.GetCombinedTexture() ){
						text.Append( " combined" );
						
					}else{
						text.Append( " unique" );
					}
					
					text.Append( " )\n" );
					answer.AppendFromUTF8( text.GetString() );
				}
			}
		}
		
		skin = ( deSkin* )skin->GetLLManagerNext();
	}
}

void deoglDeveloperModeStats::CombinedTextures( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	deoglCombinedTexture *combinedTexture = pRenderThread.GetTexture().GetCombinedTexture().GetRoot();
	decString text;
	
	while( combinedTexture ){
		const decColor &color = combinedTexture->GetColor();
		
		text.Format( "- color=(%d,%d,%d,%d) images=(%p,%p,%p,%p) usage=%d\n",
			( int )( color.r * 255.0 ), ( int )( color.g * 255.0 ),
			( int )( color.b * 255.0 ), ( int )( color.a * 255.0 ),
			( deoglRImage* )combinedTexture->GetImageAt( 0 ),
			( deoglRImage* )combinedTexture->GetImageAt( 1 ),
			( deoglRImage* )combinedTexture->GetImageAt( 2 ),
			( deoglRImage* )combinedTexture->GetImageAt( 3 ),
			combinedTexture->GetUsageCount() );
		answer.AppendFromUTF8( text.GetString() );
		
		combinedTexture = combinedTexture->GetLLNext();
	}
}

void deoglDeveloperModeStats::ShaderSources( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	const decObjectList smsources( pRenderThread.GetShader().GetShaderManager().GetSourcesAsList() );
	int ss, sst, ssp;
	decString text;
	
	const int shaderSourcesCount = smsources.GetCount();
	for( ss=0; ss<shaderSourcesCount; ss++ ){
		const deoglShaderSources &sources = *( deoglShaderSources* )smsources.GetAt( ss );
		const deoglShaderBindingList &textureList = sources.GetTextureList();
		const decStringList &parameterList = sources.GetParameterList();
		
		text.Format( "- Shader Sources '%s' ( shader file '%s' ):\n", sources.GetName().GetString(), sources.GetFilename().GetString() );
		answer.AppendFromUTF8( text.GetString() );
		
		if( ! sources.GetPathGeometrySourceCode().IsEmpty() ){
			text.Format( "   - Geometry Source Code = %s\n", sources.GetPathGeometrySourceCode().GetString() );
			answer.AppendFromUTF8( text.GetString() );
		}
		
		if( ! sources.GetPathVertexSourceCode().IsEmpty() ){
			text.Format( "   - Vertex Source Code = %s\n", sources.GetPathVertexSourceCode().GetString() );
			answer.AppendFromUTF8( text.GetString() );
		}
		
		if( ! sources.GetPathFragmentSourceCode().IsEmpty() ){
			text.Format( "   - Fragment Source Code = %s\n", sources.GetPathFragmentSourceCode().GetString() );
			answer.AppendFromUTF8( text.GetString() );
		}
		
		const int shaderSourcesTextureCount = textureList.GetCount();
		text.Set( "   - Textures:" );
		for( sst=0; sst<shaderSourcesTextureCount; sst++ ){
			text.AppendFormat( "%s %s(%d)", sst == 0 ? "" : ",",
				textureList.GetNameAt( sst ).GetString(), textureList.GetTargetAt( sst ) );
		}
		text.Append( "\n" );
		answer.AppendFromUTF8( text.GetString() );
		
		const int shaderSourcesParameterCount = parameterList.GetCount();
		text.Set( "   - Parameters:" );
		for( ssp=0; ssp<shaderSourcesParameterCount; ssp++ ){
			text.AppendFormat( "%s %s", ssp == 0 ? "" : ",", parameterList.GetAt( ssp ).GetString() );
		}
		text.Append( "\n" );
		answer.AppendFromUTF8( text.GetString() );
	}
}

void deoglDeveloperModeStats::ShaderPrograms( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	deoglShaderManager &shaderManager = pRenderThread.GetShader().GetShaderManager();
	const char *defineName, *defineValue;
	int p, programCount;
	int d, defineCount;
	decString text;
	
	programCount = shaderManager.GetProgramCount();
	for( p=0; p<programCount; p++ ){
		const deoglShaderProgram &program = *shaderManager.GetProgramAt( p );
		const deoglShaderDefines &defines = program.GetDefines();
		
		text.Format( "- Shader '%s' Defines(", program.GetSources()->GetName().GetString() );
		
		defineCount = defines.GetDefineCount();
		for( d=0; d<defineCount; d++ ){
			defineName = defines.GetDefineNameAt( d );
			defineValue = defines.GetDefineValueAt( d );
			
			if( strlen( defineValue ) > 10 ){
				text.AppendFormat( "%s %s=%.10s...", d == 0 ? "" : ",", defineName, defineValue );
				
			}else{
				text.AppendFormat( "%s %s=%s", d == 0 ? "" : ",", defineName, defineValue );
			}
		}
		text.Append( " )\n" );
		answer.AppendFromUTF8( text.GetString() );
	}
}

void deoglDeveloperModeStats::SkinShaders( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	deoglSkinShaderManager &manager = pRenderThread.GetShader().GetSkinShaderManager();
	const int shaderCount = manager.GetShaderCount();
	decString text, configString;
	int i;
	
	text.Format( "%d skin shaders\n", shaderCount );
	answer.AppendFromUTF8( text.GetString() );
	
	for( i=0; i<shaderCount; i++ ){
		const deoglSkinShader &shader = manager.GetShaderAt( i );
		
		shader.GetConfig().DebugGetConfigString( configString );
		
		text.Format( "- usage=%d config=", shader.GetRefCount() - 1 );
		answer.AppendFromUTF8( text.GetString() );
		answer.AppendFromUTF8( configString.GetString() );
		answer.AppendFromUTF8( "\n" );
	}
}

void deoglDeveloperModeStats::RenderablesTexturesColor( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	deoglRenderableColorTextureManager &manager = pRenderThread.GetTexture().GetRenderableColorTexture();
	int t, textureCount = manager.GetTextureCount();
	const deoglRenderableColorTexture *texture;
	int memoryConsumption = 0;
	decString text;
	int texMem;
	
	for( t=0; t<textureCount; t++ ){
		texture = manager.GetTextureAt( t );
		
		text.Format( "- %d x %d: components=%d float=%s used=%s\n", texture->GetWidth(),
			texture->GetHeight(), texture->GetComponentCount(),
			texture->GetIsFloat() ? "y" : "n", texture->GetInUse() ? "y" : "n" );
		answer.AppendFromUTF8( text.GetString() );
		
		texMem = texture->GetWidth() * texture->GetHeight() * texture->GetComponentCount();
		if( texture->GetIsFloat() ) texMem *= 2;
		
		memoryConsumption += texMem;
	}
	
	text.Format( "%d Textures consuming %.1fMB GPU RAM\n", textureCount, ( float )memoryConsumption * 1e-6f );
	answer.AppendFromUTF8( text.GetString() );
}

void deoglDeveloperModeStats::RenderablesTexturesDepth( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	deoglRenderableDepthTextureManager &manager = pRenderThread.GetTexture().GetRenderableDepthTexture();
	int t, textureCount = manager.GetTextureCount();
	const deoglRenderableDepthTexture *texture;
	int memoryConsumption = 0;
	decString text;
	int texMem;
	
	for( t=0; t<textureCount; t++ ){
		texture = manager.GetTextureAt( t );
		
		text.Format( "- %d x %d: withStencil=%s used=%s\n", texture->GetWidth(),
			texture->GetHeight(), texture->GetWithStencil() ? "y" : "n", texture->GetInUse() ? "y" : "n" );
		answer.AppendFromUTF8( text.GetString() );
		
		texMem = texture->GetWidth() * texture->GetHeight();
		if( texture->GetWithStencil() ){
			texMem *= 4;
			
		}else{
			texMem *= 3; // maybe 4? not sure
		}
		
		memoryConsumption += texMem;
	}
	
	text.Format( "%d Textures consuming %.1fMB GPU RAM\n", textureCount, ( float )memoryConsumption * 1e-6f );
	answer.AppendFromUTF8( text.GetString() );
}

void deoglDeveloperModeStats::RenderablesCubeColor( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	deoglRenderableColorCubeMapManager &manager = pRenderThread.GetTexture().GetRenderableColorCubeMap();
	int t, cubemapCount = manager.GetCubeMapCount();
	const deoglRenderableColorCubeMap *cubemap;
	int memoryConsumption = 0;
	decString text;
	int texMem;
	
	for( t=0; t<cubemapCount; t++ ){
		cubemap = manager.GetCubeMapAt( t );
		
		text.Format( "- %d x %d: components=%d float=%s used=%s\n", cubemap->GetSize(),
			cubemap->GetSize(), cubemap->GetComponentCount(),
			cubemap->GetIsFloat() ? "y" : "n", cubemap->GetInUse() ? "y" : "n" );
		answer.AppendFromUTF8( text.GetString() );
		
		texMem = cubemap->GetSize() * cubemap->GetSize() * cubemap->GetComponentCount();
		if( cubemap->GetIsFloat() ) texMem *= 2;
		
		memoryConsumption += texMem * 6;
	}
	
	text.Format( "%d CubeMaps consuming %.1fMB GPU RAM\n", cubemapCount, ( float )memoryConsumption * 1e-6f );
	answer.AppendFromUTF8( text.GetString() );
}

void deoglDeveloperModeStats::RenderablesCubeDepth( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	deoglRenderableDepthCubeMapManager &manager = pRenderThread.GetTexture().GetRenderableDepthCubeMap();
	int t, cubemapCount = manager.GetCubeMapCount();
	const deoglRenderableDepthCubeMap *cubemap;
	int memoryConsumption = 0;
	decString text;
	int texMem;
	
	for( t=0; t<cubemapCount; t++ ){
		cubemap = manager.GetCubeMapAt( t );
		
		text.Format( "- %d x %d: used=%s\n", cubemap->GetSize(), cubemap->GetSize(), cubemap->GetInUse() ? "y" : "n" );
		answer.AppendFromUTF8( text.GetString() );
		
		texMem = cubemap->GetSize() * cubemap->GetSize() * 3; // maybe 4? not sure
		
		memoryConsumption += texMem * 6;
	}
	
	text.Format( "%d CubeMaps consuming %.1fMB GPU RAM\n", cubemapCount, ( float )memoryConsumption * 1e-6f );
	answer.AppendFromUTF8( text.GetString() );
}

void deoglDeveloperModeStats::SharedVBOs( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	const deoglSharedVBOListList &listList = pRenderThread.GetBufferObject().GetSharedVBOListList();
	const int listCount = listList.GetCount();
	decString text;
	int drawType;
	int i, j, k;
	
	for( i=0; i<listCount; i++ ){
		const deoglSharedVBOList &list = *listList.GetAt( i );
		const deoglVBOLayout &layout = list.GetLayout();
		const int attributeCount = layout.GetAttributeCount();
		const int stride = layout.GetStride();
		const int vboCount = list.GetCount();
		
		// list
		text.Format( "- list %d (maxSize=(%d|%d) maxIndexSize=(%d|%d) drawType=", i,
			list.GetMaxSize(), list.GetMaxPointCount(), list.GetMaxIndexSize(), list.GetMaxIndexCount() );
		
		drawType = list.GetDrawType();
		
		if( drawType == GL_STATIC_DRAW ){
			text.Append( "GL_STATIC_DRAW" );
			
		}else if( drawType == GL_STREAM_DRAW ){
			text.Append( "GL_STREAM_DRAW" );
			
		}else{
			text.AppendFormat( "%x", drawType );
		}
		
		text.Append( "):\n" );
		answer.AppendFromUTF8( text.GetString() );
		
		// layout
		text.Format( "  - layout (stride=%d index=", layout.GetStride() );
		
		if( layout.GetIndexType() == deoglVBOLayout::eitUnsignedInt ){
			text.Append( "int" );
			
		}else if( layout.GetIndexType() == deoglVBOLayout::eitUnsignedShort ){
			text.Append( "short" );
			
		}else if( layout.GetIndexType() == deoglVBOLayout::eitUnsignedByte ){
			text.Append( "byte" );
			
		}else{
			text.Append( "none" );
		}
		
		text.Append( "):\n" );
		answer.AppendFromUTF8( text.GetString() );
		
		for( j=0; j<attributeCount; j++ ){
			const deoglVBOAttribute &attribute = layout.GetAttributeAt( j );
			
			text.Format( "    - attribute %d (offset=%d componentCount=%d dataType=", j, attribute.GetOffset(), attribute.GetComponentCount() );
				
			if( attribute.GetDataType() == deoglVBOAttribute::edtFloat ){
				text.Append( "float" );
				
			}else if( attribute.GetDataType() == deoglVBOAttribute::edtByte ){
				text.Append( "float byte" );
				
			}else if( attribute.GetDataType() == deoglVBOAttribute::edtIByte ){
				text.Append( "integer byte" );
				
			}else if( attribute.GetDataType() == deoglVBOAttribute::edtUByte ){
				text.Append( "float unsigned byte" );
				
			}else if( attribute.GetDataType() == deoglVBOAttribute::edtIUByte ){
				text.Append( "integer unsigned byte" );
				
			}else if( attribute.GetDataType() == deoglVBOAttribute::edtShort ){
				text.Append( "float short" );
				
			}else if( attribute.GetDataType() == deoglVBOAttribute::edtIShort ){
				text.Append( "integer short" );
				
			}else if( attribute.GetDataType() == deoglVBOAttribute::edtUShort ){
				text.Append( "float unsigned short" );
				
			}else if( attribute.GetDataType() == deoglVBOAttribute::edtIUShort ){
				text.Append( "integer unsigned short" );
				
			}else if( attribute.GetDataType() == deoglVBOAttribute::edtInt ){
				text.Append( "float int" );
				
			}else if( attribute.GetDataType() == deoglVBOAttribute::edtIInt ){
				text.Append( "integer int" );
				
			}else if( attribute.GetDataType() == deoglVBOAttribute::edtUInt ){
				text.Append( "float unsigned int" );
				
			}else{ // attribute.GetDataType() == deoglVBOAttribute::edtIUInt ){
				text.Append( "integer unsigned int" );
			}
			
			text.Append( ")\n" );
			answer.AppendFromUTF8( text.GetString() );
		}
		
		// vbos
		for( j=0; j<vboCount; j++ ){
			const deoglSharedVBO &vbo = *list.GetAt( j );
			const int blockCount = vbo.GetBlockCount();
			
			text.Format( "  - vbo %d (vbo=%u vao=%u ibo=%u usedSize=(%d|%d) indexSize=%d)\n", j,
				vbo.GetVBO(), vbo.GetVAO()->GetVAO(), vbo.GetIBO(), vbo.GetUsedSize(),
				vbo.GetUsedSize() * stride, vbo.GetIndexUsedSize() );
			answer.AppendFromUTF8( text.GetString() );
			
			// blocks
			for( k=0; k<blockCount; k++ ){
				const deoglSharedVBOBlock &block = *vbo.GetBlockAt( k );
				
				text.Format( "    - block %d (offset=(%d|%d) size=(%d|%d) indices=(%d,%d) empty=%d)\n", k, block.GetOffset(),
					block.GetOffset() * stride, block.GetSize(), block.GetSize() * stride, block.GetIndexOffset(),
					block.GetIndexCount(), block.GetEmpty() ? 1 : 0 );
				answer.AppendFromUTF8( text.GetString() );
			}
		}
	}
}

void deoglDeveloperModeStats::TextureUnitsConfigurations( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	const deoglTexUnitsConfigList &tuclist = pRenderThread.GetShader().GetTexUnitsConfigList();
	const deoglTexUnitsConfig *tuc = tuclist.GetRoot();
	bool verbose = false;
	int i, j, unitCount;
	bool csv = false;
	decString text;
	
	text.Format( "%d Texture Units Configurations\n", tuclist.GetCount() );
	answer.AppendFromUTF8( text.GetString() );
	
	for( i=2; i<command.GetArgumentCount(); i++ ){
		if( command.MatchesArgumentAt( i, "verbose" ) ){
			verbose = true;
			
		}else if( command.MatchesArgumentAt( i, "csv" ) ){
			csv = true;
		}
	}
	
	if( csv ){
		answer.AppendFromUTF8( "Index Usage UnitCount Unit01 Unit02 Unit03 Unit04 Unit05 Unit06 Unit07 Unit08 Hash\n" );
	}
	
	i = 0;
	while( tuc ){
		unitCount = tuc->GetUnitCount();
		
		if( verbose ){
			text.Format( "- Configuration %d: usage=%d units=%d spb=%p hash=%u\n", i,
				tuc->GetUsageCount(), unitCount, tuc->GetParameterBlock(), tuc->GetUnitsHashCode() );
			answer.AppendFromUTF8( text.GetString() );
			
			for( j=0; j<unitCount; j++ ){
				const deoglTexUnitConfig &unit = tuc->GetUnitAt( j );
				
				if( unit.GetTexture() ){
					text.Format( "  - Unit %d Texture: gl=%u filtering=%d wrapping=%u depthCompare=%s\n", j,
						unit.GetTexture()->GetTexture(),
						unit.GetSampler() ? unit.GetSampler()->GetFilterMode() : 0,
						unit.GetSampler() ? unit.GetSampler()->GetWrapMode() : 0,
						unit.GetSampler() ? ( unit.GetSampler()->GetDepthCompareMode() ? "y" : "n" ) : "?" );
					
				}else if( unit.GetCubeMap() ){
					text.Format( "  - Unit %d CubeMap: gl=%u filtering=%d depthCompare=%s\n", j,
						unit.GetCubeMap()->GetTexture(),
						unit.GetSampler() ? unit.GetSampler()->GetFilterMode() : 0,
						unit.GetSampler() ? ( unit.GetSampler()->GetDepthCompareMode() ? "y" : "n" ) : "?" );
					
				}else if( unit.GetTBO() ){
					text.Format( "  - Unit %d TBO: gl=%u\n", j, unit.GetTBO() );
					
				}else if( unit.GetSpecial() != deoglTexUnitConfig::estNone ){
					text.Format( "  - Unit %d Special: type=%d filtering=%d wrapping=%u depthCompare=%s\n", j,
						unit.GetSpecial(),
						unit.GetSampler() ? unit.GetSampler()->GetFilterMode() : 0,
						unit.GetSampler() ? unit.GetSampler()->GetWrapMode() : 0,
						unit.GetSampler() ? ( unit.GetSampler()->GetDepthCompareMode() ? "y" : "n" ) : "?" );
					
				}else{
					text.Format( "  - Unit %d Empty\n", j );
				}
				answer.AppendFromUTF8( text.GetString() );
			}
			
		}else if( csv ){
			text.Format( "%d %d %d", i, tuc->GetUsageCount(), unitCount );
			
			for( j=0; j<unitCount; j++ ){
				if( j == 8 ){
					break;
				}
				
				const deoglTexUnitConfig &unit = tuc->GetUnitAt( j );
				
				if( unit.GetTexture() ){
					text.AppendFormat( " T%d", unit.GetTexture()->GetTexture() );
					
				}else if( unit.GetCubeMap() ){
					text.AppendFormat( " C%d", unit.GetCubeMap()->GetTexture() );
					
				}else if( unit.GetTBO() ){
					text.AppendFormat( " B%d", unit.GetTBO() );
					
				}else if( unit.GetSpecial() != deoglTexUnitConfig::estNone ){
					text.AppendFormat( " S%d", unit.GetSpecial() );
					
				}else{
					text.AppendFormat( " -" );
				}
			}
			
			for( j=unitCount; j<8; j++ ){
				text.AppendFormat( " -" );
			}
			
			text.AppendFormat( " %u\n", tuc->GetUnitsHashCode() );
			answer.AppendFromUTF8( text.GetString() );
			
		}else{
			text.Format( "- Configuration %d: usage=%d units=(", i, tuc->GetUsageCount() );
			
			for( j=0; j<unitCount; j++ ){
				const deoglTexUnitConfig &unit = tuc->GetUnitAt( j );
				
				if( j > 0 ){
					text.AppendCharacter( ' ' );
				}
				
				if( unit.GetTexture() ){
					text.AppendFormat( "T%d", unit.GetTexture()->GetTexture() );
					
				}else if( unit.GetCubeMap() ){
					text.AppendFormat( "C%d", unit.GetCubeMap()->GetTexture() );
					
				}else if( unit.GetTBO() ){
					text.AppendFormat( "B%d", unit.GetTBO() );
					
				}else if( unit.GetSpecial() != deoglTexUnitConfig::estNone ){
					text.AppendFormat( "S%d", unit.GetSpecial() );
					
				}else{
					text.AppendCharacter( '-' );
				}
			}
			
			text.AppendFormat( ") hash=%u\n", tuc->GetUnitsHashCode() );
			answer.AppendFromUTF8( text.GetString() );
		}
		
		tuc = tuc->GetLLNext();
		i++;
	}
}
