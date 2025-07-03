#include <QFileDialog>
#include <QWebEngineDownloadRequest>
#include <QtQml>

class DownloadManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    Q_SLOT void downloadFile(QWebEngineDownloadRequest *request);
    static DownloadManager *self();

private:
    static const QPair<QString, QString> getDownloadPath(const QString &mimeString);
    static const QString askDownloadPath(QWebEngineDownloadRequest *request);
};
