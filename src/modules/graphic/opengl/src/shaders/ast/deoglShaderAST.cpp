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

#include "deoglShaderAST.h"
#include "deoglShaderASTHelper.h"
#include "deoglSASTDeclaration.h"
#include "node/deoglSASTNode.h"
#include "node/deoglSASTNodeConstant.h"
#include "node/deoglSASTNodeFunctionCall.h"
#include "node/deoglSASTNodeOperator.h"
#include "node/deoglSASTNodeBlock.h"
#include "node/deoglSASTNodeReturn.h"
#include "node/deoglSASTNodeVariable.h"
#include "node/deoglSASTNodeIfElse.h"
#include "node/deoglSASTNodeDiscard.h"
#include "deoglSASTGenerateCode.h"
#include "deoglSASTFunction.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/logger/deLogger.h>



// Data Type Strings
//////////////////////

static const char *vDataTypeStrings[ deoglShaderAST::EDT_COUNT ] = {
	"void", // edtVoid
	"float", // edtFloat
	"int", // edtInt
	"bool", // edtBool
	"vec2", // edtVec2
	"vec3", // edtVec3
	"vec4", // edtVec4
	"ivec2", // edtIVec2
	"ivec3", // edtIVec3
	"ivec4", // edtIVec4
	"bvec2", // edtBVec2
	"bvec3", // edtBVec3
	"bvec4", // edtBVec4
	"mat3x2", // edtMat3x2
	"mat3", // edtMat3x3
	"mat4x3", // edtMat4x3
	"mat4", // edtMat4x4
	"sampler2D", // edtSampler2D
	"sampler2DShadow", // edtSampler2DShadow
	"sampler2DArray", // edtSampler2DArray
	"sampler2DArrayShadow", // edtSampler2DArrayShadow
	"samplerCube", // edtSamplerCube
	NULL, // edtCustom
};

static const char *vSwizzleStrings[ deoglShaderAST::ES_COUNT ] = {
	"", // esNone
	"x", // esX
	"y", // esY
	"z", // esZ
	"w", // esW
	"r", // esR
	"g", // esG
	"b", // esB
	"a" // esA
};



// Class deoglShaderAST
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderAST::deoglShaderAST( deoglRenderThread &renderThread ) :
pHighPrecision( true ){
}

deoglShaderAST::~deoglShaderAST(){
	pCleanUp();
}



// Management
///////////////

void deoglShaderAST::SetHighPrecision( bool highPrecision ){
	pHighPrecision = highPrecision;
}



void deoglShaderAST::AddUniform( const char *name, int dataType ){
	deoglSASTDeclaration *declaration = NULL;
	
	try{
		declaration = new deoglSASTDeclaration( name, dataType );
		pUniformList.Add( declaration );
		declaration->FreeReference();
		
	}catch( const deException & ){
		if( declaration ){
			declaration->FreeReference();
		}
		throw;
	}
}

void deoglShaderAST::AddSampler( const char *name, int dataType ){
	deoglSASTDeclaration *declaration = NULL;
	
	try{
		declaration = new deoglSASTDeclaration( name, dataType );
		pSamplerList.Add( declaration );
		declaration->FreeReference();
		
	}catch( const deException & ){
		if( declaration ){
			declaration->FreeReference();
		}
		throw;
	}
}

void deoglShaderAST::AddInput( const char *name, int dataType ){
	deoglSASTDeclaration *declaration = NULL;
	
	try{
		declaration = new deoglSASTDeclaration( name, dataType );
		pInputList.Add( declaration );
		declaration->FreeReference();
		
	}catch( const deException & ){
		if( declaration ){
			declaration->FreeReference();
		}
		throw;
	}
}

void deoglShaderAST::AddOutput( const char *name, int dataType ){
	deoglSASTDeclaration *declaration = NULL;
	
	try{
		declaration = new deoglSASTDeclaration( name, dataType );
		pOutputList.Add( declaration );
		declaration->FreeReference();
		
	}catch( const deException & ){
		if( declaration ){
			declaration->FreeReference();
		}
		throw;
	}
}

