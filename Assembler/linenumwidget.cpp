#include "linenumwidget.h"
#include "editor.h"
LineNumWidget::LineNumWidget(Editor *parent)
	: QWidget(parent),editor(parent)
{

}

QSize LineNumWidget::sizeHint() const
{
	return QSize(editor->lineNumberAreaWidth(),0);
}

void LineNumWidget::paintEvent(QPaintEvent *event)
{
	editor->lineNumberAreaPaintEvent(event);
}
