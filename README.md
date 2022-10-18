# SensorNetwork

Multiprocess, multithreaded random earthquake simulation


## Usage
### Running
```
mkdir build
cd build
cmake ..
make
```

Example using 2 x 2 grid:
```
echo -e "2\n2\nf\n" | mpirun -np 5 --oversubscribe ./sensor_network
```
### Exiting
#### Using signal value
In another terminal run:
```
killall -s SIGQUIT sensor_network
``` 
Then enter a user input into the running program

#### Using sentinel value
Enter user specified sentinel value during run-time

## Debug
By adding the macro DEBUG to the compilation, debug prints will give further information about the running program

```cmake
target_compile_options(${SENSOR_NETWORK} PRIVATE -Wall -Wextra -D DEBUG)
```
