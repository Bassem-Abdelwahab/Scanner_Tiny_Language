#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <QTextCharFormat>
#include <QTextCursor>
#include <Qcolor>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

#include "scanner.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void viewOpenFile(QString filename);

private slots:
  void on_browsePushButton_clicked();

  void on_filePathLineEdit_editingFinished();

  void on_resetScanButton_clicked();

  void on_scanNextButton_clicked();

  void on_scanTokenButton_clicked();

  void on_scanFileButton_clicked();

  void on_actionOpen_triggered();

  void on_actionExport_List_Of_Tokens_triggered();

  void on_actionExit_triggered();

  void on_actionAbout_QT_triggered();

  void on_actionAbout_triggered();

  void on_actionHow_to_use_triggered();

private:
  char takeOneChar();
  void addToken(evaluation token);
  void closeEvent(QCloseEvent *event);

  Ui::MainWindow *ui;
  scanner mainScanner;
  QFile openFile;
  QTextCursor selectedChar;
  int numberOfTokens;
  int cursorPosition;
  bool tokensChanged;
};
#endif // MAINWINDOW_H
