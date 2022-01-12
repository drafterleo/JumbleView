#include <QApplication>
#include <QTextStream>
#include <QTime>

#include "cw_mainform.h"

void loadStyle ()
{
    QFile inputFile(":/styles/style.style");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        in.setCodec("UTF-8");
        QString styleStr = in.readAll();
        inputFile.close();

        qApp->setStyleSheet(styleStr);
    }
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    loadStyle();

    qsrand(QTime::currentTime().msec());

    CMainForm *mainForm = new CMainForm;
    mainForm->show();

    return app.exec();
}

//#include "main.moc"
