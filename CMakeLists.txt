cmake_minimum_required(VERSION 3.10)

project(Minimize)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
	
	lab2_new.cpp
	automates_classes/Automate.h
	automates_classes/mealy_automate/MealyAutomate.cpp
	automates_classes/mealy_automate/MealyAutomate.h
	automates_classes/moore_automate/MooreAutomate.cpp
	automates_classes/moore_automate/MooreAutomate.cpp
	functions/Functions.cpp
	functions/Functions.h
)

add_executable(Minimize ${SOURCE_FILES})