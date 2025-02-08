#include "notepad.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QStyle>

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent), currentFile("") { // 初始化 currentFile 为空
    textEdit = new QTextEdit(this);
    setCentralWidget(textEdit);
    createMenu();
    setWindowTitle("Notepad");
    resize(600, 400);
}

void Notepad::newFile() {
    // 检查当前文档是否已修改
    if (textEdit->document()->isModified()) {
        // 提示用户保存当前未保存的更改
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "保存更改",
                                      "当前文档有未保存的更改。是否保存？",
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply == QMessageBox::Yes) {
            // 用户选择保存，调用保存函数
            if (!saveFile()) {
                // 如果保存失败，返回，不创建新文件
                return;
            }
        } else if (reply == QMessageBox::Cancel) {
            // 用户选择取消，返回，不创建新文件
            return;
        }
    }

    // 提示用户选择新文件的保存路径
    QString fileName = QFileDialog::getSaveFileName(this, "新建文件", "",
                                                    "文本文件 (*.txt);;所有文件 (*)");
    if (fileName.isEmpty()) {
        // 用户取消了文件选择，返回
        return;
    }

    // 清空文本编辑器
    textEdit->clear();

    // 更新当前文件路径
    currentFile = fileName;
    setWindowTitle(QFileInfo(currentFile).fileName() + " - 记事本");

    // 保存新文件
    saveFile();
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

bool Notepad::saveFile() {
    if (currentFile.isEmpty()) {
        return saveFileAs();
    } else {
        QFile file(currentFile);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "Error", "Cannot save file: " + file.errorString());
            return false;
        }
        QTextStream out(&file);
        out << textEdit->toPlainText();
        file.close();
        textEdit->document()->setModified(false);
        return true;
    }
}
bool Notepad::saveFileAs() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) {
        return false; // 用户取消了保存操作
    }
    currentFile = fileName;
    return saveFile();
}
void Notepad::toggleNightMode()
{
    static bool nightModeEnabled = false;
    nightModeEnabled = !nightModeEnabled;
    if (nightModeEnabled) {
        qApp->setStyleSheet(R"(
            QMenuBar {
                background-color: #353535;
                color: #ffffff;
            }
            QMenuBar::item {
                background-color: #353535;
                color: #ffffff;
            }
            QMenuBar::item::selected {
                background-color: #454545;
            }
            QMenu {
                background-color: #353535;
                color: #ffffff;
            }
            QMenu::item {
                background-color: #353535;
                color: #ffffff;
            }
            QMenu::item::selected {
                background-color: #454545;
            }
            QMessageBox {
                background-color: #2d2d2d;
                color: #ffffff;
            }
            QMessageBox QLabel {
                color: #ffffff;
            }
            QMessageBox QPushButton {
                background-color: #555555;
                color: #ffffff;
                border: none;
                padding: 5px 10px;
            }
            QMessageBox QPushButton:hover {
                background-color: #777777;
            }
            QMessageBox QPushButton:pressed {
                background-color: #999999;
            }
            )");
    } else {
        qApp->setStyleSheet("");
    }

    QPalette palette;
    if (nightModeEnabled) {
        palette.setColor(QPalette::Window, QColor(53, 53, 53));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(42, 42, 42));
        palette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(53, 53, 53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, QColor(42, 130, 218));
        palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        palette.setColor(QPalette::HighlightedText, Qt::black);
        palette.setColor(QPalette::ButtonText, Qt::white); // 菜单文本颜色
        palette.setColor(QPalette::Text, Qt::white);
        textEdit->setStyleSheet("QTextEdit { background-color: #2b2b2b; color: #ffffff; }");
    } else {
        palette = QApplication::style()->standardPalette();
        textEdit->setStyleSheet("");
    }
    QApplication::setPalette(palette);
}
void Notepad::createMenu() {
    QMenu *fileMenu = menuBar()->addMenu("菜单"); // 创建 "File" 菜单
    fileMenu->addAction("新建", QKeySequence::New, this, &Notepad::newFile); // 添加 "New" 操作，并绑定快捷键
    fileMenu->addAction("打开", QKeySequence::Open, this, &Notepad::openFile); // 添加 "Open" 操作，并绑定快捷键
    fileMenu->addAction("保存", QKeySequence::Save, this, &Notepad::saveFile); // 添加 保存 操作，并绑定快捷键
    fileMenu->addAction("另存为",QKeySequence("Ctrl+Shift+S"), this, &Notepad::saveFileAs); //添加另存为操作
    fileMenu->addAction("退出", QKeySequence::Quit, this, &QWidget::close); // 添加 "Exit" 操作，并绑定快捷键

    QMenu *editMenu = menuBar()->addMenu("编辑");
    editMenu->addAction("撤销", QKeySequence::Undo, textEdit, &QTextEdit::undo);
    editMenu->addAction("恢复", QKeySequence::Redo, textEdit, &QTextEdit::redo);

    QMenu *viewMenu = menuBar()->addMenu("主题");
    QAction *toggleNightModeAction = viewMenu->addAction("黑夜模式", this, &Notepad::toggleNightMode);
    toggleNightModeAction->setCheckable(true);
}
