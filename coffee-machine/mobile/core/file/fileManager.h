#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "../component.h"
#include <QStandardPaths>
#include "fileOperations.h"
#include <QFile>
#include "core/views.h"
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif


COMPONENT(file, Manager)
    Q_OBJECT
public:
    void init()
    {
        core::Views::instance()->registerComponent("file", this);
        REGISTRATION(file);
    }

    Q_INVOKABLE QString location()
    {
#ifdef Q_OS_IOS
        return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#elif defined(Q_OS_ANDROID)
        auto checkPermission = QtAndroid::checkPermission(QString("android.permission.WRITE_EXTERNAL_STORAGE"));
        if(checkPermission == QtAndroid::PermissionResult::Denied)
        {
            QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({"android.permission.WRITE_EXTERNAL_STORAGE"}));
            if(resultHash["android.permission.WRITE_EXTERNAL_STORAGE"] == QtAndroid::PermissionResult::Granted)
                qDebug() << "Temporary file requires permission to write";
        }
        return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#else
        return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif
    }


    Q_INVOKABLE QString getFilePath(QString name)
    {
        auto filepath = location()+"/"+name;
        auto file_existed = QFile::exists(filepath);
        if(file_existed)
        {
            QFile f(filepath);
            if(f.size() == 0)
            {
                QFile::remove(filepath);
                file_existed = false;
            }
            f.close();
        }
        if (file_existed)
        {
            return filepath;
        }
        return QString("");
    }
COMPONENT_END

#endif // FILEMANAGER_H
