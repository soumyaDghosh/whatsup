#include <AbstractKirigamiApplication>
#include <QObject>
#include <QQmlEngine>


class App : public AbstractKirigamiApplication {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    App(QObject *parent = nullptr);
    ~App() override = default;
};