void deoglShaderAST::AddConstantFloat( const char *name, float initValue ){
	deoglSASTDeclaration *declaration = NULL;
	deoglSASTNodeConstant *nodeConst = NULL;
	
	try{
		nodeConst = new deoglSASTNodeConstant( initValue );
		declaration = new deoglSASTDeclaration( name, deoglShaderAST::edtFloat, nodeConst );
		NODE_FREE( nodeConst );
		
		pConstantList.Add( declaration );
		declaration->FreeReference();
		
	}catch( const deException & ){
		NODE_CATCH_CLEANUP( nodeConst );
		if( declaration ){
			declaration->FreeReference();
		}
		throw;
	}
}

void deoglShaderAST::AddConstantVec2( const char *name, float initValueX, float initValueY ){
	deoglSASTNodeFunctionCall *nodeFuncCall = NULL;
	deoglSASTDeclaration *declaration = NULL;
	deoglSASTNodeConstant *nodeConst2 = NULL;
	deoglSASTNodeConstant *nodeConst = NULL;
	
	try{
		nodeConst = new deoglSASTNodeConstant( initValueX );
		nodeConst2 = new deoglSASTNodeConstant( initValueY );
		nodeFuncCall = NEW_NODE_FUNCCALL2( "vec2", nodeConst, nodeConst2 );
		
		declaration = new deoglSASTDeclaration( name, deoglShaderAST::edtVec2, nodeFuncCall );
		NODE_FREE( nodeFuncCall );
		pConstantList.Add( declaration );
		declaration->FreeReference();
		
	}catch( const deException & ){
		NODE_CATCH_CLEANUP( nodeConst );
		NODE_CATCH_CLEANUP( nodeConst2 );
		NODE_CATCH_CLEANUP( nodeFuncCall );
		if( declaration ){
			declaration->FreeReference();
		}
		throw;
	}
}

void deoglShaderAST::AddConstantVec2( const char *name, float initValue ){
	deoglSASTNodeFunctionCall *nodeFuncCall = NULL;
	deoglSASTDeclaration *declaration = NULL;
	deoglSASTNodeConstant *nodeConst = NULL;
	
	try{
		nodeConst = new deoglSASTNodeConstant( initValue );
		nodeFuncCall = NEW_NODE_FUNCCALL( "vec2", nodeConst );
		
		declaration = new deoglSASTDeclaration( name, deoglShaderAST::edtVec2, nodeFuncCall );
		NODE_FREE( nodeFuncCall );
		pConstantList.Add( declaration );
		declaration->FreeReference();
		
	}catch( const deException & ){
		NODE_CATCH_CLEANUP( nodeConst );
		NODE_CATCH_CLEANUP( nodeFuncCall );
		if( declaration ){
			declaration->FreeReference();
		}
		throw;
	}
}

void deoglShaderAST::AddConstantVec3( const char *name, float initValueX, float initValueY, float initValueZ ){
	deoglSASTNodeFunctionCall *nodeFuncCall = NULL;
	deoglSASTDeclaration *declaration = NULL;
	deoglSASTNodeConstant *nodeConst3 = NULL;
	deoglSASTNodeConstant *nodeConst2 = NULL;
	deoglSASTNodeConstant *nodeConst = NULL;
	
	try{
		nodeConst = new deoglSASTNodeConstant( initValueX );
		nodeConst2 = new deoglSASTNodeConstant( initValueY );
		nodeConst3 = new deoglSASTNodeConstant( initValueZ );
		nodeFuncCall = NEW_NODE_FUNCCALL3( "vec3", nodeConst, nodeConst2, nodeConst3 );
		
		declaration = new deoglSASTDeclaration( name, deoglShaderAST::edtVec3, nodeFuncCall );
		NODE_FREE( nodeFuncCall );
		pConstantList.Add( declaration );
		declaration->FreeReference();
		
	}catch( const deException & ){
		NODE_CATCH_CLEANUP( nodeConst );
		NODE_CATCH_CLEANUP( nodeConst2 );
		NODE_CATCH_CLEANUP( nodeConst3 );
		NODE_CATCH_CLEANUP( nodeFuncCall );
		if( declaration ){
			declaration->FreeReference();
		}
		throw;
	}
}

