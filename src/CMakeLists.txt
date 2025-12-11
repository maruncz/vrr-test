
set(VRR_TEST_SRCS
    glmisc.hpp
    glshader.cpp
    glshader.h
    main.cpp
    misc.hpp
    strip.cpp
    strip.hpp
    window.cpp
    window.hpp
)

add_executable(vrr-test ${VRR_TEST_SRCS})

target_link_libraries(vrr-test GLEW::GLEW glfw glm::glm OpenGL::GL)
