#include "QtComplierPrinciple.h"

#include "qcombobox.h"
#include "qpushbutton.h"
#include <qsplitter.h>
#include <qdialog.h>
#include <qfiledialog.h>
#include <iostream>
#include <string>
#include <qicon.h>
#include <QIcon>
#include <GrammarLib/Grammar.h>
#include <GrammarLib/GUtil.h>
#include <qtoolbutton.h>


QtComplierPrinciple::QtComplierPrinciple(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //connect(&ui.treeWidget->itemWidget,)

    // 创建无法可视化UI直接设计的组件
    // 插入语法文件操作
    /*QToolButton* actNewGrammar = new QToolButton(this);
    QAction* actNew = new QAction(this);
    actNew->setText("新建动作");
    ui.toolBar->addAction(actNew);*/

    // 插入运行功能
    QComboBox* comboFuncBox = new QComboBox(this);
    comboFuncBox->setMinimumWidth(150);
    comboFuncBox->setMinimumHeight(40);
    QFont font = comboFuncBox->font();
    font.setPointSize(14);
    comboFuncBox->setFont(font);
    comboFuncBox->addItem("加标记法处理");
    comboFuncBox->addItem("LL1自顶向下推导");
    comboFuncBox->addItem("消除左递归");
    comboFuncBox->addItem("DFA推导");
    comboFuncBox->addItem("NFA转换为DFA");
    comboFuncBox->addItem("递归下降分析");
    comboFuncBox->addItem("算符优先算法");
    comboFuncBox->addItem("LR识别句子");
    ui.toolBar->addWidget(comboFuncBox);

    /*QPushButton* btn_run = new QPushButton(this);
    QIcon icon(R"(.\icons\run.png)");
    btn_run->setIcon(icon);
    btn_run->setText("运行");
    ui.toolBar->addWidget(btn_run);*/


   /* QList<int> sizes = { 50, INT_MAX };*/
    //ui.splitter->setSizes(sizes);
    ui.splitter->setStretchFactor(1, 2);

    // 设置QTreeWidget的样式表
    ui.treeWidget->setStyleSheet(
        "QTreeWidget::item:selected {"
        "    background-color: #567dbc;"
        "    color: white;"
        "}"
        "QTreeWidget { "
        "    border: 1px solid black; "
        "    background-color: #c8e0df;  "
        "}"
       
    );

    // 加载默认的语法文件
    loadDefalutGrammar();

    
    //connect(ui.actOpenFolder, &QAction::triggered, this, &QtComplierPrinciple::on_actOpenFolder_triggered, Qt::UniqueConnection);
}

QtComplierPrinciple::~QtComplierPrinciple(){

}

void QtComplierPrinciple::on_actNewGrammar_triggered(){
    std::cout << "自动连接\n";
}

void QtComplierPrinciple::on_treeWidget_itemClicked(QTreeWidgetItem* item, int column){
    if (item->parent() == NULL) {
        //std::cout << "不存在父项\n";
        return;
    }
    // 获取文件目录及名称
    /*QString folder = item->parent()->text(0);
    QString name = item->text(0);
    QFileInfo info(folder +"\\" + name);
    std::cout << "文件的绝对路径: " << info.absoluteFilePath().toStdString();*/

}

// 双击事件
void QtComplierPrinciple::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column) {

    // 清空
    ui.plainTextEdit->clear();

    //std::cout << "双击项目\n";
    if (item->parent() == nullptr) {
        return;
    }
    //QString folder = item->parent()->text(0);
    //std::cout << "目录名: " << folder.toStdString() <<std::endl;
    QString name = item->text(0);
    //std::cout << "文件名: " << name.toStdString() << std::endl;
    QFileInfo info(defalutDir+"\\" + name);
    //std::cout << "文件的绝对路径: " << info.absoluteFilePath().toStdString() <<std::endl;

    //QString content = readFile(info.absoluteFilePath());


    std::string filePath;

    filePath = info.absoluteFilePath().toLocal8Bit();
    std::vector<Grammar> grammars;
    PUTIL::readGrammar(grammars, filePath);
    for (Grammar g : grammars) {
        QString s;
        for (Production pro : g.productions) {
            for (std::string p_str : pro.right) {
                s.append(p_str+"|");
            }
            s.append("\n");
        }
        ui.plainTextEdit->appendPlainText(s);
    }

}

void QtComplierPrinciple::on_actOpenFolder_triggered() {
    QString dir = QFileDialog::getExistingDirectory();
    
    if (dir.isEmpty()) {
        return;
    }
    QStringList files = findTxtFiles(dir);
    if (files.isEmpty()) {
        return;
    }

    QTreeWidgetItem* topItem = new QTreeWidgetItem();
    topItem->setIcon(0, QIcon(":/icons/icons/folder.png"));
    QFileInfo info(dir);
    topItem->setText(0, info.dir().dirName());
    for (QString s : files) {
        QTreeWidgetItem* ch = new QTreeWidgetItem(topItem);
        ch->setText(0,s);
        ch->setIcon(0,QIcon(":/icons/icons/file.png"));
        topItem->addChild(ch);
    }

    ui.treeWidget->addTopLevelItem(topItem);

   

}

// 找到所有txt文件
QStringList QtComplierPrinciple::findTxtFiles(const QString& directoryPath) {

    QStringList files;

    QDir dir(directoryPath);
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot); // 设置过滤器只获取文件

    // 获取目录中的所有文件
    QFileInfoList list = dir.entryInfoList();
    for (const QFileInfo& fileInfo : list) {
        if (fileInfo.isFile() && fileInfo.suffix().compare("txt", Qt::CaseInsensitive) == 0) {
            // 如果是文件并且文件后缀名为.txt（不区分大小写）
            files.append(fileInfo.fileName());
            //qDebug() << "Found .txt file:" << fileInfo.absoluteFilePath();
        }
    }
    return files;
}
// 加载默认语法
void QtComplierPrinciple::loadDefalutGrammar() {
    
    if (defalutDir.isEmpty()) {
        return;
    }
    QStringList files = findTxtFiles(defalutDir);
    if (files.isEmpty()) {
        return;
    }

    QTreeWidgetItem* topItem = new QTreeWidgetItem(ui.treeWidget);
    topItem->setIcon(0, QIcon(":/icons/icons/folder.png"));
    QFileInfo info(defalutDir);
    topItem->setText(0, info.dir().dirName());
    for (QString s : files) {
        QTreeWidgetItem* ch = new QTreeWidgetItem(topItem);
        ch->setText(0, s);
        ch->setIcon(0, QIcon(":/icons/icons/file.png"));
        topItem->addChild(ch);
    }
    
    topItem->setExpanded(true);
}

// 读取文件的所有内容
QString QtComplierPrinciple::readFile(const QString& filePath) {
    QString content;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "文件打开失败：" << filePath.toStdString() << std::endl;;
        //qDebug() << "无法打开文件:" << filePath;
        return  QString();
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        content += in.readLine();
        content += '\n';
        //QString line = in.readLine();
        //qDebug() << line;
    }

    file.close();
    return content;
}