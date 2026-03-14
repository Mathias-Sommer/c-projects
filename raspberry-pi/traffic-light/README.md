# traffic-light project
To reproduce this setup you will need the following:  
1x Breadboard, 3x Resistors(10k), 3x LED, 4x Jumper wires and of course the Raspberry Pi.  

To compile the source code using gcc:
```bash
gcc traffic-light.c -o traffic-light
```  

To run the code:
```bash
./traffic-light n
```  
Where `n` is the number of cycles to run.  

## Why i made this
This is my first project on I/O with a Raspberry Pi in pure C. I figured this was a good project to learn basic C syntax and memory allocation.  
My goal is to use pure C with "default" libraries. And then create all the I/O in pure C without libraries.
