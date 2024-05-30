#pragma once
#define RPC_NO_WINDOWS_H
#include <windows.h>

// 你的其他代码


// 你的其他代码
#include <GrammarLib/Grammar.h>



#include <QtWidgets/QMainWindow>
#include "ui_QtComplierPrinciple.h"
#include <vector>
#include <string>
#include <qstring.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qfile.h>
#include <qtextstream.h> 




class QtComplierPrinciple : public QMainWindow
{
    Q_OBJECT

public:
    QtComplierPrinciple(QWidget *parent = nullptr);
    ~QtComplierPrinciple();

private:
    //Ui::QtComplierPrincipleClass ui;
    //std::vector<Grammar> vgfile;
    //std::vector<QString> vfileName;
    int currFile = 0;
    int currFloder = 0;
    std::vector<std::string> grammarFiles;
    //std::vector<std::vector<std::string>> grammarFiles;

    Ui::QtComplierPrincipleClass ui;
private slots:
    void on_actOpenFolder_triggered();
    void on_actNewGrammar_triggered();
    void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column);

private:

    const QString defalutDir = R"(C:\Grammar\)";

    // 查询 给定目录下的一级的所有.txt文件,并加入到tree中
    QStringList findTxtFiles(const QString& directoryPath);
    // 加载默认的语法
    void loadDefalutGrammar();
    // 读取文件的所有内容
    QString readFile(const QString& filePath);

};