from sys import exit
import re

# Get the card number


def get_valid_card_no():
    while True:
        try:
            card_no = int(input("What is the card number?\n> "))
        except:
            pass
        else:
            card_no = str(card_no)
            break
    card_len = len(card_no)
    if card_len > 16 or card_len < 13 or card_len == 14:
        print("INVALID")
        exit()
    else:
        return card_no

# Checksum


def try_checksum_pass(card_no):
    multiplier = 0
    totaller = 0
    for c in card_no[::-1]:
        product = int(c)*(multiplier + 1)
        if len(str(product)) == 2:
            totaller += int(str(product)[0])
            totaller += int(str(product)[1])
        else:
            totaller += product
        multiplier = multiplier ^ 1
    if (totaller % 10) == 0:
        return True
    else:
        return False

# Get card type


def get_card_type(card_no):
    if re.search("(^[3][47])\w+", card_no):
        return "AMEX"
    elif re.search("(^[5][12345])\w+", card_no):
        return "MASTERCARD"
    elif re.search("(^[4])\w+", card_no):
        return "VISA"
    else:
        return "INVALID"
    
    
# Main


card_no = get_valid_card_no()
if try_checksum_pass(card_no):
    print(get_card_type(card_no))
else:
    print("INVALID")
