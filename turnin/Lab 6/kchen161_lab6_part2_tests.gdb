# Test file for Lab2_introToAVR


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f 
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n


# Example test:
#test "PINA: 0x00, PINB: 0x00 => PORTC: 0"
# Set inputs
#setPINA 0x00
#setPINB 0x00
# Continue for several ticks
#continue 2
# Set expect values
#expectPORTC 0
# Check pass/fail
#checkResult


# Add tests below
test "2 ticks -> PORTB: 0x01, state = blink1"
set state = start
continue 2
expectPORTB 0x01
expect state blink1
checkResult

test "32 ticks -> PORTB: 0x02, state = blink2"
set state = start
continue 32
expectPORTB 0x02
expect state blink2
checkResult

test "62 ticks -> PORTB: 0x04, state = blink3"
set state = start
continue 62
expectPORTB 0x04
expect state blink3
checkResult

test "92 ticks -> PORTB: 0x01, state = blink1"
set state = start
continue 92
expectPORTB 0x01
expect state blink1
checkResult

test "32 ticks, A0 = 1, 5 ticks -> PORTB: 0x02, state = wait2"
set state = start
continue 32
setPINA 0x01
continue 5
expectPORTB 0x02
expect state wait2
checkResult

test "62 ticks, A0 = 1, 5 ticks -> PORTB: 0x04, state = wait3"
set state = start
setPINA 0x00
continue 62
setPINA 0x01
continue 5
expectPORTB 0x04
expect state wait3
checkResult

test "92 ticks, A0 = 1, 5 ticks -> PORTB: 0x01, state = wait1"
set state = start
setPINA 0x00
continue 92
setPINA 0x01
continue 5
expectPORTB 0x01
expect state wait1
checkResult

test "32 ticks, A0 = 1, 5 ticks, A0 = 0, 5 ticks -> PORTB: 0x02, state = stay2"
set state = start
setPINA 0x00
continue 32
setPINA 0x01
continue 5
setPINA 0x00
continue 5
expectPORTB 0x02
expect state stay2
checkResult

test "32 ticks, A0 = 1, 5 ticks, A0 = 0, 5 ticks, A0 = 1, 5 ticks -> PORTB: 0x00, state = init"
set state = start
setPINA 0x00
continue 32
setPINA 0x01
continue 5
setPINA 0x00
continue 5
setPINA 0x01
continue 5
expectPORTB 0x00
expect state init
checkResult

test "32 ticks, A0 = 1, 5 ticks, A0 = 0, 5 ticks, A0 = 1, 5 ticks, A0 = 0, 1 tick -> PORTB: 0x01, state = blink1"
set state = start
setPINA 0x00
continue 32
setPINA 0x01
continue 5
setPINA 0x00
continue 5
setPINA 0x01
continue 5
setPINA 0x00
continue 1
expectPORTB 0x01
expect state blink1
checkResult


# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
