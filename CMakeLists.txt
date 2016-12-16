project(Lugaru)

cmake_minimum_required(VERSION 2.8.7) #Steam runtime version
cmake_policy(SET CMP0004 OLD)

if(UNIX)
	include(FindPkgConfig)
	include(GNUInstallDirs)
endif()

### Helpers

set(SRCDIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()


### CMake config

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif(NOT CMAKE_BUILD_TYPE)
message("CMake build type: ${CMAKE_BUILD_TYPE}")

if(MINGW)
    add_definitions( -DMINGW )
endif()

if(WIN32)
    add_definitions( -DWINDOWS )
endif()

if(MINGW AND WIN32)
    #vorbis header wants fseeko64 
    set(CMAKE_CXX_FLAGS "-Wall -Wno-parentheses -pedantic --std=gnu++11 ${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wno-parentheses -pedantic ${CMAKE_CXX_FLAGS}")

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Build architectures for OSX")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING
        "Minimum OS X version to target for deployment (at runtime); newer APIs weak linked. Set to empty string for default value")
    set(CMAKE_OSX_SYSROOT "/Developer/SDKs/MacOSX10.9.sdk" CACHE PATH
        "The product will be built against the headers and libraries located inside the indicated SDK.")

    SET(PLATFORM_PREFIX             "macosx")
    
    SET(BIN_RPATH "@executable_path/../Frameworks")

    SET(CMAKE_SKIP_BUILD_RPATH              TRUE)
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH      TRUE)
    SET(CMAKE_INSTALL_RPATH                 ${BIN_RPATH})
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH   FALSE)
endif(APPLE)

option(BUILD_SERVER "Build server" Off)
option(DO_DIST_ARCHIVE "Generate dist archive" Off)
option(PERFORM_INSTALL "Generate steps for installation into system" On)

IF(BUILD_SERVER)
    SET(DO_DIST_ARCHIVE On)
    SET(PERFORM_INSTALL Off)
ENDIF()

if(LINUX)
    option(SYSTEM_INSTALL "Enable system-wide installation, with hardcoded data directory defined with CMAKE_INSTALL_DATADIR" OFF)
