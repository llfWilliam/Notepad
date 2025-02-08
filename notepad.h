#ifndef NOTEPAD_H // 预处理指令，防止头文件被多次包含
#define NOTEPAD_H

#include <QMainWindow> // 引入 QMainWindow 头文件，提供主窗口功能
#include <QTextEdit> // 引入 QTextEdit 头文件，提供文本编辑功能

class Notepad : public QMainWindow { // 定义 Notepad 类，继承自 QMainWindow
    Q_OBJECT // 启用 Qt 的元对象功能

public:
    Notepad(QWidget *parent = nullptr); // 构造函数，定义主窗口

private slots:
    void newFile(); // 槽函数：创建新文件
    void openFile(); // 槽函数：打开文件
    void saveFile(); // 槽函数：保存文件
    void saveFileAs();

private:
    void createMenu();
    QTextEdit *textEdit;
    QString currentFile;
};

#endif // NOTEPAD_H
