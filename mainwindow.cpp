#include "mainwindow.h"
#include "ui_mainwindow.h"

QTextCharFormat textFormatBackground(QColor background)
{
  QTextCharFormat result;
  result.setBackground(background);
  result.setFontFamily("cascadia code");
  return result;
}

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  selectedChar= QTextCursor(ui->filePreviewTextEdit->document());
  ui->listOfTokensTable->setColumnCount(2);
  ui->listOfTokensTable->setHorizontalHeaderLabels(QStringList()<<"Token Type"<<"Token Value");
  ui->filePreviewTextEdit->setReadOnly(true);
  ui->filePreviewTextEdit->setFontFamily("cascadia code");
  numberOfTokens=0;
  cursorPosition=0;
  tokensChanged=false;
  this->setWindowTitle("Tiny Scanner");

}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::on_browsePushButton_clicked()
{
  tokensChanged=false;
  ui->filePathLineEdit->setText(
                     QFileDialog::getOpenFileName(
                     this,
                     tr("Open File"),
                     ui->filePathLineEdit->text(),
                     tr("TINY files (*.tiny);;Text files (*.txt)")));
  viewOpenFile(ui->filePathLineEdit->text());
}

void MainWindow::on_filePathLineEdit_editingFinished()
{
  if(QFile(ui->filePathLineEdit->text()).exists())
    {
       viewOpenFile(ui->filePathLineEdit->text());
    }
  else
    {
      QMessageBox::warning(this,"Not Found","This File doesn't Exist");
    }
}

void MainWindow::viewOpenFile(QString filename)
{
  numberOfTokens=0;
  cursorPosition=0;
  openFile.close();
  ui->filePreviewTextEdit->clear();
  ui->listOfTokensTable->clearContents();
  ui->listOfTokensTable->setRowCount(numberOfTokens);
  openFile.setFileName(filename);
  openFile.open(QFile::ReadOnly | QFile::Text);
  if(openFile.isOpen())
    {
      QTextStream openFileStream(&openFile);
      ui->filePreviewTextEdit->setText(openFileStream.readAll());
      selectedChar=QTextCursor(ui->filePreviewTextEdit->document());
      mainScanner.insert_next_char(takeOneChar());
    }
}

void MainWindow::on_resetScanButton_clicked()
{
  viewOpenFile(ui->filePathLineEdit->text());
}

void MainWindow::on_scanNextButton_clicked()
{
  if(cursorPosition+1 < ui->filePreviewTextEdit->document()->characterCount())
    {
      evaluation result = mainScanner.insert_next_char(takeOneChar());
      if(result.tokenEvaluated)
        {
          addToken(result);
        }
    }
}

void MainWindow::on_scanTokenButton_clicked()
{
  evaluation result;
  if(cursorPosition+1 < ui->filePreviewTextEdit->document()->characterCount())
    {
      do
      {
        result= mainScanner.insert_next_char(takeOneChar());
      }
      while(!result.tokenEvaluated && cursorPosition+1 < ui->filePreviewTextEdit->document()->characterCount());
      if(result.tokenEvaluated)addToken(result);
    }
}

void MainWindow::on_scanFileButton_clicked()
{
  evaluation result;
  do
  {
    result= mainScanner.insert_next_char(takeOneChar());
    if(result.tokenEvaluated)
      {
        addToken(result);
      }
  }
  while(  cursorPosition+1 < ui->filePreviewTextEdit->document()->characterCount());
}

char MainWindow::takeOneChar()
{
  QString input=0;
  selectedChar.setCharFormat(textFormatBackground(Qt::transparent));
  if(cursorPosition+1 < ui->filePreviewTextEdit->document()->characterCount())
  {
    selectedChar.setPosition(cursorPosition++,QTextCursor::MoveAnchor);
    selectedChar.setPosition(cursorPosition, QTextCursor::KeepAnchor);
    input = selectedChar.selectedText();
  }
  selectedChar.setCharFormat(textFormatBackground(Qt::darkCyan));
  return input.toStdString()[0];
}

void MainWindow::addToken(evaluation token)
{
  tokensChanged=true;
  numberOfTokens++;
  ui->listOfTokensTable->setRowCount(numberOfTokens);
  ui->listOfTokensTable->setItem(numberOfTokens-1,0,new QTableWidgetItem(QString(tokenName[token.tokenType])));
  ui->listOfTokensTable->setItem(numberOfTokens-1,1,new QTableWidgetItem(QString(token.tokenValue)));
}

void MainWindow::on_actionOpen_triggered()
{
    on_browsePushButton_clicked();
}

void MainWindow::on_actionExport_List_Of_Tokens_triggered()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                              ".\\untitled.csv",
                              tr("CSV files (*.csv)"));
  if(!fileName.isEmpty())
    {
      QFile exportedFile(fileName);
      exportedFile.open(QFile::WriteOnly|QFile::Text);
      QTextStream exportFileStream(&exportedFile);
      for(int i=0 ; i<numberOfTokens ; i++)
        {
          exportFileStream<<ui->listOfTokensTable->item(i,1)->text()
                     <<','<<ui->listOfTokensTable->item(i,0)->text()
                     <<'\n';
        }
      exportFileStream.flush();
      exportedFile.close();
    }
  tokensChanged=false;
}

void MainWindow::on_actionExit_triggered()
{
  this->close();
}

void MainWindow::on_actionAbout_QT_triggered()
{
  QMessageBox::aboutQt(this , "About QT");
}

void MainWindow::on_actionAbout_triggered()
{
  QMessageBox::about(this, "About",
                     "This program is made for the first assignment of Systems Software course"
                     " for fourth year CSE in Faculty of Engineering ASU.\n"
                     "This entry is submitted by:\n"
                     "Bassem Osama Farouk (1500400)\n"
                     "Khaled Shehab Shams Eldin (15X0031)\n"
                     "Ayman Mohamed Saad Eldin (14T0045)\n"
                     "Assem AbdElMoniem Ibrahim (1205036)\n"
                     "Kirollos Samir Asaad (1601023)");
}

void MainWindow::on_actionHow_to_use_triggered()
{
  QMessageBox::information(this,"How to use",
                           "Press the Browse Button to open a file into the text preview box\n"
                           "You can open the code either as .tiny or .txt\n"
                           "You can hover over the buttons with the mouse for a brief "
                           "tooltip explaining what the button does\n"
                           "From left to write the buttons are:\n"
                           "Reset scan\n"
                           "scan next character\n"
                           "scan next token\n"
                           "scan the whole file",
                           QMessageBox::Ok , QMessageBox::Ok);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (tokensChanged)
      {
        QMessageBox::StandardButton reply =
        QMessageBox::question(this , "Token Table not Exported" ,
                                 "The Token Table was changed since the last export\n"
                                 " Do you want to export it before leaving?",
                                 QMessageBox::Yes|QMessageBox::No , QMessageBox::Yes);
        if(reply == QMessageBox::Yes)
          {
              on_actionExport_List_Of_Tokens_triggered();
          }
      }
    event->accept();
}
