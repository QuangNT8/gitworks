#include <QQuickItem>
#include <QScreen>
#include <QGuiApplication>
#include <QGuiApplication>
#include <QQmlContext>
#include "views.h"
#include <QDateTime>

void core::Views::init()
{
    registerComponent("Views", this);
}

void core::Views::start()
{
    qmlEngine_.load(QUrl(path_));
    window_ = qobject_cast<QQuickWindow*>(qmlEngine_.rootObjects().at(0));
#ifdef Q_OS_ANDROID
    _window->showMaximized();
#elif defined(Q_OS_IOS)
    _window->showMaximized();
#else
    window_->setHeight(height_);
    window_->setWidth(width_);
    window_->setX((QGuiApplication::primaryScreen()->size().width() - width_)/2);
    window_->setY((QGuiApplication::primaryScreen()->size().height() - height_)/2);
    window_->show();
#endif
}

void core::Views::stop()
{
}

void core::Views::registerComponent(const QString& name, QObject* component)
{
    qmlEngine_.rootContext()->setContextProperty(name,component);
}

void core::Views::showMessage(QString text)
{
    QDateTime now = QDateTime::currentDateTime();
    QMetaObject::invokeMethod(window_, "showMessage", Q_ARG(QVariant, now.toString("hh:mm:ss") + ": " + text));
}
