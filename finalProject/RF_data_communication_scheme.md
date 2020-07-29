RF data communication scheme
============================
arrays
------
format of arrays: `[char1, char2, checksum]`

where `checksum = (((dec(char1)*dec(char2))*13)%8+81`

correct array: `['A', 'G', 'M']`


    "A" --> 65
	"G" --> 71
	checksum = 84 --> "T"

---
table of codes
--------------
A (65) - identity

B (66) - identity

C (67) - identity

D (68) - identity

E (69) - identity

F (70) - identity

G (71) - frequency1 (Min working voltage)

H (72) - frequency2

I (73) - frequency3

J (74) - frequency4

K (75) - frequency5

L (76) - frequency6

M (77) - frequency7

N (78) - frequency8

O (79) - frequency9

P (80) - frequency10 (Max working voltage)

Q (81) - switch on/off

R (82) - checksum

S (83) - checksum

T (84) - checksum

U (85) - checksum

V (86) - checksum

W (87) - checksum

X (88) - checksum

Y (89) - checksum

Z (90) - affirmative

---
algorithm for communication between diamond and dock
----------------------------------------------------
##diamond

	diamond idle state:
		transmit '[identification, frequency, checksum]' (2 seconds) --> receive (2 seconds) (repeatedly).
		if receive same array as transmitted:
			transmits [identification, affirmative, checksum] and goes to setting state.

	diamond setting state:
		stops transmitting for 20 seconds (only receives).
		if receives [identification, frequency, checksum]:
			sets new frequency and resets timer for 20 seconds.
		else if 20 seconds is up:
			goes to idle state.

##dock

	dock (on menu for changing frequency) idle state:
		only receives
		if receives correct array:
			goes to setting state.


	dock (on menu for changing frequency) waiting state:
		transmits same array once --> wait to receive affirmative (1 second) (repeatedly for 3 seconds)
		if received affirmative: 
			goes to setting state.
		else if 3 seconds is up without receiving affirmative:
			goes to idle state.

	dock (on menu for changing frequency) setting state:
		user can select new frequency, transmits [identification, new frequency, checksum]
		if send affirmative to diamond (to exit):
			goes to idle state.

dock          diamond
out----,-> >-in(B)
	 	R
in(A)---'

when docked, in(A) = LOW & in(B) = HIGH
when not docked, in(A) = HIGH & in(B) = LOW