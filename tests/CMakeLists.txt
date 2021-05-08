add_executable(queueModelTest tst_queueModelTest.cpp ../queuemodel.cpp)
add_test(NAME queueModelTest COMMAND queueModelTest)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
)
FetchContent_MakeAvailable(googletest)

target_include_directories(queueModelTest PRIVATE
    Qt5::Test
    "${googletest_SOURCE_DIR}/googletest/include"
    "${googletest_SOURCE_DIR}/googlemock/include"
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/mpdclient/include
)
target_link_libraries(queueModelTest PRIVATE Qt5::Core Qt5::Test mpdclient gmock)