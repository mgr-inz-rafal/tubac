/*
 *
 * Turbo Basic Compiler by mgr_inz_rafal.
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <rchabowski@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 *														// mgr inz. Rafal
 * ----------------------------------------------------------------------------
 */

TODO:
- Functions like open_IOCB() should not be inlined, but parametrized and called (moved to runtime, the same way as POP_TO)
- Should usage of uninitialized variable in BASIC spawn them and init with 0?
- "Reflections" for generating runtime functions (boost::fusion - somehow?)
- synth_PEEK_TO() and similar methods should not use "cfg.get_number_interpretation()->get_size()" byt pointer size
- handle integer and hex_integer in the same way

DONE:
- FAKE_POP combo should be extracted to separate subroutine
- Get rid of direct references to ___TUBAC___INTEGER_0 (and _1) from runtime_integer::synth_COMPARE_FR0_FR1_EQUAL()
- Add printing of newline after PRINT
- "synth(...)" instead of "synth() << ..." to allow automatic endlining (no automatic endlining, but E_ symbol introduced)
- Multiplication with 0 on either side should return 0
- Replace all hardcoded "___TUBAC___*" with token provide (for example inruntime_base::synth_SOUND())
- Common code for handling stacks
- Extract stack class
- Consider combining "Creating compiler variable" with "Init pointer", so we can have single line in assembly: "variable dta a(initial value)" (Not possible since MADS cannot initialize values on ZP)
- Generate all stacks with one loop (iterate over "generator::stacks")
- Add support for negative numbers (___TUBAC___INTEGER_-17 is causing errors)
- Unit tests from files
