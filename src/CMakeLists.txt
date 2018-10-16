add_executable(img-dup
        main.cpp
        MainWindow.cpp
        ImgDup.cpp
        )

target_link_libraries(img-dup
        Qt5::Widgets
        ${PHASH_LIBRARIES}
        ${Boost_LIBRARIES}
        )
