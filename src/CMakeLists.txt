
set(VRR_TEST_SRCS
    main.cpp
    window.cpp
    window.hpp
)

add_executable(vrr-test ${VRR_TEST_SRCS}
    glmisc.hpp)

target_link_libraries(vrr-test GLEW::GLEW glfw glm::glm OpenGL::GL)
