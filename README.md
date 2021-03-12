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
 ![equation](https://latex.codecogs.com/png.latex?P%28x%29%20%3D%20a_n%20x%5En%20&plus;%20a_%7Bn-1%7D%20x%5E%7Bn-1%7D%20&plus;%20...%20&plus;%20a_0)
 

