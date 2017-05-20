# The Tubac
ATARI Turbo Basic Compiler by mgr_inz_rafal.

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
Compiler fully supports arithmetic expressions with parenthesis.

### Atari Basic
|Command|Status|Comment|
|-------------|:-----------:|------|
|ABS|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|AND|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|ASC|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|ATN|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|BYE|![#000000](https://placehold.it/15/000000/000000?text=+)|It makes no sense in compiled program.|
|CLOAD|![#000000](https://placehold.it/15/000000/000000?text=+)|Compiled program will not allow loading from cassettes without OS.|
|CHR$|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|CLOG|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|CLOSE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|CLR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|COLOR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|COM|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|CONT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|COS|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|CSAVE|![#000000](https://placehold.it/15/000000/000000?text=+)|Compiled program will not allow saving to cassettes without OS.|
|DATA|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|DEG|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|DIM|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|DOS|![#000000](https://placehold.it/15/000000/000000?text=+)|Compiled program will not exit to DOS.|
|DRAWTO|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|END|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|ENTER|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|EXP|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|FOR|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)||
|GET|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|GOSUB|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)||
|GOTO|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)|Only direct jumps to line numbers. ```GOTO A+8``` is not (and will not be) supported. I did it once in the past, just for fun, but it didn't bring any value besides slowing the jumps down tremendously.|
|GRAPHICS|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|IF...THEN|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)||
|INPUT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|INT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|LEN|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|LET|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)||
|LIST|![#000000](https://placehold.it/15/000000/000000?text=+)|It doesn't make sense to list compiled program.|
|LOAD|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|LOCATE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|LOG|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|LPRINT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|NEW|![#000000](https://placehold.it/15/000000/000000?text=+)|Not gonna happen.|
|NEXT|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)||
|NOT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|NOTE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|ON...GOTO|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|ON...GOSUB|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|OPEN|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|OR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|PADDLE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|PEEK|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)||
|PLOT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|POINT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|POKE|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)||
|POP|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|POSITION|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|PRINT|![#1589F0](https://placehold.it/15/1589F0/000000?text=+)|Comma separator is not supported yet. It acts in the same way as semicolon|
|PTRIG|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|PUT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|RAD|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|READ|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|REM|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|RESTORE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|RETURN|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)||
|RND|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|RUN|![#000000](https://placehold.it/15/000000/000000?text=+)|Nope.|
|SAVE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|SETCOLOR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|SGN|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|SIN|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|SOUND|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)|Seems to be producing incorrect sounds... Need to investigate more.|
|SQR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|STATUS|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|STEP|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)||
|STICK|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|STRIG|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|STOP|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|STR$|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|TRAP|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|USR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|VAL|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|XIO|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
### Turbo Basic XL
|Command|Status|Comment|
|-------------|:-----------:|------|
|BLOAD|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|BRUN|![#000000](https://placehold.it/15/000000/000000?text=+)||
|DELETE|![#000000](https://placehold.it/15/000000/000000?text=+)||
|DIR|![#000000](https://placehold.it/15/000000/000000?text=+)||
|RENAME|![#000000](https://placehold.it/15/000000/000000?text=+)||
|LOCK|![#000000](https://placehold.it/15/000000/000000?text=+)||
|UNLOCK|![#000000](https://placehold.it/15/000000/000000?text=+)||
|DPOKE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|DPEEK|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|MOVE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|-MOVE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|%PUT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|%GET|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|CLS|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|PAUSE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|RENUM|![#000000](https://placehold.it/15/000000/000000?text=+)|No line numbers - no renumeration.|
|DEL|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|DUMP|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|TRACE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|INPUT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|GO TO|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|DSOUND|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|# (line labels)|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|GO#|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|ON...EXEC|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|ON...GO#|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|RAND|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|TIME|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|TIME$|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|INKEY$|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|INSTR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|UINSTR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|ERR|![#000000](https://placehold.it/15/000000/000000?text=+)||
|ERL|![#000000](https://placehold.it/15/000000/000000?text=+)||
|REPEAT...UNTIL|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)||
|WHILE...WEND|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)||
|DO...LOOP|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)||
|IF...ELSE...ENDIF|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)||
|PROC...ENDPROC|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)||
|EXEC|![#00ff00](https://placehold.it/15/00ff00/000000?text=+)||
|CIRCLE|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|FCOLOR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|FILLTO|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|PAINT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|TEXT|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|HEX$|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|DEC|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|DIV|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|MOD|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|FRAC|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|TRUNC|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|&|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|!|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|EXOR|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||
|%0 - %3 (constants)|![#f03c15](https://placehold.it/15/f03c15/000000?text=+)||

### Extension
- Support for ```EXEC``` in the middle of the line
