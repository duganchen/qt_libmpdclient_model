add_library(mpdclient SHARED
  connection.cpp
  song.cpp
  status.cpp
)

target_include_directories(mpdclient PRIVATE ${LIBMPDCLIENT_INCLUDE_DIRS})
target_compile_definitions(mpdclient PRIVATE MPDCLIENT_LIBRARY)
target_link_libraries(mpdclient PRIVATE PkgConfig::LIBMPDCLIENT)
set_property(TARGET mpdclient PROPERTY CXX_STANDARD 20)
