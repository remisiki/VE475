import numpy as np
import math

def Ex1P1():
	c = 'EVIRE'
	for i in range(26):
		for j in c:
			print(chr((ord(j) - 65 - i) % 26 + 65), end = "")
		print(", ", end = "")

def Ex1P2():
	a = 'dont'
	b = 'ELNI'
	n = 2
	a = np.array(list(map(lambda x: ord(x) - 97, a))).reshape(n, n)
	b = np.array(list(map(lambda x: ord(x) - 65, b))).reshape(n, n)
	# print(math.gcd(round(np.linalg.det(a)), 26))
	x = np.array(
		[
			[9, 18],
			[13, 11],
		]
	)
	y = np.array(
		[
			[4, 11],
			[13, 8],
		]
	)
	print(np.dot(x, y) % 26)

def Ex2P1():
	print("\\hline")
	print(" & ", end = "")
	for i in range(26):
		alpha = chr(i + 65)
		print(alpha, end = "\\\\\n" if (i == 25) else " & ")
	print("\\hline")
	for i in range(26):
		print(i, end = " & ")
		for j in range(26):
			alpha = chr((i + j) % 26 + 65)
			print(alpha, end = "\\\\\n" if (j == 25) else " & ")
		print("\\hline")

def Ex2P2():
	plain = "TORADORA"
	key = "TAIGA"
	for i in range(len(plain)):
		print(chr((ord(key[i % len(key)]) - 65 + ord(plain[i]) - 65) % 26 + 65))



if __name__ == '__main__':
	# Ex1P1()
	Ex2P2()