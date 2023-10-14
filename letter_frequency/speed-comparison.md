# Slow algorithm with `cout`:

```
printFrequencySlow() called, offset = 0, size = 95
  106786
! 0
" 0
# 0
$ 0
% 0
& 0
' 0
( 0
) 0
* 0
+ 0
, 0
- 0
. 0
/ 0
0 0
1 0
2 0
3 0
4 0
5 0
6 0
7 0
8 0
9 0
: 0
; 0
< 0
= 0
> 0
? 0
@ 0
A 0
B 0
C 0
D 0
E 0
F 0
G 0
H 0
I 0
J 0
K 0
L 0
M 0
N 0
O 0
P 0
Q 0
R 0
S 0
T 0
U 0
V 0
W 0
X 0
Y 0
Z 0
[ 0
\ 0
] 0
^ 0
_ 0
` 0
a 50504
b 6955
c 24289
d 17869
e 68480
f 3852
g 8239
h 3424
i 60883
j 321
k 0
l 31993
m 28997
n 35524
o 22363
p 15194
q 9202
r 34561
s 53500
t 53286
u 52644
v 10700
w 0
x 0
y 0
z 0
{ 0
| 0
} 0
~ 0
printFrequencySlow() ended
Time difference slow = 320 milliseconds
```

---

# Slow algorithm without `cout`:

```
Time difference slow = 254 milliseconds
```

---

# Hashing algorithm (`cout` separately):

```
Time difference fast = 6 milliseconds
  106786
! 0
" 0
# 0
$ 0
% 0
& 0
' 0
( 0
) 0
* 0
+ 0
, 0
- 0
. 0
/ 0
0 0
1 0
2 0
3 0
4 0
5 0
6 0
7 0
8 0
9 0
: 0
; 0
< 0
= 0
> 0
? 0
@ 0
A 0
B 0
C 0
D 0
E 0
F 0
G 0
H 0
I 0
J 0
K 0
L 0
M 0
N 0
O 0
P 0
Q 0
R 0
S 0
T 0
U 0
V 0
W 0
X 0
Y 0
Z 0
[ 0
\ 0
] 0
^ 0
_ 0
` 0
a 50504
b 6955
c 24289
d 17869
e 68480
f 3852
g 8239
h 3424
i 60883
j 321
k 0
l 31993
m 28997
n 35524
o 22363
p 15194
q 9202
r 34561
s 53500
t 53286
u 52644
v 10700
w 0
x 0
y 0
z 0
{ 0
| 0
} 0
~ 0
```

# Conclusion

This shows that implementing the word frequency counting algorithm with hashing (`O(n)`) makes it dozens of times faster than with a simple `O(m * n)` algorithm where `n` is the text length (`699566` in this case) and `m` is ASCII section length (`95` in this case).

It's also worth noting that the hashing algorithm will finish in the same time no matter what `m` is. So if there are, hypothetically, `5000` letters in your alphabet, it will still complete in `6` milliseconds if the text length is the same, it'll just use more memory for the alphabet array.
