#include <QApplication>
#include "audiowidget.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationVersion(qVersion());
    QGuiApplication::setApplicationDisplayName(AudioWidget::tr("Spatial Audio Test Application"));

    QCommandLineParser commandLineParser;
    commandLineParser.addVersionOption();
    commandLineParser.addHelpOption();
    commandLineParser.addPositionalArgument("Audio File", "Audio File to play");

    commandLineParser.process(app);

    AudioWidget w;
    w.show();

    if (!commandLineParser.positionalArguments().isEmpty())
        w.setFile(commandLineParser.positionalArguments().constFirst());

    return app.exec();
}