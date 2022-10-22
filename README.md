# Sensor network simulation

Multiprocess, multithreaded random earthquake simulation using openMPI and POSIX threads

## Build

OpenMPI must be installed in the system

```shell
cmake -S . -B build && make -C build 
```

## Run

The program need to be run with `mpirun`

No arguments are taken by the program,
but inputs are required for the grid size (number of sensors per rows and columns)
and sentinel value (a character to notice the program to stop)

Example using 3 x 3 cartesian grid and sentinel value 'f':

``` shell
echo -e "3\n3\nf\n" | mpirun -np 10 --oversubscribe ./sensor_network
```

Example where the user will be asked for cartesian grid size at run time:

```shell
mpirun -np 10 --oversubscribe ./sensor_network
```

### Exit

#### Using signal

In another terminal run:

```
killall -s SIGQUIT sensor_network
``` 

Then enter a user input into the running program

#### Using sentinel value

A sentinel character value is asked to the user after program launch,
entering the same character any time after will stop the simulation.

## Debug / explicit output

By adding the macro DEBUG to the compilation, debug prints will give further information about the running program

```cmake
target_compile_options(${SENSOR_NETWORK} PRIVATE -Wall -Wextra -D DEBUG)
```

## Understand

This program is a simulation of a sensor network, composed of several logical elements :

1. Base station

The base station is the main program,
it launches multiple threads for multiple asynchronous actions

- reading thread

Listens for seismic reading from sensors,
when it receives one, it compares it with the balloon's reading,
if they match, it registers these readings into 'alert_conclusive.txt'.
It also registers any received message in 'alert_inconclusive.txt'.
It also produces the file 'metrics.txt', which contains global information about the simulation, once it is over.

- balloon thread

Produces (randomly generates) periodic seismic sensor reading,
which be compared to alerts sent by sensors

- fault detection thread

Polls each sensor to check if it is still alive,
if not (sensor randomly generate errors which cause them to stop),
exit gracefully

2. Sensors

An MPI cartesian grid of processes (aka node or sensor).
Each sensor periodically (randomly) generates seismic readings containing timestamp, magnitude, depth, coordinates.
If the magnitude is above 2.5, it sends this reading to its direct neighbours,
they will each check if their own reading has a high magnitude and if both happened in a close range.
If data matches, it sends those readings to the base station.

The algorithm of sensors as well as further explanations are described
in [docs/Sensor_network_simulation_-_Explanations.pdf](./docs/Sensor_network_simulation_-_Explanations.pdf),

3. Balloon sensor

The balloon sensor generates data just like sensors, except the coordinates can be anywhere in the defined area
(the sensors generate data in coordinates related to their MPI cartesian grid coordinates).
This data is used to cross-check the sensors data.

### Documentation

A report is available in
[docs/Sensor_network_simulation_-_Explanations.pdf](./docs/Sensor_network_simulation_-_Explanations.pdf),
it describes the internal functioning of the project
---
The source code is documented with doxygen style comments.