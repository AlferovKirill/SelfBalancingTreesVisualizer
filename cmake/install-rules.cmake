install(
    TARGETS SelfBalancingTreesVisualizer_exe
    RUNTIME COMPONENT SelfBalancingTreesVisualizer_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
