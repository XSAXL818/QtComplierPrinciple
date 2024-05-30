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
#include <Callback.h>
#include <thread>

QtComplierPrinciple::QtComplierPrinciple(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this); 
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
    // 设置QListWidget的样式表
    ui.listWidget->setStyleSheet(
        "QListWidget::item:selected {"
        "    background-color: #567dbc;"
        "    color: white;"
        "}"
        "QListWidget::item {"
        "   border: 1px solid black"
        "   font"
        "}"
        "QListWidget     {"
        "    border: 1px solid black;"
        //"    background-color: #c8e0df;"
        "}"
    );

    //设置GroupBox
    ui.groupBox->setStyleSheet("QGroupBox { padding: 0px; margin: 0px; }");
    ui.groupBox_4->setStyleSheet("QGroupBox { padding: 0px; margin: 0px; }");
    ui.groupBox_5->setStyleSheet("QGroupBox { padding: 0px; margin: 0px; }");




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

// 文法文件的双击事件
void QtComplierPrinciple::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column) {
    // cmd打印缓冲区
    cmdVStr.clear();
    // 如果是目录文件，无操作
    if (item->parent() == nullptr) {
        return;
    }
    // 获取所选中的文件名
    QString name = item->text(0);
    // 获取文件所在的绝对路径
    QFileInfo info(defalutDir+"\\" + name);
    // 如果文件名为中文，转换为系统本地的编码的绝对路径
    std::string filePath;
    filePath = info.absoluteFilePath().toLocal8Bit();
    // 读取文件的所有文法
    cmdStr = "获取文件: " + name.toStdString() + "的所有文法";
    //cmdVStr.push_back("获取文件: " + name.toStdString() + "的所有文法");
    PUTIL::readGrammar(grammars, filePath);
    // 加载到listWidget中
    for (Grammar g : grammars) {
        // 获取转换的字符串
        QString s = getQStringFromGrammar(g);
        // 设置item
        QListWidgetItem* item = new QListWidgetItem(s);
        item->setIcon(QIcon("icons/dragon.png"));
        ui.listWidget->addItem(item);
    }
    // 输出到命令行所做的操作
    callCmd(cmdStr);
}
void QtComplierPrinciple::callCmd(std::string s) {
    std::cout << "\n🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼<界面操作内容>🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼\n\n😀\t" << s << 
                 "\n\n🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼\n";
    std::thread t(Callback::myCallbackFunction);
    t.detach();
}

void QtComplierPrinciple::callCmd(std::vector<std::string>& vs) {
    std::cout << "\n🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼<界面操作内容>🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼\n\n";
    for (std::string s : vs) {
        std::cout << "😀\t" << s << std::endl;
    }
    std::cout << "\n🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼\n";
    std::thread t(Callback::myCallbackFunction);
    t.detach();
}

// listItem的单击事件：选中某项，将其放入到当前项中
void QtComplierPrinciple::on_listWidget_itemClicked(QListWidgetItem* item){
    // 获取行
    int row = ui.listWidget->row(item);
    // 加载到当前项中
    QString strG = getQStringFromGrammar(grammars[row]);
    ui.plainTextEdit->setPlainText(strG);
    // 打印操作信息到cmd
    cmdVStr.clear();
    Grammar& g = grammars[row];
    cmdVStr.push_back("选中文法G=" + string("(") + g.start + ")");
    std::string info = "非终结符集={";
    bool isFirst = true;
    for (std::string s : g.nons) {
        if (isFirst) {
            isFirst = false;
            info += s;
        }
        else {
            info += ","+ s;
        }
    }
    info += "}";
    cmdVStr.push_back(info);

    info = "终结符集={";
    isFirst = true;
    for (char c : g.terms) {
        if (isFirst) {
            isFirst = false;
            info += string("")+c;
        }
        else {
            info += string(",") + c;
        }
    }
    info += "}";
    cmdVStr.push_back(info);
    cmdVStr.push_back("规则集如下:");
    std::vector<std::string> gvs = getVStringFromGrammar(g);
    cmdVStr.insert(cmdVStr.end(), gvs.begin(), gvs.end());
    callCmd(cmdVStr);
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
    // cmd打印内容缓冲区清空
    std::vector<std::string> cmdVStr;
    // 判断文件目录是否存在
    if (defalutDir.isEmpty()) {
        cmdVStr.push_back("默认目录: " + defalutDir.toStdString() + " 不存在");
        return;
    }
    cmdVStr.push_back("找到默认目录: " + defalutDir.toStdString());
    // 获取目录下的所有文件
    QStringList files = findTxtFiles(defalutDir);
    if (files.isEmpty()) {// 如果为空
        cmdVStr.push_back("默认目录下不存在文法文件");
        return;
    }
    // 字体
    QFont font;
    font.setPointSize(15);
    // 获取文件并加入到treeWidget中
    // 第一层：Grammar目录
    QTreeWidgetItem* topItem = new QTreeWidgetItem(ui.treeWidget);
    topItem->setIcon(0, QIcon(":/icons/icons/folder.png"));
    topItem->setFont(0,font);
    // 获取所有文件名
    QFileInfo info(defalutDir);
    topItem->setText(0, info.dir().dirName());
    for (QString s : files) {// 加入到第一目录中
        QTreeWidgetItem* ch = new QTreeWidgetItem(topItem);
        ch->setFont(0,font);
        ch->setText(0, s);
        ch->setIcon(0, QIcon(":/icons/icons/file.png"));
        topItem->addChild(ch);
        cmdVStr.push_back("读取文法文件: " + s.toStdString());
    }
    topItem->setExpanded(true);
    callCmd(cmdVStr);
}

// 读取文件的所有内容
QString QtComplierPrinciple::readFile(const QString& filePath) {

    QString content;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cmdStr = "文件打开失败：" + filePath.toStdString() + "\n";
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

QString QtComplierPrinciple::getQStringFromGrammar(Grammar g){
    QString str;

    int cnt = 0;
    for (Production pro : g.productions) {
        str += QString("%1 %2 ::= ")
            .arg(++cnt)
            .arg(QString::fromStdString(pro.left))
            ;
        bool isFirst = true;
        for (std::string s : pro.right) {
            if (isFirst) {
                isFirst = false;
                str += QString::fromStdString(s);
            }
            else {
                str += "|" + QString::fromStdString(s);
            }
        }
        str += "\n";
    }
    return str;
}

std::vector<std::string> QtComplierPrinciple::getVStringFromGrammar(Grammar g){
    std::vector<std::string> vs;
    int cnt = 0;
    for (Production pro : g.productions) {
        QString str;
        str += QString("%1 %2 ::= ")
            .arg(++cnt)
            .arg(QString::fromStdString(pro.left))
            ;
        bool isFirst = true;
        for (std::string s : pro.right) {
            if (isFirst) {
                isFirst = false;
                str += QString::fromStdString(s);
            }
            else {
                str += "|" + QString::fromStdString(s);
            }
        }
        vs.push_back(str.toStdString());
    }
    return vs;
   
}
