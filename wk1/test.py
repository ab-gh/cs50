card_no = int(input("Card number: "))
working_no = card_no
multiplier = 1
while True:
    working_digit = (card_no // multiplier) % 10
    working_no = working_no // 10
    print(working_digit, " ... " , working_no)
    multiplier = multiplier * 10
    if working_no == 0:
        quit()

