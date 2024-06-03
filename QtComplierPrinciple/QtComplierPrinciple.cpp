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
#include <qitemselectionmodel.h>
#include "GrammarLib/OPGUtil.h"
#include <GrammarLib/FAUtil.h>


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
    comboFuncBox = new QComboBox(this);
    comboFuncBox->setMinimumWidth(150);
    comboFuncBox->setMinimumHeight(40);
    QFont font = comboFuncBox->font();
    font.setPointSize(14);
    comboFuncBox->setFont(font);
    comboFuncBox->addItem("加标记法处理");    // 0
    comboFuncBox->addItem("LL1自顶向下推导"); // 1
    comboFuncBox->addItem("消除左递归");      // 2
    comboFuncBox->addItem("DFA推导");         // 3
    comboFuncBox->addItem("NFA转换为DFA");    // 4
    comboFuncBox->addItem("递归下降分析");    // 5
    comboFuncBox->addItem("算符优先算法识别句子");// 6
    comboFuncBox->addItem("算符优先符表转化为FG优先函数(Warshall)");//7
    comboFuncBox->addItem("LR识别句子");// 8
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
    // 设置QLineEdit的样式表
    ui.lineEdit->setStyleSheet(
        "QLineEdit { border: 1px solid black }"
    );

    //设置GroupBox
    ui.groupBox->setStyleSheet("QGroupBox { padding: 0px; margin: 0px; }");
    ui.groupBox_4->setStyleSheet("QGroupBox { padding: 0px; margin: 0px; }");
    ui.groupBox_5->setStyleSheet("QGroupBox { padding: 0px; margin: 0px; }");




    // 加载默认的语法文件
    loadDefalutGrammar();

    
    //connect(ui.actOpenFolder, &QAction::triggered, this, &QtComplierPrinciple::on_actOpenFolder_triggered, Qt::UniqueConnection);
    //connect(ui.checkBox, &QCheckBox::checkStateChanged, this, &QtComplierPrinciple::on_checkBox_checkStateChanged);
        
}
// 文法可编辑状态改变
void QtComplierPrinciple::on_checkBox_checkStateChanged(Qt::CheckState checked) {
    //ui.textEdit->setEnabled(checked);
    ui.textEdit->setReadOnly(!checked);
    //ui.actSaveGrammar->setEnabled(!checked);
}
// 文法内容改变
void QtComplierPrinciple::on_textEdit_textChanged(){
    // 判断可编辑选项是否选中，如果未选中说明是第一次加载文法，无需开启save动作
    if (ui.checkBox->isChecked()) {
        ui.actSaveGrammar->setEnabled(true);
        return;
    }
    ui.actSaveGrammar->setEnabled(false);
}
// 保存文法点击
void QtComplierPrinciple::on_actSaveGrammar_triggered(){
    callCmd("文法保存功能待实现....");
    ui.actSaveGrammar->setEnabled(false);
}

// 删除
void QtComplierPrinciple::on_actDeleteGrammar_triggered(){
    callCmd("文法删除功能待实现....");
    //ui.listWidget->removeItemWidget(ui.listWidget->currentItem());
    if (ui.listWidget->selectedItems().size() == ui.listWidget->count()) {// 全选
        for (int i = ui.listWidget->count()-1; i >= 0; --i) {
            delete ui.listWidget->takeItem(i);
            
        }
        ui.textEdit->clear();
        return;
    }
    delete ui.listWidget->takeItem(ui.listWidget->currentRow());
    ui.listWidget->reset();
    //ui.listWidget->setCurrentRow(0);
    ui.textEdit->clear();
    ui.checkBox->setChecked(false);
    ui.checkBox->setEnabled(false);
   
}
// 全选
void QtComplierPrinciple::on_actSelectAllGrammar_triggered(){
    callCmd("文法全选功能");
    ui.actDeleteGrammar->setEnabled(true);

    QItemSelectionModel* model = ui.listWidget->selectionModel();
    model->select(
        QItemSelection(
            ui.listWidget->model()->index(0, 0), ui.listWidget->model()->index(ui.listWidget->count() - 1, 0)
        )
        , QItemSelectionModel::Select
    );
}

