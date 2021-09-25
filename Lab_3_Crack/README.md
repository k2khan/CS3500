# Group7

1./ Hamad Khan, Julia Pratt, Bridget O'Reilly

2./ pass

3./ ferry

4./ used 1 thread. time was 52.819s

5./ 

    2 threads: 28.169s

    4 threads: 15.824s

    8 threads: 9.210s

    13 threads: 8.746s

    26 threads: 56.826s

6./

    1 thread  : (26^5 + 26^4 + 26^3 + 26^2 + 26) / 52.819 = 233,942.89

    2 threads : (26^5 + 26^4 + 26^3 + 26^2 + 26) / 28.169 = 438,660.58

    4 threads : (26^5 + 26^4 + 26^3 + 26^2 + 26) / 15.824 = 780,879.04

    8 threads : (26^5 + 26^4 + 26^3 + 26^2 + 26) / 9.210  = 1,341,653.63

    13 threads: (26^5 + 26^4 + 26^3 + 26^2 + 26) / 8.746  = 1,412,832.15

    26 threads: (26^5 + 26^4 + 26^3 + 26^2 + 26) / 5.791  = 2,133,764.46

7./ Our program seems to get logarithmically faster as we increase threads.
   Starts out getting much faster very quickly then doesn't get faster. We have one odd number at 26 where it takes a lot longer. There are diminishing returns as you increase the number of threads, which is why we don't consistently see out hash rates double.

8./ Our max hash rate was at 13 threads, 

9./ (26^8 + 26^7 +26^6 +26^5 + 26^4 + 26^3 +26^2 + 26) = 21,718,014,715 

10./ 21,718,014,714 / 2,133,764.46 = 10,178 seconds to search the keyspace for keysizes of one through eight.

11./ 52+62+93 = 207
     (207^6 +207^5 + 207^4 + 207^3 +207^2 + 207) / 2,133,764.46 = 37049190.26

12./ We don't know any known bugs.

13./ did the first & second 


Extra Cred:
1./ 

    abA.g8pU2Iffo: salts

    cdfnIXMyMCpPg: salts

    efgC/gw8PDKhs: salts

    ghLneTdBMxJP.: salts

    ijSsXTgIC7QRU: salts

    klr7dT7cAODsk: donot

    mn4iWfK0m76t6: skipp

    opjPsgpXaahxM: anyyy

    qr6ncfvfqecME: salts
    
2./ Salts are useful because they "randomize" hashes by appending or prepending a random known string. This makes it significantly more challenging for an attacker to use lookup tables to crack these passwords. It basically makes it a lot harder for hackers to reverse engineer in terms of password cracking.