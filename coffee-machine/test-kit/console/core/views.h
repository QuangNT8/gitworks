#ifndef CORE_VIEWS_H
#define CORE_VIEWS_H

#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkDiskCache>
#include <QNetworkAccessManager>
#include <QStandardPaths>

#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include "component.h"
#include "operation.h"
#include <QMap>

COMPONENT(core, Views)
    Q_OBJECT
public:
    void init();
    void start();
    void stop();
    void registerComponent(const QString& name, QObject* component);
    void showMessage(QString text);
    void setPath(QString path){this->path_ = path;}
    void setWidth(int width){this->width_ = width;}
    void setHeight(int height){this->height_ = height;}
    void showAppError(QString message, quint32 timeout);
    void showAppError(QString message){showAppError(message,0);}

    Q_INVOKABLE void showView(QString url);
    Q_INVOKABLE void showPage(QString url, QVariant param = QVariant());
    Q_INVOKABLE void hideAppError();
    Q_INVOKABLE void showLoading();
    Q_INVOKABLE void hideLoading();
    Q_INVOKABLE void back();
    Q_INVOKABLE void home();
private:
    QQmlApplicationEngine qmlEngine_;
    QQuickWindow* window_ = nullptr;
    QString path_;
    int width_;
    int height_;

COMPONENT_END

namespace core
{

class NetworkFactory : public QQmlNetworkAccessManagerFactory
{
public:
    QNetworkAccessManager *create(QObject *parent)
    {
        QNetworkAccessManager* manager = new QNetworkAccessManager(parent);
        QNetworkDiskCache* cache = new QNetworkDiskCache(manager);
        cache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
        cache->setMaximumCacheSize(500 * 1024 * 1024); // 500MB
        manager->setCache(cache);
        return manager;
    }
};

}

#endif
