set(LANGUAGES en ru)

foreach(LANGUAGE ${LANGUAGES})
    list(APPEND TS_FILES ${CMAKE_SOURCE_DIR}/assets/translations/SelfBalancingTreesVisualizer_${LANGUAGE}.ts)
endforeach()

qt_add_translations(SelfBalancingTreesVisualizer_exe
    TS_FILE_DIR ${CMAKE_SOURCE_DIR}/assets/translations
    TS_FILES ${TS_FILES}
    RESOURCE_PREFIX "/translations"
    LUPDATE_OPTIONS
        -locations none
    LRELEASE_OPTIONS
        -compress
        -nounfinished
        -removeidentical
)

string(REPLACE ";" "\",\n    \"" LANGUAGES_CPP_LIST "${LANGUAGES}")
set(LANGUAGES_CPP_LIST "\"${LANGUAGES_CPP_LIST}\"")

configure_file(
    ${CMAKE_SOURCE_DIR}/config/translations_cmake_variables.h.in
    ${CMAKE_SOURCE_DIR}/source/config/translations_cmake_variables.h
)
