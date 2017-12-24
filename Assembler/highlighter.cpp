#include "highlighter.h"
#include "instructionlist.h"
#include <QRegularExpression>
Highlighter::Highlighter(QTextDocument *parent)
	: QSyntaxHighlighter(parent)
{
	HighlightingRule rule;

	keywordFormat.setForeground(Qt::darkRed);
	keywordFormat.setFontWeight(QFont::Bold);
//	QStringList keywordPatterns;
//	keywordPatterns << "\\blw\\b" << "\\blb\\b" << "\\blbu\\b"
//					<< "\\blh\\b" << "\\blhu\\b" << "\\bsw\\b"
//					<< "\\bsb\\b" << "\\bsh\\b" << "\\badd\\b"
//					<< "\\baddu\\b" << "\\bsub\\b" << "\\bsubu\\b"
//					<< "\\bslt\\b" << "\\bsltu\\b" << "\\band\\b"
//					<< "\\bor\\b" << "\\bxor\\b" << "\\bnor\\b"
//					<< "\\bsll\\b" << "\\bsrl\\b" << "\\bsra\\b"
//					<< "\\bmult\\b" << "\\bmultu\\b" << "\\bdiv\\b"
//					<< "\\bdivu\\b" << "\\baddi\\b" << "\\baddiu\\b"
//					<< "\\bori\\b" << "\\bxori\\b" << "\\blui\\b"
//					<< "\\bslti\\b" << "\\bsltiu\\b" << "\\bbeq\\b"
//					<< "\\bbne\\b" << "\\bblez\\b" << "\\bbgtz\\b"
//					<< "\\bbltz\\b" << "\\bbgez\\b" << "\\bj\\b"
//					<< "\\bjal\\b" << "\\bjalr\\b" << "\\bjr\\b"
//					<< "\\bmfhi\\b" << "\\bmflo\\b" << "\\bmthi\\b"
//					<< "\\bmtlo\\b" << "\\beret\\b" << "\\bmfco\\b"
//					<< "\\bmtco\\b" << "\\bbreak\\b" << "\\bsyscall\\b"
//					<< "\\bla\\b" << "\\bmove\\b" << "\\bli\\b";



	numFormat.setForeground(Qt::darkMagenta);
    rule.pattern = "\\b(0x|0X)?[a-fA-F0-9]+\\b";
	rule.format = numFormat;
	highlightingRules.append(rule);

	registerFormat.setFontWeight(QFont::Bold);
	registerFormat.setForeground(Qt::darkBlue);
    rule.pattern = "(\\$\\b(\\d|[12]\\d|3[01]|zero|at|[vk][01]|"
						   "a[0-3]|t\\d|s[0-7]|gp|sp|fp|ra)\\b)|"
						   "(\\b[rR](\\d|[12]\\d|3[01])\\b)";
	rule.format = registerFormat;
	highlightingRules.append(rule);

	markFormat.setForeground(Qt::darkGray);
	rule.pattern = "(\\b\\w+\\b(?=:))|(((?<=jal)|(?<=j))\\s+\\b\\w+\\b)";
	rule.format = markFormat;
	highlightingRules.append(rule);

    foreach(const Instruction &instruction, instructionList)
    {
        rule.pattern = "\\b" + instruction.symbol() + "\\b";
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

	singleLineCommentFormat.setForeground(Qt::darkGreen);
	rule.pattern = "(//|#)[^\n]*";
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);
}


void Highlighter::highlightBlock(const QString &text)
{
//	foreach (const HighlightingRule &rule, highlightingRules)
//	{
//		QRegExp expression(rule.pattern);
//		int index = expression.indexIn(text);
//		while (index >= 0)
//		{
//			int length = expression.matchedLength();
//			setFormat(index, length, rule.format);
//			index = expression.indexIn(text, index + length);
//		}
//	}

//	QTextCharFormat format;
//	format.setFontWeight(QFont::Bold);
//	format.setForeground(Qt::red);

	foreach(const HighlightingRule &rule, highlightingRules)
	{
		QRegularExpression expression(rule.pattern);
		auto i = expression.globalMatch(text);
		while(i.hasNext())
		{
			auto match = i.next();
			setFormat(match.capturedStart(),match.capturedLength(),
					  rule.format);
		}
	}


}
