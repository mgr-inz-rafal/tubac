# The Tubac
Turbo Basic Compiler by mgr_inz_rafal.

## License
This project is licensed under "THE BEER-WARE LICENSE" (Revision 42).

<rchabowski@gmail.com> wrote this project. As long as you retain this notice you
can do whatever you want with this stuff. If we meet some day, and you think
this stuff is worth it, you can buy me a beer in return.

Yours,
mgr inż. Rafał

## Scope
This project aims to provide a tool that helps transfer the ATARI Turbo Basic listings into binary code that can be executed directly on the 6502 processor.

![Alt text](https://g.gravizo.com/svg?digraph%20G%20%7B%0A%20%20%20Start%20%5Bshape%3Dbox%2C%20style%3Dfilled%2C%20color%3Dchartreuse%5D%3B%0A%20%20%20End%20%5Bshape%3Dbox%2C%20style%3Dfilled%2C%20color%3Dred%5D%3B%0A%20%20%20TUBAC%20%5Bshape%3Ddoubleoctagon%2C%20style%3D%22filled%2Cdashed%22%20color%3Dcadetblue1%2C%20xlabel%3D%22You%20are%20here!%22%5D%3B%0A%20%20%20Start%20-%3E%20%22ATARI%20Turbo%20Basic%20Listing%22%20%5Bshape%3Dbox%5D%3B%0A%20%20%20%22ATARI%20Turbo%20Basic%20Listing%22%20-%3E%20TUBAC%3B%0A%20%20%20TUBAC%20-%3E%20%226502%20assembly%20code%22%3B%0A%20%20%20%226502%20assembly%20code%22%20-%3E%20%22ATARI%20executable%22%3B%0A%20%20%20%22ATARI%20executable%22%20-%3E%20End%20%5Bshape%3Dbox%5D%3B%0A%20%7D)