void deoglShaderAST::AddConstantVec3( const char *name, float initValue ){
	deoglSASTNodeFunctionCall *nodeFuncCall = NULL;
	deoglSASTDeclaration *declaration = NULL;
	deoglSASTNodeConstant *nodeConst = NULL;
	
	try{
		nodeConst = new deoglSASTNodeConstant( initValue );
		nodeFuncCall = NEW_NODE_FUNCCALL( "vec3", nodeConst );
		
		declaration = new deoglSASTDeclaration( name, deoglShaderAST::edtVec3, nodeFuncCall );
		NODE_FREE( nodeFuncCall );
		pConstantList.Add( declaration );
		declaration->FreeReference();
		
	}catch( const deException & ){
		NODE_CATCH_CLEANUP( nodeConst );
		NODE_CATCH_CLEANUP( nodeFuncCall );
		if( declaration ){
			declaration->FreeReference();
		}
		throw;
	}
}

void deoglShaderAST::AddConstantVec4( const char *name, float initValueX, float initValueY, float initValueZ, float initValueW ){
	deoglSASTNodeFunctionCall *nodeFuncCall = NULL;
	deoglSASTDeclaration *declaration = NULL;
	deoglSASTNodeConstant *nodeConst4 = NULL;
	deoglSASTNodeConstant *nodeConst3 = NULL;
	deoglSASTNodeConstant *nodeConst2 = NULL;
	deoglSASTNodeConstant *nodeConst = NULL;
	
	try{
		nodeConst = new deoglSASTNodeConstant( initValueX );
		nodeConst2 = new deoglSASTNodeConstant( initValueY );
		nodeConst3 = new deoglSASTNodeConstant( initValueZ );
		nodeConst4 = new deoglSASTNodeConstant( initValueZ );
		nodeFuncCall = NEW_NODE_FUNCCALL4( "vec4", nodeConst, nodeConst2, nodeConst3, nodeConst4 );
		
		declaration = new deoglSASTDeclaration( name, deoglShaderAST::edtVec4, nodeFuncCall );
		NODE_FREE( nodeFuncCall );
		pConstantList.Add( declaration );
		declaration->FreeReference();
		
	}catch( const deException & ){
		NODE_CATCH_CLEANUP( nodeConst );
		NODE_CATCH_CLEANUP( nodeConst2 );
		NODE_CATCH_CLEANUP( nodeConst3 );
		NODE_CATCH_CLEANUP( nodeConst4 );
		NODE_CATCH_CLEANUP( nodeFuncCall );
		if( declaration ){
			declaration->FreeReference();
		}
		throw;
	}
}

void deoglShaderAST::AddConstantVec4( const char *name, float initValue ){
	deoglSASTNodeFunctionCall *nodeFuncCall = NULL;
	deoglSASTDeclaration *declaration = NULL;
	deoglSASTNodeConstant *nodeConst = NULL;
	
	try{
		nodeConst = new deoglSASTNodeConstant( initValue );
		nodeFuncCall = NEW_NODE_FUNCCALL( "vec4", nodeConst );
		
		declaration = new deoglSASTDeclaration( name, deoglShaderAST::edtVec4, nodeFuncCall );
		NODE_FREE( nodeFuncCall );
		pConstantList.Add( declaration );
		declaration->FreeReference();
		
	}catch( const deException & ){
		NODE_CATCH_CLEANUP( nodeConst );
		NODE_CATCH_CLEANUP( nodeFuncCall );
		if( declaration ){
			declaration->FreeReference();
		}
		throw;
	}
}



