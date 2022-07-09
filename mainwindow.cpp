#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Connections between signals and slots */
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newFile);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::print);
    connect(ui->actionPrint_Preview, &QAction::triggered, this, &MainWindow::printPreviewDialog);
    connect(ui->actionExport_PDF, &QAction::triggered, this, &MainWindow::exportPdf);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exit);
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::redo);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::cut);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copy);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::paste);
    connect(ui->actionUnderline, &QAction::triggered, this, &MainWindow::underline);
    connect(ui->actionBold, &QAction::triggered, this, &MainWindow::bolden);
    connect(ui->actionItalic, &QAction::triggered, this, &MainWindow::italicize);
    connect(ui->actionLeft, &QAction::triggered, this, &MainWindow::alignLeft);
    connect(ui->actionCenter, &QAction::triggered, this, &MainWindow::alignCenter);
    connect(ui->actionRight, &QAction::triggered, this, &MainWindow::alignRight);
    connect(ui->actionJustify, &QAction::triggered, this, &MainWindow::justify);
    connect(ui->actionColor, &QAction::triggered, this, &MainWindow::changeColor);
    connect(ui->actionFont, &QAction::triggered, this, &MainWindow::changeFont);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    MainWindow::exit(true);
}

void MainWindow::newFile()
{
    currentFile.clear();
    setWindowTitle("Untitled");
    ui->textEdit->setText(QString());
}


void MainWindow::openFile()
{
    QString fileName{QFileDialog::getOpenFileName(this, "Open file")};
    MainWindow::loadText(fileName);

    moveCursorToEnd();
}

void MainWindow::save()
{
    QString fileName{};

    if(currentFile.isEmpty())
    {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFile = fileName;
    }
    else
    {
        fileName = currentFile;
    }

    MainWindow::writeFile(fileName);

    setWindowTitle(fileName);


    MainWindow::loadText(fileName);
}

void MainWindow::saveAs()
{
    QString fileName{QFileDialog::getSaveFileName(this, "Save As")};


    MainWindow::writeFile(fileName);

    currentFile = fileName;
    setWindowTitle(fileName);

    MainWindow::loadText(fileName);
}

void MainWindow::print()
{
    QPrinter printer{};
    QPrintDialog dialog{&printer, this};
    if(dialog.exec() != QDialog::Rejected)
    {
        ui->textEdit->print(&printer);
    }
}

void MainWindow::printPreview(QPrinter* printer)
{
    ui->textEdit->print(printer);
}

void MainWindow::printPreviewDialog()
{
    QPrinter printer{QPrinter::HighResolution};
    QPrintPreviewDialog preview{&printer, this};

    preview.setWindowFlags(Qt::Window);
    connect(&preview, &QPrintPreviewDialog::paintRequested, this, &MainWindow::printPreview);
    preview.exec();
}

void MainWindow::exportPdf()
{
    QString fileName{QFileDialog::getSaveFileName(this, "Export PDF")};

    if(!fileName.isEmpty())
    {
        QPrinter printer{QPrinter::PrinterResolution};
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        printer.setPageMargins(QMarginsF{30, 30, 30, 30});
        ui->textEdit->document()->print(&printer);
    }
}

void MainWindow::exit(bool isForceClosing = false)
{
    auto text{ui->textEdit->toPlainText()};
    if(!text.isEmpty() || !currentFile.isEmpty())
    {
        QMessageBox::StandardButton response{};

        if(isForceClosing)
        {
            response = QMessageBox::warning(this, "Warning", "Would you like to save before exiting?",
                                 {QMessageBox::StandardButton::Yes |
                                  QMessageBox::StandardButton::No});
        }
        else
        {
            response = QMessageBox::warning(this, "Warning", "Would you like to save before exiting?",
                                 {QMessageBox::StandardButton::Yes |
                                  QMessageBox::StandardButton::No |
                                  QMessageBox::StandardButton::Cancel});
        }


        if(response != QMessageBox::StandardButton::Cancel)
        {

            if(response == QMessageBox::StandardButton::Yes)
            {
                if(!currentFile.isEmpty())
                {
                    MainWindow::save();
                }
                else
                {
                    MainWindow::saveAs();
                }

                QApplication::quit();
            }
            else
            {
                QApplication::quit();
            }
        }
    }

    return;
}

void MainWindow::undo()
{
    ui->textEdit->undo();
}

void MainWindow::redo()
{
    ui->textEdit->redo();
}

void MainWindow::cut()
{
    ui->textEdit->cut();
}

void MainWindow::copy()
{
    ui->textEdit->copy();
}

void MainWindow::paste()
{
    ui->textEdit->paste();
}

void MainWindow::underline()
{
    QFont font{ui->textEdit->font()};
    font.setUnderline(!(ui->textEdit->font().underline()));
    ui->textEdit->setFont(font);
}

void MainWindow::bolden()
{
    QFont font{ui->textEdit->font()};
    font.setBold(!(ui->textEdit->font().bold()));
    ui->textEdit->setFont(font);
}

void MainWindow::italicize()
{
    QFont font{ui->textEdit->font()};
    font.setItalic(!(ui->textEdit->font().italic()));
    ui->textEdit->setFont(font);
}

void MainWindow::alignLeft()
{
    ui->textEdit->setAlignment(Qt::AlignmentFlag::AlignLeft);
}

void MainWindow::alignRight()
{
    ui->textEdit->setAlignment(Qt::AlignmentFlag::AlignRight);
}

void MainWindow::alignCenter()
{
    ui->textEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);
}

void MainWindow::justify()
{
    ui->textEdit->setAlignment(Qt::AlignmentFlag::AlignJustify);
}

void MainWindow::changeColor()
{
    QColorDialog colorDialog{};
    auto color{colorDialog.getColor()};
    ui->textEdit->setTextColor(color);
}

void MainWindow::changeFont()
{
    bool ok{};
    auto font{QFontDialog::getFont(&ok, this)};
    if(ok)
    {
        ui->textEdit->setFont(font);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, "About Application", "This is a simple text editor created by Matthew Crowell at Developers Crowell using Qt6.");
}

void MainWindow::loadText(const QString& fileName)
{
    QFile file(fileName);

    currentFile = fileName;

    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Can not open file " + file.errorString());
        return;
    }

    setWindowTitle(fileName);
    QTextStream in{&file};

    QString text{in.readAll()};
    ui->textEdit->setText(text);

    file.close();
}

void MainWindow::moveCursorToEnd()
{
    std::unique_ptr<QTextCursor> newCursor{std::make_unique<QTextCursor>(ui->textEdit->textCursor())};
    newCursor->movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(*newCursor);
}

void MainWindow::writeFile(const QString& fileName)
{
   QFile file{fileName};
   if(!file.open(QIODevice::WriteOnly | QFile::Text))
   {
       QMessageBox::warning(this, "Warning", "Cannot save file " + file.errorString());
       return;
   }

    QTextStream out{&file};

    QString text{ui->textEdit->toHtml()};

    out << text;

    if(!text.endsWith('\n'))
    {
       out << Qt::endl;
    }

    file.close();
}
