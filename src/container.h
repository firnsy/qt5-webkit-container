#ifndef CONTAINER_H
#define CONTAINER_H

#include <QtWebKitWidgets>

class Container : public QWebView {
    Q_OBJECT

  public:
    explicit Container(QWidget *parent = 0);

    void load(const QUrl &url);

    void setFixedSize(int w, int h);
    void setScrollBars(bool state);
    void setVerbosity(bool state);

    ~Container();

  private:
    QWidget *splash;
    QUrl    url;
    bool    verbose;

  private slots:
    void loadFinished(bool ok);
    void loadStarted();
    void retry();
};

#endif // CONTAINER_H