endif(LINUX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules/")


### Sources

set(LUGARU_SRCS
    ${SRCDIR}/main.cpp
    ${SRCDIR}/Animation/Animation.cpp
    ${SRCDIR}/Animation/Joint.cpp
    ${SRCDIR}/Animation/Muscle.cpp
    ${SRCDIR}/Animation/Skeleton.cpp
    ${SRCDIR}/Audio/openal_wrapper.cpp
    ${SRCDIR}/Audio/Sounds.cpp
    ${SRCDIR}/Devtools/ConsoleCmds.cpp
    ${SRCDIR}/Environment/Lights.cpp
    ${SRCDIR}/Environment/Skybox.cpp
    ${SRCDIR}/Environment/Terrain.cpp
    ${SRCDIR}/Graphic/Models.cpp
    ${SRCDIR}/Graphic/Sprite.cpp
    ${SRCDIR}/Graphic/Stereo.cpp
    ${SRCDIR}/Graphic/Text.cpp
    ${SRCDIR}/Graphic/Texture.cpp
    ${SRCDIR}/Level/Awards.cpp
    ${SRCDIR}/Level/Campaign.cpp
    ${SRCDIR}/Level/Dialog.cpp
    ${SRCDIR}/Level/Hotspot.cpp
    ${SRCDIR}/Math/Frustum.cpp
    ${SRCDIR}/Math/Quaternions.cpp
    ${SRCDIR}/Menu/Menu.cpp
    ${SRCDIR}/Objects/Objects.cpp
    ${SRCDIR}/Objects/Person.cpp
    ${SRCDIR}/Objects/Weapons.cpp
    ${SRCDIR}/User/Account.cpp
    ${SRCDIR}/User/Settings.cpp
    ${SRCDIR}/Utils/Folders.cpp
    ${SRCDIR}/Utils/ImageIO.cpp
    ${SRCDIR}/Utils/Input.cpp
    ${SRCDIR}/Utils/String.cpp
    ${SRCDIR}/Utils/pack.c
    ${SRCDIR}/Utils/private.c
    ${SRCDIR}/Utils/unpack.c
    ${SRCDIR}/Game.cpp
    ${SRCDIR}/GameDraw.cpp
    ${SRCDIR}/GameInitDispose.cpp
    ${SRCDIR}/GameTick.cpp
    ${SRCDIR}/Globals.cpp

)

set(LUGARU_H
    ${SRCDIR}/Animation/Animation.hpp
    ${SRCDIR}/Animation/Joint.hpp
    ${SRCDIR}/Animation/Muscle.hpp
    ${SRCDIR}/Animation/Skeleton.hpp
    ${SRCDIR}/Audio/openal_wrapper.hpp
    ${SRCDIR}/Audio/Sounds.hpp
    ${SRCDIR}/Devtools/ConsoleCmds.hpp
    ${SRCDIR}/Environment/Lights.hpp
    ${SRCDIR}/Environment/Skybox.hpp
    ${SRCDIR}/Environment/Terrain.hpp
    ${SRCDIR}/Graphic/gamegl.hpp
    ${SRCDIR}/Graphic/Models.hpp
    ${SRCDIR}/Graphic/Sprite.hpp
    ${SRCDIR}/Graphic/Stereo.hpp
    ${SRCDIR}/Graphic/Text.hpp
    ${SRCDIR}/Graphic/Texture.hpp
    ${SRCDIR}/Level/Campaign.hpp
    ${SRCDIR}/Level/Dialog.hpp
    ${SRCDIR}/Level/Hotspot.hpp
    ${SRCDIR}/Math/Frustum.hpp
    ${SRCDIR}/Math/PhysicsMath.hpp
    ${SRCDIR}/Math/Quaternions.hpp
    ${SRCDIR}/Math/Random.hpp
    ${SRCDIR}/Menu/Menu.hpp
    ${SRCDIR}/Objects/Objects.hpp
    ${SRCDIR}/Objects/Person.hpp
    ${SRCDIR}/Objects/Weapons.hpp
    ${SRCDIR}/Thirdparty/optionparser.h
    ${SRCDIR}/User/Account.hpp
    ${SRCDIR}/User/Settings.hpp
    ${SRCDIR}/Utils/binio.h
    ${SRCDIR}/Utils/Folders.hpp
    ${SRCDIR}/Utils/ImageIO.hpp
    ${SRCDIR}/Utils/Input.hpp
    ${SRCDIR}/Utils/String.hpp
    ${SRCDIR}/Utils/private.h
    ${SRCDIR}/Game.hpp

)

if(UNIX)
    set(LUGARU_SRCS
        ${LUGARU_SRCS}
        ${SRCDIR}/MacCompatibility.cpp
    )
    set(LUGARU_H
        ${LUGARU_H}
        ${SRCDIR}/MacCompatibility.hpp
    )
endif(UNIX)

if(WIN32)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lugaru_resource.obj
                       COMMAND ${CMAKE_RC_COMPILER}
                       -I${SRCDIR}/win-res
                       -o ${CMAKE_CURRENT_BINARY_DIR}/lugaru_resource.obj
                       -i${SRCDIR}/win-res/Lugaru.rc
                       DEPENDS ${SRCDIR}/win-res/Lugaru.rc
    )

    # FIXME: get rid of this.
    set(LUGARU_SRCS
        ${LUGARU_SRCS}
        ${SRCDIR}/WinDefs.cpp)

    set(LUGARU_H
        ${LUGARU_H}
        ${SRCDIR}/WinDefs.hpp
        ${SRCDIR}/win-res/resource.hpp)
endif(WIN32)

