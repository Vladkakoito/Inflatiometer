#The "inih" library is distributed under the New BSD license:

# Copyright (c) 2009, Ben Hoyt
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of Ben Hoyt nor the names of its contributors
#       may be used to endorse or promote products derived from this software
#       without specific prior written permission.

# THIS SOFTWARE IS PROVIDED BY BEN HOYT ''AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL BEN HOYT BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
add_subdirectory(Ini)

# Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
# associated documentation files (the "Software"), to deal in the Software without restriction, 
# including without limitation the rights to use, copy, modify, merge, publish, distribute, 
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all copies or 
# substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
set(SAVE_CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}) 
set(ENABLE_CJSON_TEST OFF CACHE BOOL "Enable building the tests. (on by default)")
set(ENABLE_CJSON_UTILS OFF CACHE BOOL "Enable building cJSON_Utils. (off by default)")
set(ENABLE_TARGET_EXPORT OFF CACHE BOOL "Enable the export of CMake targets. Turn off if it makes problems. (on by default)")
set(ENABLE_CUSTOM_COMPILER_FLAGS OFF CACHE BOOL "Enable custom compiler flags (currently for Clang, GCC and MSVC). Turn off if it makes problems. (on by default)")
set(ENABLE_VALGRIND OFF CACHE BOOL "Run tests with valgrind. (off by default)")
set(ENABLE_SANITIZERS OFF CACHE BOOL "Compile cJSON with AddressSanitizer and UndefinedBehaviorSanitizer enabled (if possible). (off by default)")
set(ENABLE_SAFE_STACK OFF CACHE BOOL "Enable the SafeStack instrumentation pass. Currently only works with the Clang compiler. (off by default)")
set(DBUILD_SHARED_LIBS OFF CACHE BOOL "Build the shared libraries. (on by default)")
set(BUILD_SHARED_AND_STATIC_LIBS OFF CACHE BOOL "Build both shared and static libraries. (off by default)")
set(ENABLE_LOCALES OFF CACHE BOOL "Enable the usage of localeconv method. ( on by default )")
set(CJSON_OVERRIDE_BUILD_SHARED_LIBS OFF CACHE BOOL "Enable overriding the value of BUILD_SHARED_LIBS with -DCJSON_BUILD_SHARED_LIBS.")
set(ENABLE_CJSON_VERSION_SO OFF CACHE BOOL "Enable cJSON so version. ( on by default )")
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}")
add_subdirectory(Json)
set(CMAKE_INSTALL_PREFIX ${SAVE_CMAKE_INSTALL_PREFIX})