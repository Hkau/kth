################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Board/Board.cpp \
../src/Board/BoardLocal.cpp \
../src/Board/MABoardGraph.cpp \
../src/Board/MABoardGuru.cpp \
../src/Board/MABoardSearch.cpp \
../src/Board/StaticBoard.cpp 

OBJS += \
./src/Board/Board.o \
./src/Board/BoardLocal.o \
./src/Board/MABoardGraph.o \
./src/Board/MABoardGuru.o \
./src/Board/MABoardSearch.o \
./src/Board/StaticBoard.o 

CPP_DEPS += \
./src/Board/Board.d \
./src/Board/BoardLocal.d \
./src/Board/MABoardGraph.d \
./src/Board/MABoardGuru.d \
./src/Board/MABoardSearch.d \
./src/Board/StaticBoard.d 


# Each subdirectory must supply rules for building sources it contributes
src/Board/%.o: ../src/Board/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