void QtComplierPrinciple::on_actRun_triggered(){
    switch ( comboFuncBox->currentIndex() ){
    case 0:
        qf0();
        break;
    case 1:
        qf1();
        Callback::myCallbackFunction();
        break;
    case 2:
        qf2();
        break;
    case 3:
        qf3();
        break;
    case 4:
        qf4();
        break;
    case 5:
        qf5();
        break;
    case 6:
        qf6();
        break;
    case 7:
        qf7();
        break;
    case 8:
        break;
    default:
        break;
    }
    
}

void startQTable() {
    system("\"D:\\Code\\CCode\\VS\\QtProject\\OPG\\x64\\Debug\\OPG.exe\"");
}

void QtComplierPrinciple::qf0(){
    Grammar& g = grammars[ui.listWidget->currentRow()];
    PUTIL::labelMethod(g.productions);
    PUTIL::fixNumber(g.productions);
    ui.textEdit->setText(getQStringFromGrammar(g));
    callCmd("加标记法处理.......");
}

void QtComplierPrinciple::qf1() {
    // 获取文法
    Grammar& g = grammars[ui.listWidget->currentRow()];
    // 打印文法
    cmdVStr.clear();
    cmdVStr.push_back(getQStringFromGrammar(g).toStdString());
    // 将终结符转换为字符串，历史遗留问题：没有时间实现类似 if 是终结符的功能来识别句子
    vector<string> terms;
    for (set<char>::iterator it = g.terms.begin(); it != g.terms.end(); it++) {
        string t = "";
        t += *it;
        terms.push_back(t);
    }
    terms.push_back("$");
    vector<ProductionFirst> vpf;
    // 获取LL1Table表
    cmdVStr.push_back("打开 LL1语法分析表 GUI");
    map<LL1Key, string> LL1Table = PUTIL::getLL1Table(vpf, g.productions, terms);

    FILE* fp;
    fopen_s(&fp, toQtFilePath.c_str(), "w");
    if (fp == NULL) {
        cout << "\n文件问找到！\n";
        return;
    }
    fprintf(fp, "\n");

    // 打印到文件中
    fprintf(fp, "$");
    for (string item : terms) {
        fprintf(fp, "\t%s", item.c_str());
    }
    fprintf(fp, "\n");
    for (int i = 0; i < g.productions.size(); i++) {
        fprintf(fp, "%s", g.productions[i].left.c_str());
        for (int j = 0; j < terms.size(); j++) {
            map<LL1Key, string>::iterator it = LL1Table.find(LL1Key(g.productions[i].left, terms[j][0]));
            if (it != LL1Table.end()) {
                fprintf(fp, "\t%s", it->second.c_str());
            }
            else {
                fprintf(fp, "\t$");
            }
        }
        fprintf(fp, "\n");
    }

    PUTIL::printLL1Table(LL1Table, g.productions, terms);
    string juzi = ui.lineEdit->text().toStdString();
    if (juzi == "@") {
        return;
    }
    string ret = PUTIL::topToBottom(juzi, LL1Table, g.productions, terms);
    // 展示LL1分析表GUI
    fprintf(fp, "%s", ret.c_str());
    fclose(fp);
    std::thread t1(startQTable);
    t1.detach();

}
void QtComplierPrinciple::qf2() {
    Grammar& g = grammars[ui.listWidget->currentRow()];
    callCmd("消除左递归处理中");
    PUTIL::EliminateLeftRecursion(g.productions);
    PUTIL::fixNumber(g.productions);
    ui.textEdit->setText(getQStringFromGrammar(g));
}
void QtComplierPrinciple::qf3() {
    /*while (1) {
        FAUtil::printDFAs(dfas);
        cout << "请输入<DFA编号,字符>进行操作，输入-1退出程序" << endl;
        int opID = -1;
        string str;
        cin >> opID;
        cout << "opID: " << opID;
        if (opID == -1 || opID < 0 || opID > dfas.size()) {
            break;
        }
        else {
            cin >> str;
            FAUtil::DFADerivedSentence(dfas[opID], str);
        }

    }*/
}
void QtComplierPrinciple::qf4() {

}
void QtComplierPrinciple::qf5() {

}

