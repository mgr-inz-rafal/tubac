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

## Supported commands
### Atari Basic
|Command|Status|Comment|
|-------------|-------------|------|
|ABS|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|AND|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|ASC|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|ATN|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|BYE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|CLOAD|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|CHR$|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|CLOG|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|CLOSE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|CLR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|COLOR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|COM|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|CONT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|COS|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|CSAVE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|DATA|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|DEG|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|DIM|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|DOS|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|DRAWTO|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|END|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|ENTER|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|EXP|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|FOR|![#00ff00](https://placehold.it/15/00ff00/000000?text=+) `Supported`||
|GET|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|GOSUB|![#c5f015](https://placehold.it/15/c5f015/000000?text=+) `Supported`||
|GOTO|![#c5f015](https://placehold.it/15/c5f015/000000?text=+) `Supported`|Only direct jumps to line numbers. GOTO A+8 is not (and will not be) supported. I did it once in the past, just for fun, but it didn't bring any value besides slowing the GOTO down tremendously.|
|GRAPHICS|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|IF...THEN|![#c5f015](https://placehold.it/15/c5f015/000000?text=+) `Supported`||
|INPUT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|INT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|LEN|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|LET|![#1589F0](https://placehold.it/15/1589F0/000000?text=+) `Partially supported`|But you can simply write the assignment without LET and it'll work.|
|LIST|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|LOAD|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|LOCATE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|LOG|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|LPRINT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|NEW|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|NEXT|![#c5f015](https://placehold.it/15/c5f015/000000?text=+) `Supported`||
|NOT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|NOTE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|ON...GOTO|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|ON...GOSUB|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|OPEN|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|OR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|PADDLE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|PEEK|![#c5f015](https://placehold.it/15/c5f015/000000?text=+) `Supported`||
|PLOT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|POINT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|POKE|![#c5f015](https://placehold.it/15/c5f015/000000?text=+) `Supported`||
|POP|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|POSITION|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|PRINT|![#1589F0](https://placehold.it/15/1589F0/000000?text=+) `Partially supported`|Currently only prints single integer expressions.|
|PTRIG|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|PUT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|RAD|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|READ|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|REM|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|RESTORE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|RETURN|![#c5f015](https://placehold.it/15/c5f015/000000?text=+) `Supported`||
|RND|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|RUN|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|SAVE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|SETCOLOR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|SGN|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|SIN|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|SOUND|![#c5f015](https://placehold.it/15/c5f015/000000?text=+) `Supported`|Seems to be producing incorrect sounds... Need to investigate more.|
|SQR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|STATUS|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|STEP|![#c5f015](https://placehold.it/15/c5f015/000000?text=+) `Supported`||
|STICK|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|STRIG|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|STOP|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|STR$|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|TRAP|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|USR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|VAL|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||
|XIO|![#f03c15](https://placehold.it/15/f03c15/000000?text=+) `Not supported`||


### Turbo Basic XL
