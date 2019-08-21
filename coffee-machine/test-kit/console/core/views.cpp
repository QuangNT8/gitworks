#include <QQuickItem>
#include <QScreen>
#include <QGuiApplication>
#include <QGuiApplication>
#include <QQmlContext>
#include "views.h"
#include <QDateTime>

void core::Views::init()
{
    qmlEngine_.setNetworkAccessManagerFactory(new NetworkFactory());
    registerComponent("Views", this);

    this->setPath("qrc:/main.qml");
    this->setWidth(1366);
    this->setHeight(768);
}

void core::Views::start()
{
    qmlEngine_.load(QUrl(path_));
    window_ = qobject_cast<QQuickWindow*>(qmlEngine_.rootObjects().at(0));
    #ifdef Q_OS_ANDROID
        window_->showMaximized();
    #elif defined(Q_OS_IOS)
        window_->showMaximized();
    #else
        window_->setHeight(height_);
        window_->setWidth(width_);
        window_->setX((QGuiApplication::primaryScreen()->size().width() - window_->width())/2);
        window_->setY((QGuiApplication::primaryScreen()->size().height() - window_->height())/2);
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
    QMetaObject::invokeMethod(window_, "showMessage", Q_ARG(QVariant, now.toString("dd.MM.yyyy hh:mm:ss") + ": " + text));
}

void core::Views::showView(QString url)
{
    QMetaObject::invokeMethod(window_, "showView", Q_ARG(QVariant, url));
}

void core::Views::showPage(QString url, QVariant param)
{
    QMetaObject::invokeMethod(window_, "showPage", Q_ARG(QVariant, url), Q_ARG(QVariant, param));
}

void core::Views::showAppError(QString message, quint32 timeout)
{
    QMetaObject::invokeMethod(window_, "showAppError", Q_ARG(QVariant, message), Q_ARG(QVariant, timeout));
}

void core::Views::hideAppError()
{
    QMetaObject::invokeMethod(window_, "hideAppError");
}

void core::Views::back()
{
    QMetaObject::invokeMethod(window_, "back");
}

void core::Views::home()
{
    QMetaObject::invokeMethod(window_, "home");
}

void core::Views::showLoading()
{
    QMetaObject::invokeMethod(window_, "showLoading");
}

void core::Views::hideLoading()
{
    QMetaObject::invokeMethod(window_, "hideLoading");
}

