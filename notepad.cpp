#include "notepad.h" // 引入 Notepad 头文件
#include <QMenuBar> // 引入 QMenuBar 头文件，提供菜单栏功能
#include <QFileDialog> // 引入 QFileDialog 头文件，提供文件对话框功能
#include <QMessageBox> // 引入 QMessageBox 头文件，提供消息框功能
#include <QFile> // 引入 QFile 头文件，提供文件操作功能
#include <QTextStream> // 引入 QTextStream 头文件，提供文本流功能

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent) { // 构造函数，初始化 Notepad 窗口
    textEdit = new QTextEdit(this); // 创建 QTextEdit 实例，并设置为主窗口的中心部件
    setCentralWidget(textEdit); // 设置 QTextEdit 为窗口的中央组件
    createMenu(); // 调用 createMenu 函数创建菜单栏
    setWindowTitle("Notepad"); // 设置窗口标题
    resize(600, 400); // 设置窗口大小
}

void Notepad::newFile() {
    textEdit->clear(); // 清空文本编辑区域
}

void Notepad::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Text Files (*.txt);;All Files (*)"); // 显示打开文件对话框
    if (!fileName.isEmpty()) { // 检查文件名是否为空
        QFile file(fileName); // 创建 QFile 实例，关联文件路径
        if (file.open(QFile::ReadOnly | QFile::Text)) { // 以只读文本模式打开文件
            QTextStream in(&file); // 创建文本流读取文件内容
            textEdit->setText(in.readAll()); // 将文件内容加载到文本编辑器
            file.close(); // 关闭文件
        } else {
            QMessageBox::warning(this, "Error", "Cannot open file"); // 显示错误消息
        }
    }
}

void Notepad::saveFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "Text Files (*.txt);;All Files (*)"); // 显示保存文件对话框
    if (!fileName.isEmpty()) { // 检查文件名是否为空
        QFile file(fileName); // 创建 QFile 实例，关联文件路径
        if (file.open(QFile::WriteOnly | QFile::Text)) { // 以写入文本模式打开文件
            QTextStream out(&file); // 创建文本流写入文件内容
            out << textEdit->toPlainText(); // 获取文本编辑器内容并写入文件
            file.close(); // 关闭文件
        } else {
            QMessageBox::warning(this, "Error", "Cannot save file"); // 显示错误消息
        }
    }
}

void Notepad::createMenu() {
    QMenu *fileMenu = menuBar()->addMenu("File"); // 创建 "File" 菜单
    fileMenu->addAction("New", QKeySequence::New, this, &Notepad::newFile); // 添加 "New" 操作，并绑定快捷键
    fileMenu->addAction("Open", QKeySequence::Open, this, &Notepad::openFile); // 添加 "Open" 操作，并绑定快捷键
    fileMenu->addAction("Save", QKeySequence::Save, this, &Notepad::saveFile); // 添加 "Save" 操作，并绑定快捷键
    fileMenu->addAction("Exit", QKeySequence::Quit, this, &QWidget::close); // 添加 "Exit" 操作，并绑定快捷键
}
