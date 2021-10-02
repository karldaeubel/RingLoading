set(LP_FILES
  Interface.cpp
  BaseLp.cpp
  ReducedLp.cpp
)

add_library(${LP_LIB} STATIC ${LP_FILES})

target_include_directories(${LP_LIB} PRIVATE ${GUROBI_INCLUDE_DIRS})
