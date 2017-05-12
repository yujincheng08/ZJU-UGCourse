#ifndef EDITOR_H
#define EDITOR_H

#include <QPlainTextEdit>

class Editor : public QPlainTextEdit
{
	Q_OBJECT
private:
	QWidget *lineNumberArea;
	QFile *file;
public:
	Editor(QWidget *parent = 0);

	void lineNumberAreaPaintEvent(QPaintEvent *event);
	int lineNumberAreaWidth();
	bool saveFile(const QString &fileName);
	bool loadFile(const QString &fileName);
private:
	void autoIndent();
protected:
	void resizeEvent(QResizeEvent *e) override;
	void keyPressEvent(QKeyEvent *e) override;
public slots:
	void removeSelectedText();
private slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect &, int);
};

#endif // EDITOR_H
