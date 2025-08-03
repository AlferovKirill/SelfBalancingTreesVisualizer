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

string(REPLACE ";" "\",\n    \"" LANGUAGES_LIST "${LANGUAGES}")
set(LANGUAGES_LIST "\n    \"${LANGUAGES_LIST}\"\n")

list(LENGTH LANGUAGES LANGUAGES_COUNT)

include(${CMAKE_SOURCE_DIR}/cmake/config/translations.cmake)