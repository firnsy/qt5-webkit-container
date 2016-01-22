#include "container.h"
#include <stdio.h>

//#include <QScroller>

Container::Container(QWidget *parent) : QWebView(parent) {
  connect(this, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
  connect(this, SIGNAL(loadStarted()), this, SLOT(loadStarted()));

  // enable kinetic scrolling
  //QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);

  // build splash screen
  splash = new QWidget(this);
  splash->setStyleSheet("background-color: #000");
  splash->setFixedSize(this->size());

  // no borders
  this->setWindowFlags(Qt::FramelessWindowHint);

  this->setScrollBars(false);
}

void Container::load(const QUrl &url) {
  this->url = url;
  QWebView::load(this->url);
}

void Container::setFixedSize(int w, int h) {
  if (verbose) {
    fprintf(stderr, "Setting container size: %d x %d\n", w, h);
  }

  QWebView::setFixedSize(w, h);

  // adjust splash appropriately
  splash->setFixedSize(w, h);
}

void Container::setScrollBars(bool state) {
  // show scrollbars as needed
  if (state) {
    this->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical,   Qt::ScrollBarAsNeeded);
    this->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAsNeeded);
  }
  // disable scrollbars
  else {
    this->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical,   Qt::ScrollBarAlwaysOff);
    this->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
  }
}

void Container::setVerbosity(bool state) {
  verbose = state;
}

// PRIVATE SLOTS
void Container::loadFinished(bool ok) {
  if (ok) {
    splash->hide();

    quint64 bytes_received = page()->bytesReceived();

    if (verbose) {
      fprintf(stderr, "Loaded (%llu bytes).\n", bytes_received);
    }
  }
  else {
    QTimer::singleShot(2000, this, SLOT(retry()));
  }
}

void Container::loadStarted() {
  if (verbose) {
    fprintf(stderr, "Loading: %s\n", (const char *)this->url.toString().toLatin1());
  }
}

void Container::retry() {
  if (verbose) {
    fprintf(stderr, "Retrying ...\n");
  }

  QWebView::load(this->url);
}

Container::~Container() {
  delete splash;
}