if(APPLE)
    set(PLATFORM_LIBS "-framework Carbon -framework Cocoa -framework OpenGL -framework OpenAL")
endif(APPLE)


### Dependencies

find_package(OpenGL REQUIRED)

# Windows is funky about OpenAL detection
if(WIN32)
    if(UNIX)
        pkg_check_modules(OPENAL openal REQUIRED)
    elseif(MINGW)
    	find_package(OpenAL REQUIRED)
    endif()
    set(OPENAL_LIBRARY ${OPENAL_LIBRARIES})
else(WIN32)
    find_package(OpenAL REQUIRED)
endif(WIN32)

find_package(SDL2 REQUIRED)

message( "SDL2 path ${SDL2_INCLUDE_DIR}" )

if(APPLE)
   find_library(PNG_STATIC_LIBRARY libpng.a)
   find_library(JPEG_STATIC_LIBRARY libjpeg.a)
   find_library(OGG_STATIC_LIBRARY libogg.a)
   find_library(VORBIS_STATIC_LIBRARY libvorbis.a)
   find_library(VORBISFILE_STATIC_LIBRARY libvorbisfile.a)
   find_library(VORBISENC_STATIC_LIBRARY libvorbisenc.a)
   find_library(Z_STATIC_LIBRARY libz.a /usr/local/opt/zlib/lib/)
   set(USE_STATIC_LIBS True)
elseif(MINGW AND WIN32)
   find_library(PNG_STATIC_LIBRARY libpng.a)
   find_library(JPEG_STATIC_LIBRARY libjpeg.a)
   find_library(OGG_STATIC_LIBRARY libogg.a)
   find_library(VORBIS_STATIC_LIBRARY libvorbis.a)
   find_library(VORBISFILE_STATIC_LIBRARY libvorbisfile.a)
   find_library(VORBISENC_STATIC_LIBRARY libvorbisenc.a)
   find_library(Z_STATIC_LIBRARY libzlibstatic.a)
   set(USE_STATIC_LIBS True)
endif()

find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OggVorbis REQUIRED)

