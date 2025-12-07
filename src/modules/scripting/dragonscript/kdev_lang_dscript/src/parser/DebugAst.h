#ifndef _DEBUGAST_H_
#define _DEBUGAST_H_

#include "dsp_defaultvisitor.h"

namespace DragonScript{

class TokenStream;

class KDEVDSPARSER_EXPORT DebugAst : public DefaultVisitor{
private:
	const TokenStream &pTokenStream;
	const QByteArray &pContents;
	QString pBuffer;
	QString pPrefix;
	
public:
	DebugAst(const TokenStream &tokenStream, const QByteArray &contents);
	
	void visitNamespace(NamespaceAst *node) override;
	void visitPin(PinAst *node) override;
	void visitRequires(RequiresAst *node) override;
	void visitScriptDeclaration(ScriptDeclarationAst *node) override;
	
	void visitClass(ClassAst *node) override;
	void visitClassBegin(ClassBeginAst *node) override;
	void visitClassBodyDeclaration(ClassBodyDeclarationAst *node) override;
	void visitClassBody(ClassBodyAst *node) override;
	void visitClassEnd(ClassEndAst *node) override;
	
	void visitInterface(InterfaceAst *node) override;
	void visitInterfaceBegin(InterfaceBeginAst *node) override;
	void visitInterfaceBodyDeclaration(InterfaceBodyDeclarationAst *node) override;
	void visitInterfaceBody(InterfaceBodyAst *node) override;
	void visitInterfaceEnd(InterfaceEndAst *node) override;
	
	void visitClassFunctionDeclare(ClassFunctionDeclareAst *node) override;
	void visitClassFunctionDeclareBegin(ClassFunctionDeclareBeginAst *node) override;
	void visitClassFunctionDeclareArgument(ClassFunctionDeclareArgumentAst *node) override;
	void visitClassFunctionEnd(ClassFunctionEndAst *node) override;
	
	void visitClassVariablesDeclare(ClassVariablesDeclareAst *node) override;
	void visitClassVariableDeclare(ClassVariableDeclareAst *node) override;
	
	void visitEnumeration(EnumerationAst *node) override;
	void visitEnumerationBegin(EnumerationBeginAst *node) override;
	void visitEnumerationBody(EnumerationBodyAst *node) override;
	void visitEnumerationEnd(EnumerationEndAst *node) override;
	
	void visitStatement(StatementAst *node) override;
	void visitStatementIf(StatementIfAst *node) override;
	void visitStatementElif(StatementElifAst *node) override;
	void visitStatementSelect(StatementSelectAst *node) override;
	void visitStatementCase(StatementCaseAst *node) override;
	void visitStatementWhile(StatementWhileAst *node) override;
	void visitStatementFor(StatementForAst *node) override;
	void visitStatementThrow(StatementThrowAst *node) override;
	void visitStatementTry(StatementTryAst *node) override;
	void visitStatementCatch(StatementCatchAst *node) override;
	void visitStatementVariableDefinitions(StatementVariableDefinitionsAst *node) override;
	void visitStatementVariableDefinition(StatementVariableDefinitionAst *node) override;
	void visitStatementReturn(StatementReturnAst *node) override;
	
	void visitExpressionAdditionMore(ExpressionAdditionMoreAst *node) override;
	void visitExpressionAssignMore(ExpressionAssignMoreAst *node) override;
	void visitExpressionBitOperationMore(ExpressionBitOperationMoreAst *node) override;
	void visitExpressionBlock(ExpressionBlockAst *node) override;
	void visitExpressionBlockArgument(ExpressionBlockArgumentAst *node) override;
	void visitExpressionBlockBegin(ExpressionBlockBeginAst *node) override;
	void visitExpressionBlockEnd(ExpressionBlockEndAst *node) override;
	void visitExpressionCompareMore(ExpressionCompareMoreAst *node) override;
	void visitExpressionConstant(ExpressionConstantAst *node) override;
	void visitExpressionGroup(ExpressionGroupAst *node) override;
	void visitExpressionInlineIfElseMore(ExpressionInlineIfElseMoreAst *node) override;
	void visitExpressionLogicMore(ExpressionLogicMoreAst *node) override;
	void visitExpressionMember(ExpressionMemberAst *node) override;
	void visitExpressionMultiplyMore(ExpressionMultiplyMoreAst *node) override;
	void visitExpressionObject(ExpressionObjectAst *node) override;
	void visitExpressionPostfix(ExpressionPostfixAst *node) override;
	void visitExpressionSpecialMore(ExpressionSpecialMoreAst *node) override;
	void visitExpressionUnary(ExpressionUnaryAst *node) override;
	
	void visitFullyQualifiedClassname(FullyQualifiedClassnameAst *node) override;
	void visitTypeModifier(TypeModifierAst *node) override;
	
	QString tokenText(qint64 token) const;
	QString tokenText(const AstNode &node) const;
};

}

#endif
