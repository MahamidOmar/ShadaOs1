cmake_minimum_required(VERSION 3.25)
project(Ios)

set(CMAKE_CXX_STANDARD 11)

add_executable(Ios main.cpp Commands.h BuiltInCommand.h ChangeDirCommand.h ChmodCommand.h ExternalCommand.h ForegroundCommand.h GetCurrDirCommand.h JobsCommand.h JobsList.h KillCommand.h PipeCommand.h QuitCommand.h RedirectionCommand.h ShowPidCommand.h signals.h SmallShell.h smash.cpp)