include_directories(
    ${OPENAL_INCLUDE_DIR}
    ${JPEG_INCLUDE_DIR}
    ${PNG_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${VORBISFILE_INCLUDE_DIR}
    ${OGG_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/Source
)

if(USE_STATIC_LIBS)
	set(LUGARU_LIBS ${OPENAL_LIBRARY} ${PNG_STATIC_LIBRARY} ${JPEG_STATIC_LIBRARY} ${Z_STATIC_LIBRARY} ${SDL2_LIBRARY} ${OPENGL_LIBRARIES} ${VORBISENC_STATIC_LIBRARY} ${VORBISFILE_STATIC_LIBRARY} ${VORBIS_STATIC_LIBRARY} ${OGG_STATIC_LIBRARY} ${PLATFORM_LIBS})
else()
	set(LUGARU_LIBS ${OPENAL_LIBRARY} ${PNG_LIBRARY} ${JPEG_LIBRARY} ${ZLIB_LIBRARIES} ${SDL2_LIBRARY} ${OPENGL_LIBRARIES} ${VORBISFILE_LIBRARY} ${OGG_LIBRARY} ${PLATFORM_LIBS})
endif()

message( "Libs: ${LUGARU_LIBS}" )

### Definitions

if(WIN32)
    add_executable(Lugaru ${LUGARU_SRCS} ${LUGARU_H} lugaru_resource.obj)
else(WIN32)
    add_executable(Lugaru ${LUGARU_SRCS} ${LUGARU_H})
endif(WIN32)

target_link_libraries(Lugaru ${LUGARU_LIBS})

if(WIN32)
    add_definitions(-DBinIO_STDINT_HEADER=<stdint.h>)
    if(MINGW)
        # An alternative would be to use _WIN32 consistently instead of WIN32
        add_definitions(-DWIN32)
    endif(MINGW)
else(WIN32)
    add_definitions(-DPLATFORM_LINUX=1 -DPLATFORM_UNIX=1 -DBinIO_STDINT_HEADER=<stdint.h>)
endif(WIN32)


### Installation
# We don't care about installing in our version

if(PERFORM_INSTALL)
    if(NOT CMAKE_INSTALL_PREFIX AND WIN32)
        set(CMAKE_INSTALL_PREFIX "C:/Lugaru")
    endif(NOT CMAKE_INSTALL_PREFIX AND WIN32)

    # OS-specific installation paths

    set(LUGARU_DOCDIR ${CMAKE_INSTALL_PREFIX})
    if(LINUX)
    endif(LINUX)

    if(APPLE)
        set(LUGARU_APP_ROOT ${CMAKE_INSTALL_PREFIX}/Lugaru.app)
        set(LUGARU_BINDIR ${LUGARU_APP_ROOT}/Contents/MacOS)
        set(LUGARU_RESDIR ${LUGARU_APP_ROOT}/Contents/Resources)
    endif(APPLE)

    # Actual installation instructions

    if(WIN32)
        install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/Lugaru.exe DESTINATION ${CMAKE_INSTALL_PREFIX})
        install(DIRECTORY ${CMAKE_SOURCE_DIR}/Data DESTINATION ${CMAKE_INSTALL_PREFIX})

        if(MINGW)
            # Based off Mageia/Fedora MinGW toolchain, might not work on other distros or Windows
            set(LIBGCC_S libgcc_s_sjlj-1.dll)
            if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64") # MinGW64
                set(LIBGCC_S libgcc_s_seh-1.dll)
            endif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
            # FIXME: Filter out unneeded DLLs when building against some internal deps
            set(DLL_ROOT ${CMAKE_FIND_ROOT_PATH}/bin)
            install(FILES ${DLL_ROOT}/${LIBGCC_S}
                          ${DLL_ROOT}/libjpeg-62.dll
                          ${DLL_ROOT}/libogg-0.dll
                          ${DLL_ROOT}/libpng16-16.dll
                          ${DLL_ROOT}/libstdc++-6.dll
                          ${DLL_ROOT}/libvorbis-0.dll
                          ${DLL_ROOT}/libvorbisfile-3.dll
                          ${DLL_ROOT}/libwinpthread-1.dll
                          ${DLL_ROOT}/OpenAL32.dll
                          ${DLL_ROOT}/SDL2.dll
                          ${DLL_ROOT}/zlib1.dll
                    DESTINATION ${CMAKE_INSTALL_PREFIX})
        endif(MINGW)
    endif(WIN32)

    if(LINUX)
        if(SYSTEM_INSTALL)
            add_definitions(-DDATA_DIR="${CMAKE_INSTALL_FULL_DATADIR}/${PROJECT_NAME}")
            set(LUGARU_DOCDIR ${CMAKE_INSTALL_DOCDIR})
            install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/Lugaru DESTINATION ${CMAKE_INSTALL_BINDIR})
            # Trailing '/' is significant, it installs and _renames_ Data/ as the destination folder
            install(DIRECTORY ${CMAKE_SOURCE_DIR}/Data/ DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})
            install(FILES ${CMAKE_SOURCE_DIR}/Dist/lugaru.appdata.xml DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/appdata)
            install(FILES ${CMAKE_SOURCE_DIR}/Dist/lugaru.desktop DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
            install(FILES ${CMAKE_SOURCE_DIR}/Dist/lugaru.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/128x128/apps)
            install(FILES ${CMAKE_SOURCE_DIR}/Dist/lugaru.6 DESTINATION ${CMAKE_INSTALL_MANDIR}/man6)
        else(SYSTEM_INSTALL)
            message("You are building Lugaru without having enabled the SYSTEM_INSTALL option. It will default to looking for the data in the 'Data' directory next to the binary.")
            install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/Lugaru DESTINATION ${CMAKE_INSTALL_PREFIX})
            install(DIRECTORY ${CMAKE_SOURCE_DIR}/Data DESTINATION ${CMAKE_INSTALL_PREFIX})
        endif(SYSTEM_INSTALL)
    endif(LINUX)

    if(APPLE)
        install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/Lugaru DESTINATION ${LUGARU_BINDIR})
        install(DIRECTORY ${CMAKE_SOURCE_DIR}/Data DESTINATION ${LUGARU_APP_ROOT})
        install(FILES ${CMAKE_SOURCE_DIR}/Dist/OSX/Lugaru.icns DESTINATION ${LUGARU_RESDIR})
        install(FILES ${CMAKE_SOURCE_DIR}/Dist/OSX/Info.plist DESTINATION ${LUGARU_APP_ROOT}/Contents)
    endif(APPLE)

    # Documentation
    install(FILES ${CMAKE_SOURCE_DIR}/AUTHORS
                  ${CMAKE_SOURCE_DIR}/CONTENT-LICENSE.txt
                  ${CMAKE_SOURCE_DIR}/COPYING.txt
                  ${CMAKE_SOURCE_DIR}/README.md
                  ${CMAKE_SOURCE_DIR}/RELEASE-NOTES.md
                  ${CMAKE_SOURCE_DIR}/Docs/DEVTOOLS.txt
                  ${CMAKE_SOURCE_DIR}/Docs/README.Empire.txt
                  ${CMAKE_SOURCE_DIR}/Docs/README.SevenTasks.txt
                  ${CMAKE_SOURCE_DIR}/Docs/README.Temple.txt
            DESTINATION ${LUGARU_DOCDIR})
