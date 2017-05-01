#Challenge 4
import urllib.request
import re #regular expressions - check it

url = "http://www.pythonchallenge.com/pc/def/linkedlist.php?nothing="
noth = "12345"
z = ""
data = ""
i = 0
while (True):
    data = urllib.request.urlopen(url+noth)
    line = str(data.readline())
    print (line)
    for ch in line:
        if ch.isdigit():
            z+=ch
    noth = z
    z = ""
