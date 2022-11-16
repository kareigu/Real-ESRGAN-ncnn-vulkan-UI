cmake_minimum_required(VERSION 3.5)

project(realesrgan-ncnn-vulkan-ui VERSION 0.1 LANGUAGES CXX)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/main_window.cpp
        src/message_log.cpp
        src/path_picker.cpp
        src/settings_panel.cpp
        src/about_window.cpp
        src/options_window.cpp
        src/options.cpp
        src/download_manager.cpp
        )

set(PROJECT_RESOURCES
        resources/resources.qrc)


qt_add_executable(realesrgan-ncnn-vulkan-ui
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_RESOURCES}
        )


target_link_libraries(realesrgan-ncnn-vulkan-ui PRIVATE Qt6::Widgets)

set_target_properties(realesrgan-ncnn-vulkan-ui PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

qt_finalize_executable(realesrgan-ncnn-vulkan-ui)
