cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(MY_PROJECT_NAME cppCode)
project(${MY_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


######## Cplex Dependency
find_package(CPLEX REQUIRED)
if(CPLEX_FOUND)
    include_directories(${CPLEX_INCLUDE_DIR})
    link_directories(${CPLEX_LIBRARY})
endif()

if(CPLEX_CONCERT_FOUND)
    include_directories(${CPLEX_CONCERT_INCLUDE_DIR})
    link_directories(${CPLEX_CONCERT_LIBRARY})
endif()

if(CPLEX_ILOCPLEX_FOUND)
    include_directories(${CPLEX_ILOCPLEX_INCLUDE_DIR})
    link_directories(${CPLEX_ILOCPLEX_LIBRARY})
endif()

if(CPLEX_CP_FOUND)
    include_directories(${CPLEX_CP_INCLUDE_DIR})
    link_directories(${CPLEX_CP_LIBRARY})
endif()


######## Complier message
set(CMAKE_BUILD_TYPE Debug)
message(STATUS "System: ${CMAKE_SYSTEM}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

######## Set output directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build//bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)

######## Set C++ 11 standard
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++17" COMPILER_SUPPORTS_CXX11)
if (COMPILER_SUPPORTS_CXX11)
    if (CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    endif ()
else ()
    message(STATUS "Fatal Error: Your compiler doesn't support c++17, please install it!")
endif ()

######## find all the source files.
include_directories(include)




######## Set executable file name, and add the source files for it.
add_executable(${MY_PROJECT_NAME} src/main.cpp
        src/entities/Team.cpp
        include/entities/Team.h
        src/entities/Match.cpp
        include/entities/Match.h
        src/entities/Round.cpp
        include/entities/Round.h
        src/entities/TimeTable.cpp
        include/entities/TimeTable.h
        src/utils/TeamManager/TeamManager.cpp
        include/utils/TeamManager/TeamManager.h
        src/tournament/Tournament.cpp
        include/tournament/Tournament.h
        src/utils/Scheduling/Schedulers/Scheduler.cpp
        include/utils/Scheduling/Schedulers/Scheduler.h
        src/utils/Writers/Writer.cpp
        include/utils/Writers/Writer.h
        src/utils/Simulations/Simulator.cpp
        include/utils/Simulations/Simulator.h
        src/utils/Scheduling/Strategies/SchedulingStrategy.cpp
        include/utils/Scheduling/Strategies/SchedulingStrategy.h
        src/utils/Scheduling/Strategies/MinimizePointsDifference.cpp
        include/utils/Scheduling/Strategies/MinimizePointsDifference.h
        src/utils/Scheduling/Strategies/MinimizeRatingDifference.cpp
        include/utils/Scheduling/Strategies/MinimizeRatingDifference.h
        src/utils/Scheduling/Strategies/MinimizePointsDifferenceLaterRounds.cpp
        include/utils/Scheduling/Strategies/MinimizePointsDifferenceLaterRounds.h
        src/utils/Metrics/SecondaryMetrics.cpp
        include/utils/Metrics/SecondaryMetrics.h
        src/utils/Simulations/MonteCarloSimulator.cpp
        include/utils/Simulations/MonteCarloSimulator.h
        src/utils/Scheduling/Strategies/MaximizePointsDifference.cpp
        include/utils/Scheduling/Strategies/MaximizePointsDifference.h
        src/utils/Simulations/OutcomeProbabilities.cpp
        include/utils/Simulations/OutcomeProbabilities.h
        src/utils/Metrics/RoundStatistics.cpp
        include/utils/Metrics/RoundStatistics.h
        src/tournament/DoubleRoundRobin.cpp
        include/tournament/DoubleRoundRobin.h
        src/utils/Scheduling/Schedulers/DoubleRoundRobinScheduler.cpp
        include/utils/Scheduling/Schedulers/DoubleRoundRobinScheduler.h
        src/utils/TeamManager/PremierLeague.cpp
        include/utils/TeamManager/PremierLeague.h
        src/entities/HapSet.cpp
        include/entities/HapSet.h
        src/utils/Metrics/Bounds.cpp
        include/utils/Metrics/Bounds.h
        src/utils/Scheduling/Strategies/MinimizeIrrelevanceStaticSchedule.cpp
        include/utils/Scheduling/Strategies/MinimizeIrrelevanceStaticSchedule.h
        src/tournament/ChampionsLeague.cpp
        include/tournament/ChampionsLeague.h
        src/utils/TeamManager/ChampionsLeagueTeamManager.cpp
        include/utils/TeamManager/ChampionsLeagueTeamManager.h
        src/utils/Scheduling/Schedulers/ChampionsLeagueScheduler.cpp
        include/utils/Scheduling/Schedulers/ChampionsLeagueScheduler.h
        src/utils/Metrics/ChampionsLeagueMetrics.cpp
        include/utils/Metrics/ChampionsLeagueMetrics.h
        src/utils/TeamManager/NFLTeamManager.cpp
        include/utils/TeamManager/NFLTeamManager.h
        include/utils/Writers/TeamBoundsWriter.h
        src/utils/Metrics/TeamBounds.cpp
        include/utils/Metrics/TeamBounds.h
        src/utils/Metrics/RandomForestFeatures.cpp
        include/utils/Metrics/RandomForestFeatures.h
        src/utils/Metrics/Rankings.cpp
        include/utils/Metrics/Rankings.h
        src/utils/Scheduling/Strategies/StaticObjective.cpp
        src/utils/Scheduling/Strategies/StaticObjective.h
)

######## Add Dependency Library
target_link_libraries(${MY_PROJECT_NAME} cplex-library cplex-concert ilocplex cplex-cp)

