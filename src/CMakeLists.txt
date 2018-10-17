add_executable(img-dup
        main.cpp
        MainWindow.cpp
        )

target_link_libraries(img-dup
        Qt5::Widgets
        ${Boost_LIBRARIES}
        ${OpenCV_LIBS}
        )
