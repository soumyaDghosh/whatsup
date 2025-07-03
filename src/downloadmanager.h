#include <QFileDialog>
#include <QWebEngineDownloadRequest>
#include <QtQml>

class DownloadManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    Q_INVOKABLE void downloadFile(QWebEngineDownloadRequest *request);
    static DownloadManager *self();
};
