qt_add_translations(SelfBalancingTreesVisualizer_exe
    TS_FILE_DIR ${CMAKE_SOURCE_DIR}/assets/translations
    RESOURCE_PREFIX "/translations"
    LUPDATE_OPTIONS
        -locations none
    LRELEASE_OPTIONS
        -compress
        -nounfinished
        -removeidentical
)