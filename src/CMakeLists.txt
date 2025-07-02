# SPDX-FileCopyrightText: 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ) <soumyadeepghosh2004@zohomail.in>
#
# SPDX-License-Identifier: BSD-2-Clause

add_library(whatsup_static STATIC)

target_sources(whatsup_static PRIVATE
    permissionmanager.h
    permissionmanager.cpp
)

kconfig_target_kcfg_file(whatsup_static FILE config.kcfg CLASS_NAME Config MUTATORS GENERATE_PROPERTIES GENERATE_MOC DEFAULT_VALUE_GETTERS PARENT_IN_CONSTRUCTOR QML_REGISTRATION SINGLETON)

ecm_add_qml_module(whatsup_static
    URI io.github.soumyadghosh.whatsup
    VERSION 1.0
    GENERATE_PLUGIN_SOURCE
    QML_FILES
        qml/Main.qml
        qml/WhatsupDrawer.qml
        qml/SettingsPage.qml
        qml/RequestPermission.qml
        qml/WebEnginePage.qml
)

target_include_directories(whatsup_static PUBLIC ${CMAKE_BINARY_DIR})
target_link_libraries(whatsup_static
    PUBLIC
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Widgets
    Qt6::WebEngineQuick
    KF6::I18n
    KF6::I18nQml
    KF6::CoreAddons
    KF6::IconThemes
    KirigamiAddonsStatefulApp
)

add_executable(whatsup
    main.cpp
)
target_link_libraries(whatsup PUBLIC whatsup_static whatsup_staticplugin)

install(TARGETS whatsup ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
