add_library(list
  list.h 
  testList.cpp
)
set_property(TARGET list PROPERTY CXX_STANDARD 17)

add_executable(list
  testList.cpp
)
