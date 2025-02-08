#include "notepad.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent), currentFile("") { // 初始化 currentFile 为空
    textEdit = new QTextEdit(this);
    setCentralWidget(textEdit);
    createMenu();
    setWindowTitle("Notepad");
    resize(600, 400);
}

void Notepad::newFile() {
    textEdit->clear();
    currentFile = "";  // 清空当前文件路径，表示新建文件
}

void Notepad::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Text Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&file);
            textEdit->setText(in.readAll());
            file.close();
            currentFile = fileName;  // 记录当前打开的文件
        } else {
            QMessageBox::warning(this, "Error", "Cannot open file");
        }
    }
}

void Notepad::saveFile() {
    if (currentFile.isEmpty()) {
        saveFileAs();  // 如果没有文件路径，则执行 "另存为"
    } else {
        QFile file(currentFile);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);
            out << textEdit->toPlainText();
            file.close();
        } else {
            QMessageBox::warning(this, "Error", "Cannot save file");
        }
    }
}

void Notepad::saveFileAs() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "Text Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty()) {
        currentFile = fileName;  // 更新当前文件路径
        saveFile();  // 调用 `saveFile()` 进行保存
    }
}
void Notepad::createMenu() {
    QMenu *fileMenu = menuBar()->addMenu("File"); // 创建 "File" 菜单
    fileMenu->addAction("新建", QKeySequence::New, this, &Notepad::newFile); // 添加 "New" 操作，并绑定快捷键
    fileMenu->addAction("打开", QKeySequence::Open, this, &Notepad::openFile); // 添加 "Open" 操作，并绑定快捷键
    fileMenu->addAction("保存", QKeySequence::Save, this, &Notepad::saveFile); // 添加 "Save" 操作，并绑定快捷键
    fileMenu->addAction("退出", QKeySequence::Quit, this, &QWidget::close); // 添加 "Exit" 操作，并绑定快捷键
    fileMenu->addAction("另存为",QKeySequence("Ctrl+Shift+S"), this, &Notepad::saveFileAs);
}
