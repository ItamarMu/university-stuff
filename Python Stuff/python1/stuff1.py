import urllib.request
import re

data = urllib.request.urlopen("https://www.google.com/").read()
wanted = re.findall(r'<title>(.*)</title>',str(data),re.I|re.M)
print(wanted)
input()
