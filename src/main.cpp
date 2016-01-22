#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>

#include "container.h"

#define DEFAULT_NAME     "webkit-ui"
#define DEFAULT_VERSION  "0.2"

#define DEFAULT_URL      "http://localhost:3000"

#define DEFAULT_WIDTH    800
#define DEFAULT_HEIGHT   480


int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  a.setApplicationName(DEFAULT_NAME);
  a.setApplicationVersion(DEFAULT_VERSION);

  // process commandline
  QCommandLineParser parser;
  parser.setApplicationDescription(DEFAULT_NAME);
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption verboseOption("verbose",
      QCoreApplication::translate("main", "Show verbose messaging."));

  parser.addOption(verboseOption);

  // add container size parsing
  QCommandLineOption widthOption("width",
      QCoreApplication::translate("main", "Set <width> of container."),
      QCoreApplication::translate("main", "width"));
  parser.addOption(widthOption);

  QCommandLineOption heightOption("height",
      QCoreApplication::translate("main", "Set <height> of container."),
      QCoreApplication::translate("main", "height"));
  parser.addOption(heightOption);

  // add cursor option parsing
  QCommandLineOption cursorOption(QStringList() << "c" << "no-cursor",
      QCoreApplication::translate("main", "Hide cursor."));
  parser.addOption(cursorOption);

  // add url option parsing
  QCommandLineOption urlOption(QStringList() << "u" << "url",
      QCoreApplication::translate("main", "Load <url> on startup."),
      QCoreApplication::translate("main", "url"));
  parser.addOption(urlOption);

  // process the actual command line arguments given by the user
  parser.process(a);

  // process container size
  int width = parser.value(widthOption).toInt();
  int height = parser.value(heightOption).toInt();

  if (width <= 0) {
    width = DEFAULT_WIDTH;
  }

  if (height <= 0) {
    height = DEFAULT_HEIGHT;
  }

  // hide cursor as appropriate
  if (parser.isSet(cursorOption)) {
    QCursor cursor(Qt::BlankCursor);
    a.setOverrideCursor(cursor);
    a.changeOverrideCursor(cursor);
  }

  // process url
  QString url = parser.value(urlOption);
  if (url.length() == 0) {
    url = DEFAULT_URL;
  }

  // build container
  Container c;

  c.setVerbosity(parser.isSet(verboseOption));
  c.setFixedSize(width, height);
  c.load(QUrl(url));
  c.show();

  return a.exec();
}
