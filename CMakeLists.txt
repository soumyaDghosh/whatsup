# SPDX-FileCopyrightText: 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ) <soumyadeepghosh2004@zohomail.in>
#
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.20)

project(whatsup LANGUAGES CXX)

find_package(ECM 6.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMFindQmlModule)
include(ECMQmlModule)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Quick
    Test
    Gui
    QuickControls2
    Widgets
    WebEngineQuick
)

find_package(KF6 REQUIRED COMPONENTS
    Kirigami
    I18n
    Config
    CoreAddons
    QQC2DesktopStyle
    IconThemes
    KirigamiAddons
)


ecm_find_qmlmodule(org.kde.kirigami REQUIRED)
ecm_find_qmlmodule(org.kde.kirigamiaddons REQUIRED)

add_subdirectory(src)
