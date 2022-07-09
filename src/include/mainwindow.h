#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPrinter;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newFile();
    void openFile();
    void save();
    void saveAs();
    void print();
    void printPreview(QPrinter* printer);
    void printPreviewDialog();
    void exportPdf();
    void exit(bool);
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void underline();
    void bolden();
    void italicize();
    void alignLeft();
    void alignRight();
    void alignCenter();
    void justify();
    void changeColor();
    void changeFont();
    void about();

private:
    Ui::MainWindow *ui;
    QString currentFile;

    void closeEvent(QCloseEvent* event);

    void loadText(const QString& fileName);
    void moveCursorToEnd();
    void writeFile(const QString& fileName);
};
#endif // MAINWINDOW_H
