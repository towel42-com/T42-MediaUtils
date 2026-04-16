# The MIT License (MIT)
#
# Copyright (c) 2020-2026 Scott Aron Bloom
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

if ( TOWEL42_MEDIAUTILS_DLL )
    set(FOLDER_NAME SharedLibs)
else()
    set(FOLDER_NAME Libs)
endif()

if ( TOWEL42_MKVUTILS )
    find_package(Qt6 COMPONENTS Multimedia REQUIRED)
    add_definitions( -DTOWEL42_MKVUTILS=1)
    set(qtproject_SRCS
        FFMpegFormats.cpp
        MediaInfo.cpp
        MKVUtils.cpp
        SetMKVTags.cpp
        Towel42MediaUtilsResources.cpp
    )
    set(qtproject_H
        MediaInfo.h
        SetMKVTags.h
    )
    set(project_H
        FFMpegFormats.h
        MKVUtils.h
        Towel42MediaUtilsExport.h
        Towel42MediaUtilsFwd.h
        Towel42MediaUtilsResources.h
    )
    set(qtproject_UIS
        SetMKVTags.ui
    )

    SET( project_pub_DEPS
        ${project_pub_DEPS}
        MediaInfo
        MKVReader
        Qt6::Core
        Qt6::Widgets
        Qt6::Multimedia
        Towel42Utils
    )
    set(qtproject_QRC
        resources/Towel42MediaUtils.qrc
    )

endif()

