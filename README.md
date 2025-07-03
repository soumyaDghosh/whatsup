# WhatsUp

A WhatsApp wrapper using QtWebEngine, and Kirigami to make it look nice with KDE.

# Building from source

Currently it's not possible to build from source without the libraries from latest commmit of KDE Framework, as there are some specific patches of Kirigami Addons being used. [1](https://invent.kde.org/libraries/kirigami-addons/-/merge_requests/371) [2](https://invent.kde.org/libraries/kirigami-addons/-/merge_requests/373)

You can use [KDE Builder](https://kde-builder.kde.org/) to build this project from source.

## Packages required
All the listed packages are from the KDE Neon repo

- cmake
- qt6-webengine-dev
- kf6-kirigami-dev
- kf6-kirigami-addons-dev
- kf6-extra-cmake-modules
- kf6-ki18n-dev
- kf6-kconfig-dev
- kf6-kiconthemes-dev
- qml-module-org-kde-qqc2desktopstyle

# TODO:

[] Add support for multiple profiles
[] Add support for notifications
[] Implement Download manager
[] Optimize performance of the web view
