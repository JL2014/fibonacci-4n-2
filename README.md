# fibonacci-4n-2

a(n) = Fibonacci(4*n + 2)

- a(n-1) = ((7+3\*sqrt(5))^n - (7-3\*sqrt(5))^n + 2\*((7+3\*sqrt(5))^(n-1) - ((7-3\*sqrt(5))^(n-1)))) / (3*(2^n)\*sqrt(5))
- a(n) = ((3 + sqrt(5))^(1 + 2\*n) - (3 - sqrt(5))^(1 + 2\*n)) / (2^(1 + 2\*n)\*sqrt(5))

https://oeis.org/A033890 - a(n) = 7*a(n-1) - a(n-2), n > 1; a(0)=1, a(1)=8\
https://oeis.org/A033890/b033890.txt
