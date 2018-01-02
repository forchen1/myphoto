#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qDebug>
#include <QCryptographicHash>
#include <QDateTime>
#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList header;
    header<<tr("源文件名")<<tr("源路径")<<tr("重命名")<<tr("目标路径")<<tr("Infos")<<tr("Infos");
    photoInfoWidget = new QTableWidget(ui->photoInfoWidget);
    //photoInfoWidget->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    photoInfoWidget->setRowCount(0);
    photoInfoWidget->setColumnCount(header.size());
    photoInfoWidget->setGeometry(0,0,811,301);
    photoInfoWidget->setHorizontalHeaderLabels(header);
    photoInfoWidget->setShowGrid(false);
    photoInfoWidget->setColumnWidth(0, 100);
    photoInfoWidget->setColumnWidth(1, 100);
    photoInfoWidget->setColumnWidth(2, 100);
    photoInfoWidget->setColumnWidth(3, 100);
    photoInfoWidget->setColumnWidth(4, 100);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_selSourcePathButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,tr("选择路径"),"\\");
    ui->sourcePathLineEdit->setText(path);
}

void MainWindow::on_selTargetPathButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,tr("选择路径"),"\\");
    ui->targetPathLineEdit->setText(path);
}

void MainWindow::on_scanButton_clicked()
{

    /*
    int row = taskTableWidget->rowCount();
    taskTableWidget->insertRow(row);
    taskTableWidget->setRowHeight(row, 100);
    QTableWidgetItem * test = new QTableWidgetItem(QIcon(quan->getQuanImage().scaled(QSize(220,100))),"");
    taskTableWidget->setItem(row,0,test);
    taskTableWidget->setItem(row,1,new QTableWidgetItem(acc->getAccName()));
    taskTableWidget->setItem(row,2,new QTableWidgetItem(task->getStatusString()));
    */
    QString path = ui->sourcePathLineEdit->text();
    scanPath(path);
}

void MainWindow::scanPath(QString path){
    GetFileListWithJpgInfo(path);
}

QFileInfoList MainWindow::GetFileListWithJpgInfo(QString path){
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    foreach(QFileInfo single , file_list){
        QStringList fileInfo;
        fileInfo.append(single.fileName());
        fileInfo.append(single.absolutePath());
        fileInfo.append(single.fileName());
        fileInfo.append(single.absolutePath());
        fileInfo.append(single.baseName());
        fileInfo.append(single.baseName());
//        header<<tr("源文件名")<<tr("源路径")<<tr("重命名")<<tr("目标路径")<<tr("Infos")<<tr("Infos");
        insertTableWithFileInfo(fileInfo);
    }

    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folder_list.size(); i++)
    {
         QString name = folder_list.at(i).absoluteFilePath();
         QFileInfoList child_file_list = GetFileListWithJpgInfo(name);
         file_list.append(child_file_list);
    }

    return file_list;
}


void MainWindow::insertTableWithFileInfo(QStringList fileInfo){
    int row = photoInfoWidget->rowCount();
    photoInfoWidget->insertRow(row);
    //photoInfoWidget->setRowHeight(row, 100);
    photoInfoWidget->setItem(row,0,new QTableWidgetItem(fileInfo.at(0)));
    photoInfoWidget->setItem(row,1,new QTableWidgetItem(fileInfo.at(1)));
    photoInfoWidget->setItem(row,2,new QTableWidgetItem(fileInfo.at(2)));
    photoInfoWidget->setItem(row,3,new QTableWidgetItem(fileInfo.at(3)));
}


void MainWindow::on_deleteDuplicateButton_clicked()
{

}



void MainWindow::on_pushButton_clicked()
{
//   QString filename=QFileDialog::getOpenFileName(this,tr("action"),"E:\\demo\\picture\\1.raw","Image file(*.bmp *.jpg)",0);
//   QString path = QFileDialog::getExistingDirectory(this,tr("action"),"\\");
    QString filename = "D:\\IMG_20170912_120903.jpg";
    filename = "E:\\IMG_20170820_141214.jpg";
    filename = "D:\\FIL1057.JPG";

    QMap<QString, QString> tags = StringUtil::readJpgInfo(filename);
    QMap<QString,QString>::Iterator  it;
    for(it = tags.begin(); it != tags.end(); ++it)
    {
        qDebug() <<"key:"<<it.key()<<"value:"<<it.value();
    }

    qDebug() << "finish";
}



QFileInfoList MainWindow::GetFileList(QString path)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folder_list.size(); i++)
    {
         QString name = folder_list.at(i).absoluteFilePath();
         QFileInfoList child_file_list = GetFileList(name);
         file_list.append(child_file_list);
    }

    return file_list;
}


void MainWindow::on_pushButtonDir_clicked()
{
    qDebug() << "on_pushButtonDir_clicked";
    // handle sequence:
    // list all files from path
    // analyse pic infos from jpg file
    // rename file and move file to new path

    QString path = "F:\\照片\\未整理";
    QString destination = "F:\\照片\\自动整理";     //D:\\work\\Desktop2
    QFileInfoList filelist = GetFileList(path);
    QStringList md5List;  // to remove duplicate

    for(int i=0; i<filelist.size(); i++){
//        qDebug() << filelist.at(i).absoluteFilePath();
        QString filename = filelist.at(i).absoluteFilePath();
        if(!filename.endsWith("jpg")){
            qDebug() << filename;
            continue ;
        }
        QFile theFile(filename);
        theFile.open(QIODevice::ReadOnly);
        QByteArray ba = QCryptographicHash::hash(theFile.readAll(), QCryptographicHash::Md5);
        theFile.close();
        //qDebug() << ba.toHex().constData();
        QString md5 = ba.toHex().constData();
        QMap<QString, QString> tags = StringUtil::readJpgInfo(filename);
        QMap<QString,QString>::Iterator  it;

        if(md5List.indexOf(md5)==-1){

            // not exist
            // move file and rename
            // key: "DateTime" value: "2017:06:27 14:34:50"
            // key: "Model" value: "Canon PowerShot A540"
            QString dateTime = tags["DateTime"];
            QString model = tags["Model"].replace(" ", "").replace("\\", "")
                    .replace("/", "").replace("?", "").replace("*", "");
            if(model.length()>16){
                model = tags["Make"].replace(" ", "").replace("\\", "")
                        .replace("/", "").replace("?", "").replace("*", "");
            }
            if(dateTime.length()>0 && model.length()>0){
                QDateTime realTime  = QDateTime::fromString(dateTime, "yyyy:MM:dd hh:mm:ss");
                QString sFilePath = destination + "\\" + QString::number(realTime.date().year(), 10) + "年" + "\\"
                        +  QString::number(realTime.date().month(), 10) + "月" + "\\" + model + "_"
                        + realTime.toString("yyyyMMdd_hhmmss") + ".jpg";
                QString sPath = sFilePath.left(sFilePath.lastIndexOf('\\'));
                QString sFilename = sFilePath.right(sFilePath.length() - sFilePath.lastIndexOf('\\') - 1);
//                qDebug() << sFilePath;
                QDir dir(sPath);
                if(!dir.exists(sPath)){
                    if(!dir.mkpath(sPath)) {
                        qDebug() << "创建目录失败";
                        return ;
                    }
                }
                if(dir.exists(sFilename)){
                    dir.remove(sFilename);
                }
                if(!QFile::copy(filename, sFilePath)){
                    qDebug() << "文件复制失败";
                    return ;
                }else{
                    //QFile::remove(filename);
                }

            }

        }else{
            // rename
        }
    }
// 重复文件要有 提示
}
