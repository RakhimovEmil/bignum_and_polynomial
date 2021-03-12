# mp::bignum

Long non-negative integer non-template class. 
Such a number is analogous to unsigned int, 
but it can have an unlimited number of digits
in advance.

The class has the following list of 
operations:

* explicit construction from a decimal string (std :: string). 
* copying
* explicit conversions to an integer (uint32_t). 
If the number does not fit in uint32_t, 
the value is narrowed (like int -> short)
* the ability to use in conditional expressions
* the class should not allow subtraction
* function to get the decimal string 
representation to_string
* input/output to standard streams
* operations +, *, +=, *=  
---
# mp::polynomial

Implements polynomial abstraction
 $ P(x) = a_n x^n + a_{n-1} x^{n-1} + … a_0 $
 

