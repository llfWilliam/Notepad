#include <QApplication> // 引入 QApplication 头文件，管理应用程序的事件循环
#include "notepad.h" // 引入 Notepad 类的头文件

int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // 创建 QApplication 实例，管理 Qt 应用程序的生命周期
    Notepad notepad; // 创建 Notepad 主窗口实例
    notepad.show(); // 显示主窗口
    return app.exec(); // 进入应用程序事件循环
}
