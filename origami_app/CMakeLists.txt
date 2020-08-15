cmake_minimum_required(VERSION 2.8.4)
project(origami_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_directories(${ORIGAMI_LLVM_ROOT_PATH}/lib)
include_directories(${ORIGAMI_LLVM_ROOT_PATH}/include)

add_definitions(-D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

add_executable(origami_check origami_check.cpp)
target_link_libraries(origami_check
                      LLVM-6.0
                      clangFrontend
                      clangSerialization
                      clangDriver
                      clangParse
                      clangSema
                      clangAnalysis
                      clangAST
                      clangBasic
                      clangEdit
                      clangLex
                      clangTooling
                      clangAnalysis
                      clangAST
                      clangASTMatchers
                      clangBasic
                      clangDriver
                      clangEdit
                      clangFrontend
                      clangLex
                      clangParse
                      clangStaticAnalyzerCore
                      clangStaticAnalyzerCheckers
                      clangStaticAnalyzerFrontend
                      clangRewrite
                      clangSema
                      clangSerialization
                      clangTooling
                      LLVMBitReader
                      LLVMMC
                      LLVMMCParser
                      LLVMOption
                      LLVMSupport
                      LLVMProfileData
                      LLVMCore
                      )

target_link_libraries(origami_check
                      LLVMX86AsmParser # MC, MCParser, Support, X86Desc, X86Info
                      LLVMX86Desc # MC, Support, X86AsmPrinter, X86Info
                      LLVMX86AsmPrinter # MC, Support, X86Utils
                      LLVMX86Info # MC, Support, Target
                      LLVMX86Utils # Core, Support
                      LLVMipo
                      LLVMScalarOpts
                      LLVMInstCombine
                      LLVMTransformUtils
                      #                      LLVMipa
                      LLVMAnalysis
                      LLVMTarget
                      LLVMOption # Support
                      LLVMMCParser # MC, Support
                      LLVMMC # Object, Support
                      LLVMObject # BitReader, Core, Support
                      LLVMBitReader # Core, Support
                      LLVMCore # Support
                      LLVMSupport
                      )

include(FindCurses)
target_link_libraries(origami_check
                      pthread
                      z
                      dl
                      ${CURSES_LIBRARIES}
                      )