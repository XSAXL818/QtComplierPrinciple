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
#include "Callback.h"



class QtComplierPrinciple : public QMainWindow
{
    Q_OBJECT

public:
    QtComplierPrinciple(QWidget *parent = nullptr);
    ~QtComplierPrinciple();

private:
    //Ui::QtComplierPrincipleClass ui;
    std::vector<Grammar> grammars;
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
    /// <summary>
    /// listItem的单击事件：选中某项，将其放入到当前项中
    /// </summary>
    /// <param name="itme"></param>
    /// <param name="column"></param>
    void on_listWidget_itemClicked(QListWidgetItem* item);
private:

    std::string cmdStr;
    std::vector<std::string> cmdVStr;
    // 默认目录
    const QString defalutDir = R"(C:\Grammar\)";

    // 查询 给定目录下的一级的所有.txt文件,并加入到tree中
    QStringList findTxtFiles(const QString& directoryPath);
    // 加载默认的语法
    void loadDefalutGrammar();
    // 读取文件的所有内容
    QString readFile(const QString& filePath);
    /// <summary>
    /// 将文法的产生式转换为QString
    /// </summary>
    /// <param name="g"></param>
    /// <returns></returns>
    QString getQStringFromGrammar(Grammar g);
    std::vector<std::string> getVStringFromGrammar(Grammar g);
    /// <summary>
    /// 传入要打印的字符串，然后让cmd回到相应的函数中
    /// </summary>
    void callCmd(std::string s);
    void callCmd(std::vector<std::string>& vs);
public:
   
    void performAction(Callback::CallbackType callback);
};