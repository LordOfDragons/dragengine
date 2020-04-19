#include "DebugAst.h"
#include "dsp_tokenstream.h"
#include "dsp_tokentype.h"

#include <QtCore/QDebug>

namespace DragonScript{

DebugAst::DebugAst( const TokenStream &tokenStream, const QByteArray &contents ) :
pTokenStream( tokenStream ),
pContents( contents ){
}

void DebugAst::visitNamespace( NamespaceAst *node ){
	pBuffer = pPrefix;
	pBuffer.append( "namespace " );
	visitNode( node->name );
	qDebug() << pBuffer;
}

void DebugAst::visitPin( PinAst *node ){
	pBuffer = pPrefix;
	pBuffer.append( "pin " );
	visitNode( node->name );
	qDebug() << pBuffer;
}

void DebugAst::visitRequires( RequiresAst *node ){
	pBuffer = pPrefix;
	pBuffer.append( "requires " ).append( tokenText( node->name ) );
	qDebug() << pBuffer;
}

void DebugAst::visitScriptDeclaration( ScriptDeclarationAst *node ){
	pBuffer = pPrefix;
	DefaultVisitor::visitScriptDeclaration( node );
}



void DebugAst::visitClass( ClassAst *node ){
	const QString oldPrefix( pPrefix );
	pBuffer.append( "class " );
	
	pPrefix.append( "  " );
	DefaultVisitor::visitClass( node );
	pPrefix = oldPrefix;
	
	pBuffer = pPrefix;
	pBuffer.append( "end class " ).append( tokenText( *node->begin->name ) );
	qDebug() << pBuffer;
}

void DebugAst::visitClassBegin( ClassBeginAst *node ){
	pBuffer.append( tokenText( *node->name ) );
	
	if( node->extends ){
		pBuffer.append( " extends " );
		visitNode( node->extends );
	}
	
	if( node->implementsSequence ){
		pBuffer.append( " implements " );
		const KDevPG::ListNode<FullyQualifiedClassnameAst*> *iter = node->implementsSequence->front();
		const KDevPG::ListNode<FullyQualifiedClassnameAst*> *end = iter;
		do{
			if( iter != end ){
				pBuffer.append( ", " );
			}
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
	qDebug() << pBuffer;
}

void DebugAst::visitClassBodyDeclaration( ClassBodyDeclarationAst *node ){
	pBuffer = pPrefix;
	DefaultVisitor::visitClassBodyDeclaration( node );
}

void DebugAst::visitClassBody( ClassBodyAst *node ){
	pBuffer = pPrefix;
	DefaultVisitor::visitClassBody( node );
}

void DebugAst::visitClassEnd( ClassEndAst *node ){
	Q_UNUSED(node)
}



void DebugAst::visitInterface( InterfaceAst *node ){
	const QString oldPrefix( pPrefix );
	pBuffer.append( "interface " );
	
	pPrefix.append( "  " );
	DefaultVisitor::visitInterface( node );
	pPrefix = oldPrefix;
	
	pBuffer = pPrefix;
	pBuffer.append( "end interface " ).append( tokenText( *node->begin->name ) );
	qDebug() << pBuffer;
}

void DebugAst::visitInterfaceBegin( InterfaceBeginAst *node ){
	pBuffer.append( tokenText( *node->name ) );
	
	if( node->implementsSequence ){
		pBuffer.append( " implements " );
		const KDevPG::ListNode<FullyQualifiedClassnameAst*> *iter = node->implementsSequence->front();
		const KDevPG::ListNode<FullyQualifiedClassnameAst*> *end = iter;
		do{
			if( iter != end ){
				pBuffer.append( ", " );
			}
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
	qDebug() << pBuffer;
}

void DebugAst::visitInterfaceBodyDeclaration( InterfaceBodyDeclarationAst *node ){
	pBuffer = pPrefix;
	DefaultVisitor::visitInterfaceBodyDeclaration( node );
}

void DebugAst::visitInterfaceBody( InterfaceBodyAst *node ){
	pBuffer = pPrefix;
	DefaultVisitor::visitInterfaceBody( node );
}

void DebugAst::visitInterfaceEnd( InterfaceEndAst *node ){
	Q_UNUSED(node)
}



void DebugAst::visitClassFunctionDeclare( ClassFunctionDeclareAst *node ){
	const QString oldPrefix( pPrefix );
	pBuffer.append( "function " );
	
	pPrefix.append( "  " );
	DefaultVisitor::visitClassFunctionDeclare( node );
	pPrefix = oldPrefix;
	
	pBuffer = pPrefix;
	pBuffer.append( "end function " );
	if( node->begin->op ){
		pBuffer.append( tokenText( *node->begin->op ) );
	}else{
		pBuffer.append( tokenText( *node->begin->name ) );
	}
	qDebug() << pBuffer;
}

void DebugAst::visitClassFunctionDeclareBegin( ClassFunctionDeclareBeginAst *node ){
	QString name;
	if( node->name ){
		name = tokenText( *node->name );
	}else if( node->op ){
		name = tokenText( *node->op );
	}
	
	if( name == "new" ){
		pBuffer.append( "constructor " );
	}else if( name == "destructor" ){
		pBuffer.append( "destructor " );
	}
	
	visitNode( node->type );
	
	pBuffer.append( ' ' );
	pBuffer.append( name );
	pBuffer.append( '(' );
	if( node->argumentsSequence ){
		const KDevPG::ListNode<ClassFunctionDeclareArgumentAst*> *iter = node->argumentsSequence->front();
		const KDevPG::ListNode<ClassFunctionDeclareArgumentAst*> *end = iter;
		do{
			if( iter != end ){
				pBuffer.append( ", " );
			}
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
	pBuffer.append( ')' );
	
	int kindSuper = 0;
	if( node->super != -1 ){
		kindSuper = pTokenStream.at( node->super ).kind;
		if( kindSuper == TokenType::Token_SUPER ){
			pBuffer.append( " super(");
			
		}else if( kindSuper == TokenType::Token_THIS ){
			pBuffer.append( " this(");
		}
	}
	
	if( kindSuper == TokenType::Token_SUPER || kindSuper == TokenType::Token_THIS ){
		if( node->superArgumentsSequence ){
			const KDevPG::ListNode<ExpressionAst*> *iter = node->superArgumentsSequence->front();
			const KDevPG::ListNode<ExpressionAst*> *end = iter;
			do{
				if( iter != end ){
					pBuffer.append( ", " );
				}
				visitNode( iter->element );
				iter = iter->next;
			}while( iter != end );
		}
		pBuffer.append( ')' );
	}
	
	qDebug() << pBuffer;
}

void DebugAst::visitClassFunctionDeclareArgument( ClassFunctionDeclareArgumentAst *node ){
	visitNode( node->type );
	pBuffer.append( ' ' ).append( tokenText( *node->name ) );
}

void DebugAst::visitClassFunctionEnd( ClassFunctionEndAst *node ){
	Q_UNUSED(node)
}



void DebugAst::visitClassVariablesDeclare( ClassVariablesDeclareAst *node ){
	pBuffer.append( "variable " );
	visitNode( node->type );
	pBuffer.append( ' ' );
	
	if( node->variablesSequence ){
		const KDevPG::ListNode<ClassVariableDeclareAst*> *iter = node->variablesSequence->front();
		const KDevPG::ListNode<ClassVariableDeclareAst*> *end = iter;
		do{
			if( iter != end ){
				pBuffer.append( ", " );
			}
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
	
	qDebug() << pBuffer;
}

void DebugAst::visitClassVariableDeclare( ClassVariableDeclareAst *node ){
	pBuffer.append( tokenText( *node->name ) );
	if( node->value ){
		pBuffer.append( " = " );
		visitNode( node->value );
	}
}



void DebugAst::visitEnumeration( EnumerationAst *node ){
	const QString oldPrefix( pPrefix );
	pBuffer.append( "enumeration " );
	
	pPrefix.append( "  " );
	DefaultVisitor::visitEnumeration( node );
	pPrefix = oldPrefix;
	
	pBuffer = pPrefix;
	pBuffer.append( "end enumeration " ).append( tokenText( *node->begin->name ) );
	qDebug() << pBuffer;
}

void DebugAst::visitEnumerationBegin( EnumerationBeginAst *node ){
	pBuffer.append( tokenText( *node->name ) );
	qDebug() << pBuffer;
}

void DebugAst::visitEnumerationBody( EnumerationBodyAst *node ){
	pBuffer = pPrefix;
	pBuffer.append( tokenText( *node->name ) );
	if( node->value ){
		pBuffer.append( " = " );
		visitNode( node->value );
	}
	qDebug() << pBuffer;
}

void DebugAst::visitEnumerationEnd( EnumerationEndAst *node ){
	Q_UNUSED(node)
}



void DebugAst::visitStatement( StatementAst *node ){
	pBuffer = pPrefix;
	
	if( node->sbreak != -1 ){
		pBuffer.append( "break" );
		
	}else if( node->scontinue != -1 ){
		pBuffer.append( "continue" );
		
	}else{
		DefaultVisitor::visitStatement( node );
	}
	
	if( pBuffer != pPrefix ){
		qDebug() << pBuffer;
	}
}

void DebugAst::visitStatementIf( StatementIfAst *node ){
	pBuffer.append( "if " );
	visitNode( node->condition );
	qDebug() << pBuffer;
	
	if( node->bodySequence ){
		const QString oldPrefix( pPrefix );
		pPrefix.append( "  " );
		
		const KDevPG::ListNode<StatementAst*> *iter = node->bodySequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
		
		pPrefix = oldPrefix;
	}
	
	if( node->elifSequence ){
		const KDevPG::ListNode<StatementElifAst*> *iter = node->elifSequence->front();
		const KDevPG::ListNode<StatementElifAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
	
	if( node->elseSequence ){
		pBuffer = pPrefix;
		pBuffer.append( "else" );
		qDebug() << pBuffer;
		
		const QString oldPrefix( pPrefix );
		pPrefix.append( "  " );
		
		if( node->elseSequence ){
			const KDevPG::ListNode<StatementAst*> *iter = node->elseSequence->front();
			const KDevPG::ListNode<StatementAst*> *end = iter;
			do{
				visitNode( iter->element );
				iter = iter->next;
			}while( iter != end );
		}
		
		pPrefix = oldPrefix;
	}
	
	pBuffer = pPrefix;
	pBuffer.append( "end if" );
}

void DebugAst::visitStatementElif( StatementElifAst *node ){
	pBuffer = pPrefix;
	pBuffer.append( "elif " );
	visitNode( node->condition );
	qDebug() << pBuffer;
	
	if( node->bodySequence ){
		const QString oldPrefix( pPrefix );
		pPrefix.append( "  " );
		
		const KDevPG::ListNode<StatementAst*> *iter = node->bodySequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
		
		pPrefix = oldPrefix;
	}
}

void DebugAst::visitStatementSelect( StatementSelectAst *node ){
	pBuffer.append( "select " );
	visitNode( node->value );
	qDebug() << pBuffer;
	
	if( node->caseSequence ){
		const KDevPG::ListNode<StatementCaseAst*> *iter = node->caseSequence->front();
		const KDevPG::ListNode<StatementCaseAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
	
	if( node->elseSequence ){
		pBuffer = pPrefix;
		pBuffer.append( "else" );
		qDebug() << pBuffer;
		
		const QString oldPrefix( pPrefix );
		pPrefix.append( "  " );
		
		if( node->elseSequence ){
			const KDevPG::ListNode<StatementAst*> *iter = node->elseSequence->front();
			const KDevPG::ListNode<StatementAst*> *end = iter;
			do{
				visitNode( iter->element );
				iter = iter->next;
			}while( iter != end );
		}
		
		pPrefix = oldPrefix;
	}
	
	pBuffer = pPrefix;
	pBuffer.append( "end select" );
}

void DebugAst::visitStatementCase( StatementCaseAst *node ){
	pBuffer = pPrefix;
	pBuffer.append( "case " );
	
	if( node->matchesSequence ){
		const KDevPG::ListNode<ExpressionAst*> *iter = node->matchesSequence->front();
		const KDevPG::ListNode<ExpressionAst*> *end = iter;
		do{
			if( iter != end ){
				pBuffer.append( ", " );
			}
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
	
	qDebug() << pBuffer;
	
	if( node->bodySequence ){
		const QString oldPrefix( pPrefix );
		pPrefix.append( "  " );
		
		const KDevPG::ListNode<StatementAst*> *iter = node->bodySequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
		
		pPrefix = oldPrefix;
	}
}

void DebugAst::visitStatementWhile( StatementWhileAst *node ){
	pBuffer.append( "while " );
	visitNode( node->condition );
	qDebug() << pBuffer;
	
	if( node->bodySequence ){
		const QString oldPrefix( pPrefix );
		pPrefix.append( "  " );
		
		const KDevPG::ListNode<StatementAst*> *iter = node->bodySequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
		
		pPrefix = oldPrefix;
	}
	
	pBuffer = pPrefix;
	pBuffer.append( "end while" );
}

void DebugAst::visitStatementFor( StatementForAst *node ){
	pBuffer.append( "for " );
	visitNode( node->variable );
	pBuffer.append( " = " );
	visitNode( node->from );
	
	if( node->to ){
		pBuffer.append( " to " );
		visitNode( node->to );
		
	}else if( node->downto ){
		pBuffer.append( " downto " );
		visitNode( node->downto );
	}
	
	if( node->step ){
		pBuffer.append( " step " );
		visitNode( node->step );
	}
	
	qDebug() << pBuffer;
	
	if( node->bodySequence ){
		const QString oldPrefix( pPrefix );
		pPrefix.append( "  " );
		
		const KDevPG::ListNode<StatementAst*> *iter = node->bodySequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
		
		pPrefix = oldPrefix;
	}
	
	pBuffer = pPrefix;
	pBuffer.append( "end for" );
}

void DebugAst::visitStatementThrow( StatementThrowAst *node ){
	pBuffer.append( "throw" );
	if( node->exception ){
		pBuffer.append( ' ' );
		visitNode( node->exception );
	}
}

void DebugAst::visitStatementTry( StatementTryAst *node ){
	pBuffer.append( "try" );
	qDebug() << pBuffer;
	
	const QString oldPrefix( pPrefix );
	pPrefix.append( "  " );
	
	if( node->bodySequence ){
		const KDevPG::ListNode<StatementAst*> *iter = node->bodySequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
	
	pPrefix = oldPrefix;
	
	if( node->catchesSequence ){
		const KDevPG::ListNode<StatementCatchAst*> *iter = node->catchesSequence->front();
		const KDevPG::ListNode<StatementCatchAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
	
	pBuffer = pPrefix;
	pBuffer.append( "end try" );
}

void DebugAst::visitStatementCatch( StatementCatchAst *node ){
	pBuffer = pPrefix;
	pBuffer.append( "catch " );
	visitNode( node->type );
	pBuffer.append( ' ' ).append( tokenText( *node->variable ) );
	qDebug() << pBuffer;
	
	if( node->bodySequence ){
		const QString oldPrefix( pPrefix );
		pPrefix.append( "  " );
		
		const KDevPG::ListNode<StatementAst*> *iter = node->bodySequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
		
		pPrefix = oldPrefix;
	}
}

void DebugAst::visitStatementVariableDefinitions( StatementVariableDefinitionsAst *node ){
	pBuffer.append( "var " );
	visitNode( node->type );
	pBuffer.append( ' ' );
	
	if( node->variablesSequence ){
		const KDevPG::ListNode<StatementVariableDefinitionAst*> *iter = node->variablesSequence->front();
		const KDevPG::ListNode<StatementVariableDefinitionAst*> *end = iter;
		do{
			if( iter != end ){
				pBuffer.append( ", " );
			}
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
}

void DebugAst::visitStatementVariableDefinition( StatementVariableDefinitionAst *node ){
	pBuffer.append( tokenText( *node->name ) );
	if( node->value ){
		pBuffer.append( " = " );
		visitNode( node->value );
	}
}

void DebugAst::visitStatementReturn( StatementReturnAst *node ){
	pBuffer.append( "return" );
	if( node->value ){
		pBuffer.append( ' ' );
		visitNode( node->value );
	}
}



void DebugAst::visitExpressionAdditionMore( ExpressionAdditionMoreAst *node ){
	pBuffer.append( ' ' ).append( tokenText( *node->op ) ).append( ' ' );
	visitNode( node->right );
}

void DebugAst::visitExpressionAssignMore( ExpressionAssignMoreAst *node ){
	pBuffer.append( ' ' ).append( tokenText( *node->op ) ).append( ' ' );
	visitNode( node->right );
}

void DebugAst::visitExpressionBitOperationMore( ExpressionBitOperationMoreAst *node ){
	pBuffer.append( ' ' ).append( tokenText( *node->op ) ).append( ' ' );
	visitNode( node->right );
}

void DebugAst::visitExpressionBlock( ExpressionBlockAst *node ){
	const QString oldPrefix( pPrefix );
	pBuffer.append( "block" );
	
	pPrefix.append( "  " );
	DefaultVisitor::visitExpressionBlock( node );
	pPrefix = oldPrefix;
	
	pBuffer = pPrefix;
	pBuffer.append( "end-block" );
}

void DebugAst::visitExpressionBlockBegin( ExpressionBlockBeginAst *node ){
	if( node->argumentsSequence ){
		pBuffer.append( ' ' );
		const KDevPG::ListNode<ExpressionBlockArgumentAst*> *iter = node->argumentsSequence->front();
		const KDevPG::ListNode<ExpressionBlockArgumentAst*> *end = iter;
		do{
			if( iter != end ){
				pBuffer.append( ", " );
			}
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
	
	qDebug() << pBuffer;
}

void DebugAst::visitExpressionBlockArgument( ExpressionBlockArgumentAst *node ){
	visitNode( node->type );
	pBuffer.append( ' ' ).append( tokenText( *node->name ) );
}

void DebugAst::visitExpressionBlockEnd( ExpressionBlockEndAst *node ){
	Q_UNUSED(node)
}

void DebugAst::visitExpressionCompareMore( ExpressionCompareMoreAst *node ){
	pBuffer.append( ' ' ).append( tokenText( *node->op ) ).append( ' ' );
	visitNode( node->right );
}

void DebugAst::visitExpressionConstant( ExpressionConstantAst *node ){
	pBuffer.append( tokenText( *node ) );
}

void DebugAst::visitExpressionGroup( ExpressionGroupAst *node ){
	pBuffer.append( '(' );
	visitNode( node->expression );
	pBuffer.append( ')' );
}

void DebugAst::visitExpressionInlineIfElseMore( ExpressionInlineIfElseMoreAst *node ){
	pBuffer.append( " if " );
	visitNode( node->expressionIf );
	pBuffer.append( " else " );
	visitNode( node->expressionElse );
}

void DebugAst::visitExpressionLogicMore( ExpressionLogicMoreAst *node ){
	pBuffer.append( ' ' ).append( tokenText( *node->op ) ).append( ' ' );
	visitNode( node->right );
}

void DebugAst::visitExpressionMember( ExpressionMemberAst *node ){
	pBuffer.append( tokenText( *node->name ) );
	
	if( node->funcCall != -1 ){
		pBuffer.append( '(' );
		
		if( node->argumentsSequence ){
			const KDevPG::ListNode<ExpressionAst*> *iter = node->argumentsSequence->front();
			const KDevPG::ListNode<ExpressionAst*> *end = iter;
			do{
				if( iter != end ){
					pBuffer.append( ", " );
				}
				visitNode( iter->element );
				iter = iter->next;
			}while( iter != end );
		}
		
		pBuffer.append( ')' );
	}
}

void DebugAst::visitExpressionMultiplyMore( ExpressionMultiplyMoreAst *node ){
	pBuffer.append( ' ' ).append( tokenText( *node->op ) ).append( ' ' );
	visitNode( node->right );
}

void DebugAst::visitExpressionObject( ExpressionObjectAst *node ){
	visitNode( node->object );
	
	if( node->memberSequence ){
		const KDevPG::ListNode<ExpressionMemberAst*> *iter = node->memberSequence->front();
		const KDevPG::ListNode<ExpressionMemberAst*> *end = iter;
		do{
			pBuffer.append( '.' );
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
}

void DebugAst::visitExpressionPostfix( ExpressionPostfixAst *node ){
	visitNode( node->left );
	
	if( node->opSequence ){
		const KDevPG::ListNode<ExpressionPostfixOpAst*> *iter = node->opSequence->front();
		const KDevPG::ListNode<ExpressionPostfixOpAst*> *end = iter;
		do{
			pBuffer.append( tokenText( *iter->element ) );
			iter = iter->next;
		}while( iter != end );
	}
}

void DebugAst::visitExpressionSpecialMore( ExpressionSpecialMoreAst *node ){
	pBuffer.append( ' ' ).append( tokenText( *node->op ) ).append( ' ' );
	visitNode( node->type );
}

void DebugAst::visitExpressionUnary( ExpressionUnaryAst *node ){
	if( node->opSequence ){
		const KDevPG::ListNode<ExpressionUnaryOpAst*> *iter = node->opSequence->front();
		const KDevPG::ListNode<ExpressionUnaryOpAst*> *end = iter;
		do{
			pBuffer.append( tokenText( *iter->element ) );
			iter = iter->next;
		}while( iter != end );
	}
	
	visitNode( node->right );
}



void DebugAst::visitFullyQualifiedClassname( FullyQualifiedClassnameAst *node ){
	if( node->nameSequence ){
		const KDevPG::ListNode<IdentifierAst*> *iter = node->nameSequence->front();
		const KDevPG::ListNode<IdentifierAst*> *end = iter;
		do{
			if( iter != end ){
				pBuffer.append( '.' );
			}
			pBuffer.append( tokenText( *iter->element ) );
			iter = iter->next;
		}while( iter != end );
	}
}

void DebugAst::visitTypeModifier( TypeModifierAst *node ){
	pBuffer.append( tokenText( node->startToken ) ).append( ' ' );
}

QString DebugAst::tokenText( qint64 token ) const{
	const KDevPG::Token &realToken = pTokenStream.at( token );
	return QString::fromUtf8( pContents.mid( realToken.begin, realToken.end - realToken.begin + 1 ) );
}

QString DebugAst::tokenText( const AstNode &node ) const{
	const qint64 realBegin = pTokenStream.at( node.startToken ).begin;
	const qint64 realEnd = pTokenStream.at( node.endToken ).end;
	return QString::fromUtf8( pContents.mid( realBegin, realEnd - realBegin + 1 ) );
}

}
