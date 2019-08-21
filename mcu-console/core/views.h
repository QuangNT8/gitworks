#ifndef VIEWS_H
#define VIEWS_H

#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkDiskCache>
#include <QNetworkAccessManager>
#include <QStandardPaths>

#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include "component.h"
#include <QMap>


COMPONENT(core, Views)
    Q_OBJECT
public:
    void init();
    void start();
    void stop();
    void registerComponent(const QString& name, QObject* component);
    Q_INVOKABLE void showMessage(QString text);
    void setPath(QString path){this->path_ = path;}
    void setWidth(int width){this->width_ = width;}
    void setHeight(int height){this->height_ = height;}
    QQuickWindow* getWindow(){return window_;}

private:
    QQmlApplicationEngine qmlEngine_;
    QQuickWindow* window_ = nullptr;
    QString path_;
    int width_;
    int height_;

COMPONENT_END

#endif // VIEWS_H
