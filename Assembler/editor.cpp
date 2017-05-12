#include "editor.h"
#include "linenumwidget.h"
#include <QPainter>
#include <QTextBlock>
#include <QTextStream>
#include <QtDebug>
Editor::Editor(QWidget *parent) : QPlainTextEdit(parent)
{
	file = new QFile(this);
	setFont(QFont(QString("Source Code Pro"),12));
	setTabStopWidth(4 * fontMetrics().width(' '));

	QPalette p = palette();
	p.setColor(QPalette::Inactive, QPalette::Highlight,
			   p.color(QPalette::Active, QPalette::Highlight));
	p.setColor(QPalette::Inactive, QPalette::HighlightedText,
			   p.color(QPalette::Active, QPalette::HighlightedText));
	setPalette(p);
	lineNumberArea = new LineNumWidget(this);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	updateLineNumberAreaWidth(0);
	highlightCurrentLine();
}

int Editor::lineNumberAreaWidth()
{
	int digits = 1;
	int max = qMax(1,blockCount());
	while(max >= 10)
	{
		max /=10;
		++digits;
	}

	int space = fontMetrics().width(QLatin1Char('9')) * ++digits;
	return space;
}

bool Editor::saveFile(const QString &fileName)
{
	file->setFileName(fileName);
	if(!file->open(QIODevice::WriteOnly|QIODevice::Text))
		return false;
	QTextStream textStream(file);
	textStream << toPlainText();

	file->close();
	return true;
}

bool Editor::loadFile(const QString &fileName)
{
	file->setFileName(fileName);
	if(!file->open(QIODevice::ReadOnly|QIODevice::Text))
		return false;
	QTextStream textStream(file);
	setPlainText(textStream.readAll());

	file->close();
	return true;
}

void Editor::autoIndent()
{
	QTextBlock lastBolck = textCursor().block().previous();
	if(!lastBolck.isValid())
		return;
	QString blockText = lastBolck.text();
	QString insertText;
	for(auto i = blockText.begin(); i != blockText.end(); ++i)
		if(*i == '\t')
			insertText.append('\t');
		else if(*i == ' ')
			insertText.append(' ');
		else
			break;
	textCursor().insertText(insertText);
}

void Editor::updateLineNumberAreaWidth(int)
{
	setViewportMargins(lineNumberAreaWidth(),0,0,0);
}

void Editor::updateLineNumberArea(const QRect &rect, int dy)
{
	if(dy)
		lineNumberArea->scroll(0,dy);
	else
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(),
							   rect.height());
	if(rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}

void Editor::resizeEvent(QResizeEvent *event)
{
	QPlainTextEdit::resizeEvent(event);

	QRect cr = contentsRect();
	lineNumberArea->setGeometry(QRect(cr.left(),cr.top(),
									  lineNumberAreaWidth(),cr.height()));
}

void Editor::keyPressEvent(QKeyEvent *e)
{
	QPlainTextEdit::keyPressEvent(e);
	if(e->key() != Qt::Key_Enter && e->key() != Qt::Key_Return)
		return;
	autoIndent();
}

void Editor::removeSelectedText()
{
	textCursor().removeSelectedText();
}

void Editor::highlightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if(!isReadOnly())
	{
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(Qt::lightGray);
		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection,true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}
	setExtraSelections(extraSelections);
}

void Editor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
	QPainter painter(lineNumberArea);
	painter.fillRect(event->rect(),Qt::lightGray);
	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int) blockBoundingRect(block).height();

	while(block.isValid() && top <= event->rect().bottom())
	{
		if(block.isVisible() && bottom >= event->rect().top())
		{
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::black);
			painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
							 Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + (int) blockBoundingRect(block).height();
		++blockNumber;
	}
}
