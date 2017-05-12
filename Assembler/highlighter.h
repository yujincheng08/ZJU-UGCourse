#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

class Highlighter : public QSyntaxHighlighter
{
	Q_OBJECT
public:
	Highlighter(QTextDocument *parent = 0);

protected:
	void highlightBlock(const QString &text) override;

private:
	struct HighlightingRule
	{
		QString pattern;
		QTextCharFormat format;
	};
	QVector<HighlightingRule> highlightingRules;

	QTextCharFormat keywordFormat;
	QTextCharFormat numFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat registerFormat;
	QTextCharFormat markFormat;
};

#endif // HIGHLIGHTER_H
