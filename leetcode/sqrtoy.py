def integerSqrt(n):
	if n < 0:
		error "integerSqrt works for only nonnegative inputs"
    	else if n < 2:
        return n
    else:
        # Recursive call:
        smallCandidate = integerSqrt(n >> 2) << 1
        largeCandidate = smallCandidate + 1
        if largeCandidate*largeCandidate > n:
            return smallCandidate
        else:
            return largeCandidate

def main:
	integerSqrt(9)


if __name__ == '__main__':
    main()

