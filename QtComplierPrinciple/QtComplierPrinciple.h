#pragma once
#define RPC_NO_WINDOWS_H
#include <windows.h>

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
#include <qcombobox.h>

class PUTIL;

const std::string toQtFilePath = R"(C:\Grammar\ToQtText\toQt.txt)";

class QtComplierPrinciple : public QMainWindow
{
    Q_OBJECT

public:
    QtComplierPrinciple(QWidget *parent = nullptr);
    ~QtComplierPrinciple();

private:
    // 文法列表，保存当前文件中的文法
    std::vector<Grammar> grammars;
    // 界面初始化
    int currFile = 0;
    int currFloder = 0;
    // 保存当前文法列表的路径
    std::vector<std::string> grammarFiles;
    Ui::QtComplierPrincipleClass ui;
    // 下拉项
    QComboBox* comboFuncBox;
private slots:
    void on_actOpenFolder_triggered();
    void on_actNewGrammarFile_triggered();
    // treeItem的单击事件
    void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column);
    /// <summary>
    /// listItem的单击事件：选中某项，将其放入到当前项中
    /// </summary>
    /// <param name="itme"></param>
    /// <param name="column"></param>
    void on_listWidget_itemClicked(QListWidgetItem* item);
    // 文法可编辑状态改变
    void on_checkBox_checkStateChanged(Qt::CheckState checked);
    // 文法内容改变
    void on_textEdit_textChanged();
    // 保存文法点击
    void on_actSaveGrammar_triggered();
    // 删除文法点击
    void on_actDeleteGrammar_triggered();
    // 全选文法
    void on_actSelectAllGrammar_triggered();
    // 运行功能
    void on_actRun_triggered();
private:
    void qf0();// 加标记
    void qf1();// LL1自顶向下
    void qf2();// 消除左递归
    void qf3();// DFA推导
    void qf4();// NFA转化为DFA
    void qf5();// 递归下降分析
    void qf6();// 算符优先符表 识别句子
    void qf7();// 算符优先符表转化为FG函数表
    // 用于输出到命令行的字串
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
    
    int oldRow = -1;
public:
    friend class PUTIL;
    void performAction(Callback::CallbackType callback);
};