void QtComplierPrinciple::qf6() {
    Grammar& grammar = grammars[ui.listWidget->currentRow()];
    cmdVStr.clear();
    cmdVStr.push_back("----------------求解FIRSTTERM中------------------");
    vector<FirstTerm> first = OPGUtil::getFirstTerm(grammar);
    cmdVStr.push_back("----------------求解LASTTERM中------------------");
    vector<LastTerm> last = OPGUtil::getLastTerm(grammar);
    cmdVStr.push_back("----------------求解算符优先表中------------------");
    map<pair<char, char>, char> table = OPGUtil::getTable(grammar, first, last);
    map<pair<char, char>, char> noTable = OPGUtil::getTableNo(grammar, first, last);
    cmdVStr.push_back("算符优先矩阵：");
    OPGUtil::printOPGTable(grammar, table);
    map<string, char> proToLeftTable = OPGUtil::getProToLeftTable(grammar);
    grammar.terms.insert('#');
    string juzi = ui.lineEdit->text().toStdString();
    juzi += "#";
    if (juzi == "#") {
        return;
    }
    if (OPGUtil::isSentence(grammar.start, juzi, table, proToLeftTable) == ISSENTENCE) {
        cout << "这是个句子\n";
        std::thread t1(startQTable);
        t1.detach();
        /*system("\"D:\\Code\\CCode\\VS\\QtProject\\OPG\\x64\\Debug\\OPG.exe\"");*/
    }
    else {
        cout << "这不是个句子\n";
        std::thread t1(startQTable);
        t1.detach();
        /*system("\"D:\\Code\\CCode\\VS\\QtProject\\OPG\\x64\\Debug\\OPG.exe\"");*/
    }
}
void QtComplierPrinciple::qf7() {

}

QtComplierPrinciple::~QtComplierPrinciple(){}
// 新增文法文件
void QtComplierPrinciple::on_actNewGrammarFile_triggered(){
    std::cout << "自动连接\n";
}
// treeItem的单击事件
void QtComplierPrinciple::on_treeWidget_itemClicked(QTreeWidgetItem* item, int column){
  
}

// 文法文件的双击事件
void QtComplierPrinciple::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column) {
    // 清空ListWidget
    ui.listWidget->clear();
    // cmd打印缓冲区
    cmdVStr.clear();
    // 如果是目录文件，无操作
    if (item->parent() == nullptr) {
        ui.toolBar_2->setEnabled(false);
        return;
    }
    // 打开文法操作
    ui.toolBar_2->setEnabled(true);
    ui.actAddGrammar->setEnabled(true);
    ui.actDeleteGrammar->setEnabled(false);
    ui.actSaveGrammar->setEnabled(false);
    ui.actSelectAllGrammar->setEnabled(true);
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
    grammars = std::vector<Grammar>();
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
// 回调函数
void QtComplierPrinciple::callCmd(std::string s) {
    std::cout << "\n🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼<界面操作内容>🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼\n\n😀\t" << s << 
                 "\n\n🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼🌼\n";
    std::thread t(Callback::myCallbackFunction);
    t.detach();
}
// 回调函数
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
    // 开启文法删除操作
    ui.actDeleteGrammar->setEnabled(true);
    ui.checkBox->setEnabled(true);
    ui.checkBox->setChecked(false);
    ui.textEdit->setEnabled(true);
    ui.textEdit->setReadOnly(true);
    ui.actRun->setEnabled(true);
    // 关闭英文新添加文法导致开启了save动作
    //ui.actSaveGrammar->setEnabled(false);

    // 获取行
    int row = ui.listWidget->row(item);
    // 加载到当前项中
    QString strG = getQStringFromGrammar(grammars[row]);
    ui.textEdit->setPlainText(strG);
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
