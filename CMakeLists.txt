# The MIT License (MIT)
#
# Copyright (c) 2020-2021 Towel 42 Development, LLC and Scott Aron Bloom
# SPDX-License-Identifier: MIT License
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.30)

if( TOWEL42_MEDIAUTILS_DLL )
    SET( TOWEL42_MEDIAUTILS_DLL ON CACHE BOOL "Build Towel42 Media Utils as a Shared Library" )
else()
    SET( TOWEL42_MEDIAUTILS_DLL OFF CACHE BOOL "Build Towel42 Media Utils as a Shared Library" )
endif()

SET( TOWEL42_MKVUTILS OFF CACHE BOOL "Support for MKV Utilities" )
   
message( STATUS "T42-MediaUtils: Building for DLL=${TOWEL42_MEDIAUTILS_DLL}" )
message( STATUS "T42-MediaUtils: Building TOWEL42_MKVUTILS=${TOWEL42_MKVUTILS}" )

if ( NOT TOWEL42_MEDIAUTILS_DIR )
    SET( TOWEL42_MEDIAUTILS_DIR ${CMAKE_CURRENT_LIST_DIR} )
endif()

if ( TOWEL42_MKVUTILS )
    SET(NO_USE_MEDIAINFO_INSTALL ON)
    set(BUILD_SHARED_LIBS ON)
	option(LARGE_FILES "Enable large files support" OFF)
    add_subdirectory( MediaLibs.ProjectFiles/MediaInfoLib )
    add_subdirectory( MKVReader )
endif()

include( ${CMAKE_CURRENT_LIST_DIR}/include.cmake )
project( ${_PROJECT_NAME} ) 

find_package(IncludeProjectSettings REQUIRED)
IncludeProjectSettings()

find_package(AddUnitTest REQUIRED)

if ( TOWEL42_MKVUTILS )
    include_directories(MediaLibs/MediaInfoLib/Source)
    include_directories(MediaLibs/ZenLib/Source)
endif()

if ( TOWEL42_MEDIAUTILS_DLL )
    SET( LIBTYPE SHARED )
    add_compile_definitions( 
        TOWEL42_MEDIAUTILS_INTERNAL
        $<$<BOOL:TOWEL42_MEDIAUTILS_DLL>:TOWEL42_MEDIAUTILS_DLL>
    )
else()
    SET( LIBTYPE STATIC )
endif()

add_library(${PROJECT_NAME} ${LIBTYPE}
    ${_PROJECT_DEPENDENCIES}
)

target_link_libraries( ${PROJECT_NAME}
    PUBLIC
        ${project_pub_DEPS}
    PRIVATE 
        ${project_pri_DEPS}
)

if ( TOWEL42_MEDIAUTILS_DLL )
    set_target_properties( ${PROJECT_NAME} PROPERTIES 
                                FOLDER ${FOLDER_NAME} 
                                DEBUG_POSTFIX d
   ) 
    if ( WIN32 )
        INSTALL( FILES $<TARGET_FILE_DIR:${PROJECT_NAME}>/$<TARGET_FILE_BASE_NAME:${PROJECT_NAME}>.dll DESTINATION . )
        INSTALL( FILES $<TARGET_FILE_DIR:${PROJECT_NAME}>/$<TARGET_FILE_BASE_NAME:${PROJECT_NAME}>.pdb DESTINATION . CONFIGURATIONS Debug RelWithDebInfo )
    else()
        INSTALL( FILES $<TARGET_FILE_DIR:${PROJECT_NAME}>/lib$<TARGET_FILE_BASE_NAME:${PROJECT_NAME}>.so DESTINATION . )
    endif()
else()
    set_target_properties( ${PROJECT_NAME} PROPERTIES FOLDER ${FOLDER_NAME} )
endif()

if ( NOT TOWEL42_NO_DEPLOY_SUBMODULES )
    find_package(Deploy REQUIRED)
    if ( TOWEL42_MEDIAUTILS_DLL )
        IF(TOWEL42_MKVUTILS)
            DeployQt( ${PROJECT_NAME} . )
        ENDIF()

        DeploySystem( ${PROJECT_NAME} . )
    endif()
endif()
