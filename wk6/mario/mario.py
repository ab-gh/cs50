while True:
    try:
        n = int(input("Height: "))
    except:
        print("Please enter a number")
    else:
        if n > 8 or n < 1:
            print("Please enter a number between 1 and 8")
        else:
            break
for i in range(n):
    print(" " * (n-i-1), end="")
    print("#" * (i+1), end="  ")
    print("#" * (i+1))