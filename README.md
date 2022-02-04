# OpenCB
Open Computer Behavior

This project is in the early stages of development as of 2/4/2022.

Welcome to OpenCB, a project with the end-goal of allowing a computer to have its own behavior.
OpenCB mimicks the structure and functioning of the human mind rather than the human brain.
An algorithmic approach to artificial intelligence is applied rather than a neural net model,
meaning that the memories and thoughts of the AI are stored as objects in memory rather than 
abstractions in a neural net.

OpenCB works by feeding the system screenshot images of a computer monitor. The system then
determines if it is "attracted" or "repulsed" by an image, which then influences its behavior.
The system can act by typing a character into the output of the program, which is currently
just the command line, or by moving the screenshot window around on the screen to take a 
picture in another location. This allows the program to influence its environment, which then
establishes a feedback loop to its behavior.

If an action the system takes leads to an image its attracted to, it will favor that action in
similar situations, and the opposite if its repulsed. This allows the system to "learn" to
change its own environment through its actions so that it can bring about situations it likes
and avoid situations it dislikes.

The project is written in C++ for the Windows operating system and is, again, in the early 
stages of development. A UI is currently being designed to allow for interaction between a user 
and the system, but it's not functional yet. This code is provided to you as-is with no 
guarantee that it will work or even that it may not damage your computer.
