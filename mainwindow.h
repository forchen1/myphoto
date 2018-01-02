#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFileDialog>
#include <QMap>
#include <QTableWidget>
#include "stringUtil.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButtonDir_clicked();

    void on_selSourcePathButton_clicked();

    void on_selTargetPathButton_clicked();

    void on_scanButton_clicked();

    void on_deleteDuplicateButton_clicked();

private:
    Ui::MainWindow *ui;
    QTableWidget *photoInfoWidget;

private:



    QFileInfoList GetFileList(QString path);
    QFileInfoList GetFileListWithJpgInfo(QString path);
    void insertTableWithFileInfo(QStringList fileInfo);
    void scanPath(QString path);


};

#endif // MAINWINDOW_H
