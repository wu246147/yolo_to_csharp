cmake_minimum_required(VERSION 3.16)

project(ocr_alg_yolo_obb_opencv VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


# -------------- ONNX Runtime --------------
# 路径指向你解压的目录
set(ORT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/onnxruntime)
set(ORT_INCLUDE ${ORT_ROOT}/include)
message(ORT_INCLUDE:${ORT_INCLUDE})
include_directories(${ORT_INCLUDE})

if(WIN32)
    set(ORT_LIB ${ORT_ROOT}/lib/onnxruntime.lib)
else()
    set(ORT_LIB ${ORT_ROOT}/lib/libonnxruntime.so)
endif()

#alg
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/alg/include)
link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/alg/lib/opencv_onnx_obb_lib.lib)

#opencv
set(OpenCV_DIR "D:\\opencv4.8.0\\opencv\\build\\x64\\vc16\\lib")
 # set(OpenCV_DIR "E:\\env\\dll\\opencv-4.8.0_vs2022\\install\\x64\\vc17\\lib")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


# 库
add_library(RaiVasAlgOBBDet SHARED
    raivasalgobbdet.h
    raivasalgobbdet.cpp
)

target_link_libraries (RaiVasAlgOBBDet PRIVATE ${OpenCV_LIBS}  ${ORT_LIB})

# demo
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

add_executable(ocr_alg_yolo_obb_opencv
    ${PROJECT_SOURCES}
)

target_link_libraries(ocr_alg_yolo_obb_opencv PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(ocr_alg_yolo_obb_opencv PRIVATE ${OpenCV_LIBS}  ${ORT_LIB})
target_link_libraries (ocr_alg_yolo_obb_opencv PRIVATE RaiVasAlgOBBDet)

# # 库打包

# set(LIB_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/install_${PROJECT_VERSION})

# file(GLOB HEADERS *.h*)
# file(GLOB DEP_LIBS lib/*.lib)
# file(GLOB DEP_DLLS lib/*.dll)

# # target_link_libraries(install_test PRIVATE ${DEP_LIBS})

# include(GNUInstallDirs)
# install(TARGETS RaiVasAlgOCRDet
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${LIB_INSTALL_PATH}/lib
#     ARCHIVE DESTINATION ${LIB_INSTALL_PATH}/lib
#     RUNTIME DESTINATION ${LIB_INSTALL_PATH}/bin
# )

# install(FILES ${DEP_DLLS} DESTINATION ${LIB_INSTALL_PATH}/bin)
# install(FILES ${DEP_LIBS} DESTINATION ${LIB_INSTALL_PATH}/lib)
# install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${LIB_INSTALL_PATH}/include/)
# install(FILES ${HEADERS} DESTINATION ${LIB_INSTALL_PATH}/include/)



# set(EXE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/install_exe_${PROJECT_VERSION})

# file(GLOB HEADERS *.h*)
# file(GLOB DEP_LIBS lib/*.lib)
# file(GLOB DEP_DLLS lib/*.dll)

# # target_link_libraries(install_test PRIVATE ${DEP_LIBS})

# include(GNUInstallDirs)
# install(TARGETS RaiVasAlgOCRDet
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${EXE_INSTALL_PATH}/lib
#     ARCHIVE DESTINATION ${EXE_INSTALL_PATH}/lib
#     RUNTIME DESTINATION ${EXE_INSTALL_PATH}/bin
# )

# install(TARGETS ocr_alg_yolo_opencv
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${EXE_INSTALL_PATH}/lib
#     RUNTIME DESTINATION ${EXE_INSTALL_PATH}/bin
# )

# install(FILES ${DEP_DLLS} DESTINATION ${EXE_INSTALL_PATH}/bin)
# install(FILES ${DEP_LIBS} DESTINATION ${EXE_INSTALL_PATH}/lib)
# install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${EXE_INSTALL_PATH}/include/)
# install(FILES ${HEADERS} DESTINATION ${EXE_INSTALL_PATH}/include/)
