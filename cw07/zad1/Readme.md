### Running instruction

In order to run this project:

1. Download it

2. Open two separate terminals in project directory (cw07/zad1)

3. Type following command in whichever terminal:
```bash
make all
```

4. In first terminal run trucker process with 3 arguments, for instance:
```bash
./trucker 200 50 600
```
where:

- 200 stands for truck_capacity

- 50 stands for belt_size - how many packs the conveyor belt can contain

- 600 stands for belt_capacity - maximum weight of all packs combined

5. In second terminal run loader process with 1 argument (second argument is optional - it's the number of cycles for whole loading operation),
for instance:
```bash
./loader 20 3
```

where:

- 20 stands for pack weight
- 3 stands for number of cycles

