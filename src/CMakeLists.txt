find_package(Qt4 REQUIRED COMPONENTS QtGui QtCore QtDBus)
find_package(KDE4 REQUIRED)
include(${QT_USE_FILE})
include(KDE4Defaults)
find_package(QZeitgeist REQUIRED 0.7.90)

set(krono_SRCS
    main.cpp
    EventDelegate.cpp
    MainWindow.cpp
    LinearBrowser.cpp
    CategoryModel.cpp
    LinearTileDelegate.cpp
    )

kde4_add_ui_files(krono_SRCS
    MainWindow.ui
    )


qt4_automoc(${krono_SRCS})

kde4_add_executable(krono ${krono_SRCS})

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${QZEITGEIST_INCLUDE_DIR} ${QT_INCLUDES} ${KDE4_INCLUDES})
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(krono ${QT_LIBRARIES} ${QZEITGEIST_LIBRARY} ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_THREADWEAVER_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS krono ${INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS krono.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES kronoui.rc DESTINATION ${DATA_INSTALL_DIR}/krono)
