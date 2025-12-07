#ifndef _PARSEJOB_H_
#define _PARSEJOB_H_

#include <language/backgroundparser/parsejob.h>
#include <language/duchain/problem.h>

#include <QStringList>

#include <ktexteditor/range.h>

#include <language/duchain/topducontext.h>

using KDevelop::ILanguageSupport;
using KDevelop::IndexedString;
using KDevelop::IProblem;
using KDevelop::ParseJob;
using KDevelop::ProblemPointer;
using KDevelop::ReferencedTopDUContext;
using KDevelop::TopDUContext;

namespace DragonScript{

struct AstNode;
class DSLanguageSupport;
class EditorIntegrator;

class DSParseJob : public ParseJob{
	Q_OBJECT
	
public:
	enum {
		Resheduled = TopDUContext::LastFeature
	};
	
	explicit DSParseJob(const IndexedString &url, ILanguageSupport *languageSupport);
	~DSParseJob() override;
	
	void setParentJob(DSParseJob *job);
	
protected:
	DSLanguageSupport* ds() const;
	void run(ThreadWeaver::JobPointer self, ThreadWeaver::Thread *thread) override;
	
private:
	DSParseJob *pParentJob; ///< parent job if this one is an include
	ReferencedTopDUContext pDUContext;
	
	/**
	 * Checks if a parent job parses already \p document. Used to prevent
	 * endless recursions in include statements
	 */
	bool hasParentDocument(const IndexedString &document);
	
	/** Create a problem pointer for the current document */
	ProblemPointer createProblem(const QString &description, AstNode *node,
		EditorIntegrator *editor, IProblem::Source source,
		IProblem::Severity severity = IProblem::Error);
};

}

#endif
