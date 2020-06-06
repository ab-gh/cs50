import sys
import re

string = input("Input text\n> ")

let = re.compile(r'([A-z])', re.M)
sen = re.compile(r'([A-Z][^\.!?]*[\.!?])', re.M)

letters = len(let.findall(string))
words = len(string.split())
sentences = len(sen.findall(string))

index = 0.0588 * ((letters/words)*100) - 0.296 * ((sentences/words)*100) - 15.8

if index >= 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print("Grade", round(index))