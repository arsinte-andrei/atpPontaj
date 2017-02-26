#include "mainwindow.h"
#include <QApplication>

void formatMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug(%s:%u, %s) : %s\n", context.file, context.line, context.function, localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning(%s:%u, %s) : %s\n", context.file, context.line, context.function, localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical(%s:%u, %s) : %s\n", context.file, context.line, context.function, localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal(%s:%u, %s) : %s\n", context.file, context.line, context.function, localMsg.constData());
        abort();
    }
}

int main(int argc, char *argv[]) {

    qInstallMessageHandler(formatMessageOutput);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
