cmake_minimum_required(VERSION 3.14)
project(PatientManagementSystem)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python and pybind11
find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG REQUIRED)

# Add pybind11 module
pybind11_add_module(patient_system 
    cpp/bindings.cpp
)

# Include directories
target_include_directories(patient_system PRIVATE cpp/include)

# Copy the built module to python directory
add_custom_command(TARGET patient_system POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:patient_system>
        ${CMAKE_SOURCE_DIR}/python/
)
