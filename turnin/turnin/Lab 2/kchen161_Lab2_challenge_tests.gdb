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
test "A: 0, B: 0, C: 0 => D: 0x00"
setPINA 0
setPINB 0
setPINC 0
continue 5
expectPORTD 0x00
checkResult

test "A: 1, B: 1, C: 1 => D: 0x00"
setPINA 1
setPINB 1
setPINC 1
continue 5
expectPORTD 0x00
checkResult

test "A: 85, B: 85, C: 85 => D: 0x01"
setPINA 85
setPINB 85
setPINC 85
continue 5
expectPORTD 0x01
checkResult

test "A: 85, B: 85, C: 86 => D: 0x01"
setPINA 85
setPINB 85
setPINC 86
continue 5
expectPORTD 0x01
checkResult

test "A: 100, B: 0, C: 10 => D: 0x02"
setPINA 100
setPINB 0
setPINC 10
continue 5
expectPORTD 0x02
checkResult

test "A: 10, B: 0, C: 100 => D: 0x02"
setPINA 10
setPINB 0
setPINC 100
continue 5
expectPORTD 0x02
checkResult

test "A: 100, B: 100, C: 10 => D: 0x03"
setPINA 100
setPINB 100
setPINC 10
continue 5
expectPORTD 0x03
checkResult


# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
