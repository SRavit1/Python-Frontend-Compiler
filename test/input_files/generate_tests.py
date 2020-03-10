def gen_sia ():
	#Simple Integer Addition
	test_nums = [[0,5], [4,0], [-2,2], [1,3], [-2,5]]

	for test_no in range(len(test_nums)):
		pair = test_nums[test_no]
		test_code = "print(" + str(pair[0]) + "+" + str(pair[1]) + ");"
		filename = "SimpleIntegerAddition/sia" + str(test_no) + ".py"
		_write(test_code, filename)

def gen_sis ():
	#Simple Integer Subtraction
	test_nums = [[0,5], [4,0], [-2,2], [1,3], [5,2]]

	for test_no in range(len(test_nums)):
		pair = test_nums[test_no]
		test_code = "print(" + str(pair[0]) + "-" + str(pair[1]) + ");"
		filename = "SimpleIntegerSubtraction/sis" + str(test_no) + ".py"
		_write(test_code, filename)

def gen_sim ():
	test_nums = [[0,5], [-4,0], [-2,2], [1,3], [2,-5]]

	for test_no in range(len(test_nums)):
		pair = test_nums[test_no]
		test_code = "print(" + str(pair[0]) + "*" + str(pair[1]) + ");"
		filename = "SimpleIntegerMultiplication/sim" + str(test_no) + ".py"
		_write(test_code, filename)

def gen_sid ():
	test_nums = [[5,0], [4,2], [-0,2], [3,1], [6,-2]]

	for test_no in range(len(test_nums)):
		pair = test_nums[test_no]
		test_code = "print(" + str(pair[0]) + "/" + str(pair[1]) + ");"
		filename = "SimpleIntegerDivision/sid" + str(test_no) + ".py"
		_write(test_code, filename)

def _write (test_code, filename):
	with open(filename, 'w') as f:
		f.write(test_code)

gen_sia()
gen_sis()
gen_sim()
gen_sid()