const char *deoglShaderAST::GetDataTypeString( int dataType ){
	if( dataType < 0 || dataType >= deoglShaderAST::EDT_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	return vDataTypeStrings[ dataType ];
}

const char *deoglShaderAST::GetSwizzleString( int swizzle ){
	if( swizzle < 0 || swizzle >= deoglShaderAST::ES_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	return vSwizzleStrings[ swizzle ];
}



bool deoglShaderAST::Verify() const{
	return true;
}

void deoglShaderAST::Optimize(){
}

void deoglShaderAST::GenerateSourceCode( decString &sourceCode ){
	deoglSASTGenerateCode gencode( this, &sourceCode );
	
	sourceCode.Empty();
	GenerateHeaderSourceCode( gencode );
	GenerateDeclarationSourceCode( gencode, pUniformList, "uniform" );
	GenerateDeclarationSourceCode( gencode, pSamplerList, "uniform" );
	GenerateDeclarationSourceCode( gencode, pInputList, "in" );
	GenerateDeclarationSourceCode( gencode, pOutputList, "out" );
	GenerateDeclarationSourceCode( gencode, pConstantList, "const" );
	GenerateFunctionSourceCode( gencode, pFunctionList );
}

void deoglShaderAST::GenerateHeaderSourceCode( deoglSASTGenerateCode &gencode ){
	decString &sourceCode = gencode.GetSourceCode();
	
	sourceCode.Append( "#version 130\n" );
	
	if( pHighPrecision ){
		sourceCode.AppendCharacter( '\n' );
		sourceCode.Append( "precision highp float;\n" );
		sourceCode.Append( "precision highp int;\n" );
	}
}

void deoglShaderAST::GenerateDeclarationSourceCode( deoglSASTGenerateCode &gencode, const deoglSASTDeclarationList &list, const char *prefix ){
	decString &sourceCode = gencode.GetSourceCode();
	const int count = list.GetCount();
	int i;
	
	if( count > 0 ){
		sourceCode.AppendCharacter( '\n' );
	}
	
	for( i=0; i<count; i++ ){
		const deoglSASTDeclaration &declaration = *list.GetAt( i );
		
		sourceCode.Append( prefix );
		sourceCode.AppendCharacter( ' ' );
		sourceCode.Append( deoglShaderAST::GetDataTypeString( declaration.GetDataType() ) );
		sourceCode.AppendCharacter( ' ' );
		sourceCode.Append( declaration.GetName().GetString() );
		
		if( declaration.GetNodeInit() ){
			sourceCode.Append( " = " );
			gencode.SetRequiresParanthesis( false );
			declaration.GetNodeInit()->GenerateSourceCode( gencode );
		}
		
		sourceCode.Append( ";\n" );
	}
}

void deoglShaderAST::GenerateFunctionSourceCode( deoglSASTGenerateCode &gencode, const deoglSASTFunctionList &list ){
	const int count = list.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		GenerateFunctionSourceCode( gencode, *list.GetAt( i ) );
	}
}

void deoglShaderAST::GenerateFunctionSourceCode( deoglSASTGenerateCode &gencode, deoglSASTFunction &function ){
	const deoglSASTDeclarationList &paramList = function.GetParameterList();
	decString &sourceCode = gencode.GetSourceCode();
	const int paramCount = paramList.GetCount();
	int i;
	
	sourceCode.AppendCharacter( '\n' );
	sourceCode.Append( deoglShaderAST::GetDataTypeString( function.GetDataType() ) );
	sourceCode.AppendCharacter( ' ' );
	sourceCode.Append( function.GetName() );
	
	if( paramCount == 0 ){
		sourceCode.Append( "( void )" );
		
	}else{
		sourceCode.Append( "( " );
		gencode.IncrementIndentTabs();
		
		for( i=0; i<paramCount; i++ ){
			const deoglSASTDeclaration &declaration = *paramList.GetAt( i );
			
			if( i > 0 ){
				sourceCode.Append( ", " );
			}
			
			if( declaration.GetDirectionQualifier() == deoglSASTDeclaration::edqIn ){
				sourceCode.Append( "in " );
				
			}else if( declaration.GetDirectionQualifier() == deoglSASTDeclaration::edqOut ){
				sourceCode.Append( "out " );
				
			}else if( declaration.GetDirectionQualifier() == deoglSASTDeclaration::edqInOut ){
				sourceCode.Append( "inout " );
			}
			
			sourceCode.Append( deoglShaderAST::GetDataTypeString( declaration.GetDataType() ) );
			sourceCode.AppendCharacter( ' ' );
			sourceCode.Append( declaration.GetName().GetString() );
		}
		
		gencode.DecrementIndentTabs();
		sourceCode.Append( " )" );
	}
	
	gencode.SetRequiresParanthesis( false );
	function.GetNodeBody()->GenerateSourceCode( gencode );
	sourceCode.Append( "\n" );
}



// Debugging
//////////////

void deoglShaderAST::DebugTestAST( deoglRenderThread &renderThread ){
	deoglShaderAST *ast = deoglShaderAST::DebugCreateTestAST( renderThread );
	
	if( ast->Verify() ){
		decString sourceCode;
		
		renderThread.GetLogger().LogInfo( "DebugTestAST: Verification succeeded" );
		
		ast->Optimize();
		ast->GenerateSourceCode( sourceCode );
		renderThread.GetLogger().LogInfoFormat( "DebugTestAST: Generated Source Code:\n%s", sourceCode.GetString() );
		
	}else{
		renderThread.GetLogger().LogError( "DebugTestAST: Verification failed" );
	}
	
	delete ast;
}

deoglShaderAST *deoglShaderAST::DebugCreateTestAST( deoglRenderThread &renderThread ){
	deoglShaderAST *ast = new deoglShaderAST( renderThread );
	deoglSASTDeclaration *declaration = NULL;
	deoglSASTNodeConstant *nodeConst = NULL;
	deoglSASTNodeConstant *nodeConst2 = NULL;
	deoglSASTNodeFunctionCall *nodeFuncCall = NULL;
	deoglSASTNodeOperator *nodeOperator = NULL;
	deoglSASTNodeOperator *nodeOperator2 = NULL;
	deoglSASTNodeDiscard *nodeDiscard = NULL;
	deoglSASTNodeBlock *ifelseNodeBody = NULL;
	deoglSASTNodeBlock *funcNodeBody = NULL;
	deoglSASTNodeIfElse *nodeIfElse = NULL;
	deoglSASTNodeReturn *nodeReturn = NULL;
	deoglSASTNodeVariable *nodeVar2 = NULL;
	deoglSASTNodeVariable *nodeVar = NULL;
	deoglSASTFunction *function = NULL;
	
	ast->SetHighPrecision( true );
	
	// uniforms
	declaration = new deoglSASTDeclaration( "pParam1", edtVec4 );
	ast->GetUniformList().Add( declaration );
	declaration->FreeReference();
	
	declaration = new deoglSASTDeclaration( "pParam2", edtVec2 );
	ast->GetUniformList().Add( declaration );
	declaration->FreeReference();
	
	declaration = new deoglSASTDeclaration( "pParam3", edtMat4x3 );
	ast->GetUniformList().Add( declaration );
	declaration->FreeReference();
	
	// samplers
	declaration = new deoglSASTDeclaration( "texColor", edtSampler2D );
	ast->GetSamplerList().Add( declaration );
	declaration->FreeReference();
	
	declaration = new deoglSASTDeclaration( "texShadow", edtSampler2DShadow );
	ast->GetSamplerList().Add( declaration );
	declaration->FreeReference();
	
	declaration = new deoglSASTDeclaration( "texDepth", edtSampler2D );
	ast->GetSamplerList().Add( declaration );
	declaration->FreeReference();
	
	// inputs
	declaration = new deoglSASTDeclaration( "vTexCoord", edtVec2 );
	ast->GetInputList().Add( declaration );
	declaration->FreeReference();
	
	declaration = new deoglSASTDeclaration( "vColor", edtVec3 );
	ast->GetInputList().Add( declaration );
	declaration->FreeReference();
	
	declaration = new deoglSASTDeclaration( "vInterpolated", edtFloat );
	ast->GetInputList().Add( declaration );
	declaration->FreeReference();
	
	// outputs
	declaration = new deoglSASTDeclaration( "outColor", edtVec4 );
	ast->GetOutputList().Add( declaration );
	declaration->FreeReference();
	
	declaration = new deoglSASTDeclaration( "outNormal", edtVec4 );
	ast->GetOutputList().Add( declaration );
	declaration->FreeReference();
	
	declaration = new deoglSASTDeclaration( "outParams", edtVec4 );
	ast->GetOutputList().Add( declaration );
	declaration->FreeReference();
	
	// constants
	declaration = new deoglSASTDeclaration( "epsilon", edtFloat );
	nodeConst = new deoglSASTNodeConstant( 0.0001f );
	declaration->SetNodeInit( nodeConst );
	nodeConst->FreeReference();
	ast->GetConstantList().Add( declaration );
	declaration->FreeReference();
	
	declaration = new deoglSASTDeclaration( "packShift", edtVec3 );
	nodeFuncCall = new deoglSASTNodeFunctionCall( "vec3" );
		nodeConst = new deoglSASTNodeConstant( 1.0f );
		nodeFuncCall->GetNodeParamsList().Add( nodeConst );
		nodeConst->FreeReference();
		
		nodeConst = new deoglSASTNodeConstant( 256.0f );
		nodeFuncCall->GetNodeParamsList().Add( nodeConst );
		nodeConst->FreeReference();
		
		nodeConst = new deoglSASTNodeConstant( 65536.0f );
		nodeFuncCall->GetNodeParamsList().Add( nodeConst );
		nodeConst->FreeReference();
	declaration->SetNodeInit( nodeFuncCall );
	nodeFuncCall->FreeReference();
	ast->GetConstantList().Add( declaration );
	declaration->FreeReference();
	
	declaration = new deoglSASTDeclaration( "unpackDepth", edtVec3 );
	nodeFuncCall = new deoglSASTNodeFunctionCall( "vec3" );
		nodeConst = new deoglSASTNodeConstant( 1.0f );
		nodeFuncCall->GetNodeParamsList().Add( nodeConst );
		nodeConst->FreeReference();
		
		nodeConst = new deoglSASTNodeConstant( 1.0f );
		nodeConst2 = new deoglSASTNodeConstant( 256.0f );
		nodeOperator = new deoglSASTNodeOperator( deoglSASTNodeOperator::eoDivide, nodeConst, nodeConst2 );
		nodeConst->FreeReference();
		nodeConst2->FreeReference();
		nodeFuncCall->GetNodeParamsList().Add( nodeOperator );
		nodeOperator->FreeReference();
		
		nodeConst = new deoglSASTNodeConstant( 1.0f );
		nodeConst2 = new deoglSASTNodeConstant( 65536.0f );
		nodeOperator = new deoglSASTNodeOperator( deoglSASTNodeOperator::eoDivide, nodeConst, nodeConst2 );
		nodeConst->FreeReference();
		nodeConst2->FreeReference();
		nodeFuncCall->GetNodeParamsList().Add( nodeOperator );
		nodeOperator->FreeReference();
	declaration->SetNodeInit( nodeFuncCall );
	nodeFuncCall->FreeReference();
	ast->GetConstantList().Add( declaration );
	declaration->FreeReference();
	
	// test helper function
	function = new deoglSASTFunction( "helperFunction" );
	function->SetDataType( edtFloat );
	
	declaration = new deoglSASTDeclaration( "valIn", edtVec3, deoglSASTDeclaration::edqIn );
	function->GetParameterList().Add( declaration );
	declaration->FreeReference();
	declaration = new deoglSASTDeclaration( "valInOut", edtVec2, deoglSASTDeclaration::edqInOut );
	function->GetParameterList().Add( declaration );
	declaration->FreeReference();
	declaration = new deoglSASTDeclaration( "valOut", edtVec4, deoglSASTDeclaration::edqOut );
	function->GetParameterList().Add( declaration );
	declaration->FreeReference();
	
	funcNodeBody = function->GetNodeBody();
	
	nodeVar = new deoglSASTNodeVariable( "valInOut", esY, esX );
	nodeVar2 = new deoglSASTNodeVariable( "valIn", esZ, esX );
	nodeOperator = new deoglSASTNodeOperator( deoglSASTNodeOperator::eoAssignMultiply, nodeVar, nodeVar2 );
	nodeVar->FreeReference();
	nodeVar2->FreeReference();
	funcNodeBody->GetNodeList().Add( nodeOperator );
	nodeOperator->FreeReference();
	
	nodeVar = new deoglSASTNodeVariable( "valOut" );
	nodeFuncCall = new deoglSASTNodeFunctionCall( "vec4" );
		nodeVar2 = new deoglSASTNodeVariable( "valIn", esX, esX, esY );
		nodeFuncCall->GetNodeParamsList().Add( nodeVar2 );
		nodeVar2->FreeReference();
		
		nodeVar2 = new deoglSASTNodeVariable( "valInOut", esY );
		nodeConst = new deoglSASTNodeConstant( 2.0f );
		nodeOperator = new deoglSASTNodeOperator( deoglSASTNodeOperator::eoAdd, nodeVar2, nodeConst );
		nodeVar2->FreeReference();
		nodeConst->FreeReference();
		nodeFuncCall->GetNodeParamsList().Add( nodeOperator );
		nodeOperator->FreeReference();
	nodeOperator = new deoglSASTNodeOperator( deoglSASTNodeOperator::eoAssign, nodeVar, nodeFuncCall );
	nodeVar->FreeReference();
	nodeFuncCall->FreeReference();
	funcNodeBody->GetNodeList().Add( nodeOperator );
	nodeOperator->FreeReference();
	
	nodeVar = new deoglSASTNodeVariable( "valIn", esY );
	nodeReturn = new deoglSASTNodeReturn( nodeVar );
	nodeVar->FreeReference();
	funcNodeBody->GetNodeList().Add( nodeReturn );
	nodeReturn->FreeReference();
	
	ast->GetFunctionList().Add( function );
	function->FreeReference();
	
	// main function
	function = new deoglSASTFunction( "main" );
	funcNodeBody = function->GetNodeBody();
	
	nodeVar = new deoglSASTNodeVariable( "gl_ID" );
	nodeConst = new deoglSASTNodeConstant( 1 );
	nodeOperator = new deoglSASTNodeOperator( deoglSASTNodeOperator::eoAdd, nodeVar, nodeConst );
	nodeVar->FreeReference();
	nodeConst->FreeReference();
	nodeVar = new deoglSASTNodeVariable( "vColor", esZ );
	nodeVar->GetNodeArrayList().Add( nodeOperator );
	nodeOperator->FreeReference();
	nodeConst = new deoglSASTNodeConstant( 0.5f );
	nodeOperator2 = new deoglSASTNodeOperator( deoglSASTNodeOperator::eoGreaterThan, nodeOperator, nodeConst );
	nodeConst->FreeReference();
	nodeOperator->FreeReference();
	
	nodeIfElse = new deoglSASTNodeIfElse( nodeOperator2 );
	nodeOperator2->FreeReference();
	nodeDiscard = new deoglSASTNodeDiscard;
	nodeIfElse->GetNodeIf()->GetNodeList().Add( nodeDiscard );
	nodeDiscard->FreeReference();
	funcNodeBody->GetNodeList().Add( nodeIfElse );
	nodeIfElse->FreeReference();
	
	nodeConst = new deoglSASTNodeConstant( true );
	nodeIfElse = new deoglSASTNodeIfElse( nodeConst );
	nodeConst->FreeReference();
		ifelseNodeBody = nodeIfElse->GetNodeIf();
		
		nodeVar = new deoglSASTNodeVariable( "outColor" );
		nodeVar2 = new deoglSASTNodeVariable( "vColor" );
		nodeOperator = new deoglSASTNodeOperator( deoglSASTNodeOperator::eoAssign, nodeVar, nodeVar2 );
		nodeVar->FreeReference();
		nodeVar2->FreeReference();
		ifelseNodeBody->GetNodeList().Add( nodeOperator );
		nodeOperator->FreeReference();
		
		nodeReturn = new deoglSASTNodeReturn;
		ifelseNodeBody->GetNodeList().Add( nodeReturn );
		nodeReturn->FreeReference();
		
		ifelseNodeBody = nodeIfElse->GetNodeElse();
		nodeDiscard = new deoglSASTNodeDiscard;
		ifelseNodeBody->GetNodeList().Add( nodeDiscard );
		nodeDiscard->FreeReference();
	funcNodeBody->GetNodeList().Add( nodeIfElse );
	nodeIfElse->FreeReference();
	
	ast->GetFunctionList().Add( function );
	function->FreeReference();
	
	return ast;
}



// Private functions
//////////////////////

void deoglShaderAST::pCleanUp(){
	pFunctionList.RemoveAll();
	pOutputList.RemoveAll();
	pInputList.RemoveAll();
	pSamplerList.RemoveAll();
	pUniformList.RemoveAll();
	pConstantList.RemoveAll();
}
