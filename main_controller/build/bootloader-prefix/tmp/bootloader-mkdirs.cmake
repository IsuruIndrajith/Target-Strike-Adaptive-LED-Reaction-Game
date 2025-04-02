# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Espressif/Espressif/frameworks/esp-idf-v5.3.1/components/bootloader/subproject"
  "D:/Espressif Development/main_controller/build/bootloader"
  "D:/Espressif Development/main_controller/build/bootloader-prefix"
  "D:/Espressif Development/main_controller/build/bootloader-prefix/tmp"
  "D:/Espressif Development/main_controller/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Espressif Development/main_controller/build/bootloader-prefix/src"
  "D:/Espressif Development/main_controller/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Espressif Development/main_controller/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Espressif Development/main_controller/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
