# PROCEDURE TO SUBSTRACT TWO NUMBERS WITHOUT BORROWING

- 1. we calculate the nine's or ones complementent of the subtrahend
eg: the nine's complement of 124 is 999-124 = 875

- 2. we then sum the nine's complement of the subtrahend to the minuend eg: 240+875 = 1115

- 3. lastly, we then add 1 and substract 1000 = 1115 + 1 = 1116 - 1000 = 116

_we never borrow in this operation_



# IMPORTANT NOTE: THIS PROCEDURE EXACTLY AS IT IS RIGHT NOW ONLY WORKS FOR NON-NEGATIVE RESULTS, FOR THAT, WE NEED TO PROCEED A LITTLE BIT DIFFERENT.



When doing substraction where the minuend is minor than the subtrahend,
the steps are the same. actually, there is a nice approach that we need to keep present everytime, in a substraction, where the subtrahend is minor than the minuend, we can reverse that, and the result will be the same but negative.

Considering the earlier example. Where we had 240-124 = 116 well, we now know that 124-240 = -116.

The exact steps will be this:

- 1. we calculate the nine's or ones complementent of the subtrahend
eg: the nine's complement of 240 is 999-240 = 759

- 2. we then sum the nine's complement of the subtrahend to the minuend eg: 124+759 = 883

- 3. lastly, instead of adding  1 and substract 1000, we simply substract the result from 999 = 999 - 883 = 116 (we keep in mind that this is negative = -116).


## We can see that the procedure is the same until the third step, by that time we should check if we have a carry, if we do, then the result is positive, so we substract 1000 and add 1, otherwise, we calculate the nines complement of the sum that we get in the second step and keep in mind that it is negative


The ten's complement is equal to the nine's complement plus one.
It allows us handle negative and positive numbers in a easier way, and simplifies the way we can do substractions, since we get rid of the borrowing.
We can express a negative number by first knowing the digits we dealing, the range of numbers we are dealing with and specifying a division between them.
E.G: 3 digits and we specify the range between 0 and 1000. the division will be right in the middle: 500, so, we know now that 0 is 0, 1 is 1, 2 is 2, 498 is 498, 499 is 499 but 500 is -500 and 501 is -499