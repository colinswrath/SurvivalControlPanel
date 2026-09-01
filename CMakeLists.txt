cmake_minimum_required(VERSION 3.20)

project(
	SurvivalControlPanel
	VERSION 1.1.3
	LANGUAGES CXX
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

list(APPEND CMAKE_IGNORE_PATH "${PROJECT_BINARY_DIR}")
list(APPEND CMAKE_MODULE_PATH
	"${PROJECT_SOURCE_DIR}/tools/SKSE-CMakeModules/Modules"
	"${PROJECT_SOURCE_DIR}/tools/BethesdaCMakeModules/Modules"
)
include(SKSEPlugin)
include(Papyrus)
include(BSArchive)

SKSEPlugin_Add(
	${PROJECT_NAME}
	SOURCE_DIR src
	INCLUDE_DIR include
	SOURCES
		.clang-format
		.editorconfig
		README.md
		vcpkg.json
	PRECOMPILE_HEADERS
		src/common/PCH.h
)

set_target_properties(CommonLibSSE PROPERTIES
	FOLDER External
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	INTERPROCEDURAL_OPTIMIZATION TRUE
	INTERPROCEDURAL_OPTIMIZATION_DEBUG FALSE
)

set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")

set(TRANSLATION_FILES
	"${DATA_DIR}/Interface/Translations/SurvivalControlPanel_ENGLISH.txt"
)

set(Papyrus_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/scripts/Survival.psc"
	"${CMAKE_CURRENT_SOURCE_DIR}/scripts/Survival_Json.psc"
	"${CMAKE_CURRENT_SOURCE_DIR}/scripts/Survival_MCM.psc"
)

if(SkyrimSE_PATH)
	Papyrus_Add(
		"Papyrus"
		GAME ${SkyrimSE_PATH}
		IMPORTS
			"${CMAKE_CURRENT_SOURCE_DIR}/scripts"
			"$ENV{SkyUISDKPath}/Scripts/Source"
			"$ENV{SKSE64Path}/Scripts/Source"
		SOURCES ${Papyrus_SOURCES}
		OPTIMIZE
		ANONYMIZE
	)

	BSArchive_Add(
		"BSA"
		OUTPUT "SurvivalControlPanel.bsa"
		FORMAT SSE
		FILES
			${Papyrus_OUTPUT}
			${TRANSLATION_FILES}
			"${DATA_DIR}/Interface/SurvivalCP/SCP_splash.swf"
		PREFIX
			"${DATA_DIR}"
	)

	add_dependencies("BSA" "Papyrus")
endif()

install(
	FILES
		"${DATA_DIR}/SurvivalControlPanel.esp"
	DESTINATION "."
)

if(TARGET BSA)
	install(
		FILES
			"${CMAKE_CURRENT_BINARY_DIR}/SurvivalControlPanel.bsa"
		DESTINATION "."
	)
endif()

if(TARGET Papyrus)
	install(
		FILES ${Papyrus_SOURCES}
		DESTINATION
			"Source/Scripts"
	)
endif()

if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  file(WRITE "${CMAKE_BINARY_DIR}/.gitignore" "*")
  file(WRITE "${CMAKE_BINARY_DIR}/.hgignore" "syntax: glob\n**/*")
endif()

set(CPACK_GENERATOR "7Z")
set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${PROJECT_VERSION})
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY FALSE)
include(CPack)
