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
In another terminal run:
```
killall -s SIGQUIT sensor_network
``` 