endif(PERFORM_INSTALL)

# Generate an archive with the entire game for redist.
IF(DO_DIST_ARCHIVE)
    SET(DIST_ARCHIVE "${CMAKE_BINARY_DIR}/Lugaru.tar" CACHE STRING "Name of packaged file containing game binaries")
    IF(LINUX)
        ADD_CUSTOM_TARGET( GenerateDistArchive ALL 
            COMMAND "tar" "-chf" "\"${DIST_ARCHIVE}\"" 
              "Lugaru" 
              "Data/"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        )
        ADD_CUSTOM_TARGET( GenerateDistArchive2 ALL
            COMMAND "tar" "-rhf" "\"${DIST_ARCHIVE}\"" 
              "Docs/"
              "AUTHORS"
              "CONTENT-LICENSE.txt"
              "COPYING.txt"
              "README.md"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        )
        ADD_DEPENDENCIES( GenerateDistArchive2
                            GenerateDistArchive
        )
    ENDIF()
    IF(APPLE)
	 SET( APPLE_APP_FOLDER "${CMAKE_BINARY_DIR}/Lugaru.app" )

	 ADD_CUSTOM_TARGET( CreateMacOSXAppStructure ALL
             COMMAND "mkdir" "-p" "${APPLE_APP_FOLDER}"
             COMMAND "mkdir" "-p" "${APPLE_APP_FOLDER}/Contents"
             COMMAND "mkdir" "-p" "${APPLE_APP_FOLDER}/Contents/Frameworks"
             COMMAND "mkdir" "-p" "${APPLE_APP_FOLDER}/Contents/MacOS"
             COMMAND "mkdir" "-p" "${APPLE_APP_FOLDER}/Contents/Resources"
             WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	)

	ADD_CUSTOM_TARGET( MacOSXAppAddBinary ALL
	     COMMAND "cp" "Lugaru" "${APPLE_APP_FOLDER}/Contents/MacOS/"
             WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	)
	
        ADD_DEPENDENCIES( MacOSXAppAddBinary
             CreateMacOSXAppStructure
             Lugaru 
        )

	ADD_CUSTOM_TARGET( MacOSXAppAddData ALL
	     COMMAND "cp" "-r" "Data" "${APPLE_APP_FOLDER}/"
	     COMMAND "cp" "Dist/OSX/Info.plist" "${APPLE_APP_FOLDER}/Contents/"
	     COMMAND "cp" "Dist/OSX/Lugaru.icns" "${APPLE_APP_FOLDER}/Contents/Resources/"
	     COMMAND "cp" "-r" "Docs" "${APPLE_APP_FOLDER}/Contents/Resources/"
	     COMMAND "cp" "AUTHORS" "${APPLE_APP_FOLDER}/Contents/Resources/"
	     COMMAND "cp" "CONTENT-LICENSE.txt" "${APPLE_APP_FOLDER}/Contents/Resources/"
	     COMMAND "cp" "COPYING.txt" "${APPLE_APP_FOLDER}/Contents/Resources/"
	     COMMAND "cp" "README.md" "${APPLE_APP_FOLDER}/Contents/Resources/"
	     WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
	)

        ADD_DEPENDENCIES( MacOSXAppAddData
             CreateMacOSXAppStructure
        )

	ADD_CUSTOM_TARGET( MacOSXAppAddFrameworks ALL
             COMMAND "cp" "-r" "/Library/Frameworks/SDL2.framework" "${APPLE_APP_FOLDER}/Contents/Frameworks/"
	)

        ADD_DEPENDENCIES( MacOSXAppAddFrameworks
             CreateMacOSXAppStructure
        )

        ADD_CUSTOM_TARGET( GenerateDistArchive ALL 
            COMMAND "tar" "-chf" "\"${DIST_ARCHIVE}\"" "Lugaru.app"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        )

        ADD_DEPENDENCIES( GenerateDistArchive
             CreateMacOSXAppStructure
             MacOSXAppAddData
             MacOSXAppAddBinary
	     MacOSXAppAddFrameworks
        )

    ENDIF()
    IF(WIN32 AND MINGW)
	FIND_PROGRAM(SEVEN_ZIP NAMES 7z PATHS "C:/Program Files/7-Zip")
	FIND_FILE( SDL2_DLL NAMES SDL2.dll PATHS "C:/SDL2/i686-w64-mingw32/bin" )
	FIND_FILE( LIBGCC_DLL NAMES "libgcc_s_dw2-1.dll" PATHS "C:/MinGW/bin" )
	FIND_FILE( LIBSTDC_DLL NAMES "libstdc++-6.dll" PATHS "C:/MinGW/bin" )
	message("xcopy ${SDL2_DLL} .")
	ADD_CUSTOM_TARGET( CopyDLLFiles ALL
            COMMAND "cmake" "-E" "copy" 
	    "${SDL2_DLL}" 
            "${LIBGCC_DLL}"
	    "${LIBSTDC_DLL}"
	    "."
	    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	)

	ADD_CUSTOM_TARGET( CopyDocFiles ALL 
            COMMAND "cmake" "-E" "copy_if_different"
		  "Docs"
                  AUTHORS
                  CONTENT-LICENSE.txt
                  COPYING.txt
                  README.md
		  "${CMAKE_BINARY_DIR}/"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
	)

        ADD_CUSTOM_TARGET( GenerateDistArchive ALL 
            COMMAND ${SEVEN_ZIP} "a" "-mTAR" "\"${DIST_ARCHIVE}\"" 
	        "lugaru.exe"
	        "Data"
	        "SDL2.dll"
		"libgcc_s_dw2-1.dll"
                "libstdc++-6.dll"
		"Docs"
	        AUTHORS
	        CONTENT-LICENSE.txt
	        COPYING.txt
	        README.md
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        )

        ADD_DEPENDENCIES( GenerateDistArchive
		CopyDLLFiles
		CopyDocFiles
        )

    ENDIF()
    ADD_DEPENDENCIES( GenerateDistArchive
        Lugaru
    )
ENDIF()
