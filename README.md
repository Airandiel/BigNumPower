# BigNumPower
Library for computing power of numbers, tested up to 99999^99999
Preffered instructions to build on linux, for windows use windows compatible cmake and make:

'''
mkdir build
cd build
cmake ..
make
./task1
./tests
'''

Assumptions:
- default build is done in Debug mode, for tests to work properly, 
    if you want to try to compute numbers closer to upper input range
    it is recommended to change in CMakeLists.txt file, in line 9:
    'set(CMAKE_BUILD_TYPE "Debug")'
    change "Debug" into "Release", this speeds up the program execution

- the program may not be as brief as it should, but I wanted for it
    to be able to compute numbers in given input range (0 to 99999), in 
    a resonable time.
    It is based on computing exponentiation by squaring and computing 
    multiplication using Karatsuba method. 
    For optimisation the Big number implementation is based on 
    std::vector of uint64_t. It might be changed for different type 
    for example to uint32_t, to achieve better performance on devices with 
    32-bit architecture. It was optimized for 64-bit devices.

- for tests I am using assert command, that's why it needs to be run in 
    debug mode for the tests to work properly

- the program was assumed to be short and to solve the task required,
    so the defined operators of BigNum class doesn't have validations
    and might not give the proper results if used out of context.
