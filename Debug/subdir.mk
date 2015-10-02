################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Database.cpp \
../DatalogProgram.cpp \
../Graph.cpp \
../Interpreter.cpp \
../Main.cpp \
../Node.cpp \
../Parameter.cpp \
../Parser.cpp \
../Predicate.cpp \
../Relation.cpp \
../Rule.cpp \
../Scheme.cpp \
../Token.cpp \
../Tokenizer.cpp \
../Tuple.cpp 

OBJS += \
./Database.o \
./DatalogProgram.o \
./Graph.o \
./Interpreter.o \
./Main.o \
./Node.o \
./Parameter.o \
./Parser.o \
./Predicate.o \
./Relation.o \
./Rule.o \
./Scheme.o \
./Token.o \
./Tokenizer.o \
./Tuple.o 

CPP_DEPS += \
./Database.d \
./DatalogProgram.d \
./Graph.d \
./Interpreter.d \
./Main.d \
./Node.d \
./Parameter.d \
./Parser.d \
./Predicate.d \
./Relation.d \
./Rule.d \
./Scheme.d \
./Token.d \
./Tokenizer.d \
./Tuple.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


