import winsound
import re
import urllib.request
import msvcrt

freqs = (329.63,349.23,369.99,392.00,415.30,440.00,466.16,493.88,523.25,554.37,587.33,622.25,659.25,698.46,739.99,783.99,830.61,880.00,932.33,
         987.77,1046.50,1108.73,1174.66,1244.51,1318.51,1396.91,1479.98,1567.98,1661.22,1760.00,1864.66,1975.53,2093.00,2217.46,2349.32,2489.02,
         2637.02,2793.83,2959.96,3135.96,3322.44,3520.00,3729.31,3951.07,4186.01)

def playS (Gstring, fret, noteLength = 100): #input: Gstring (char) and fret (int), output: sound
    fretLetters = ['E','A','D','G','B','e'] #add later - other tunings: make 'tuning = 0' variable and map it to interval lists?
    intervals = [0,5,10,15,19,24]               #or make other function that uses playS and tuning input?
    dic = dict(zip(fretLetters,intervals)) #maps fretLetter to interval
    interval = dic[Gstring]
    return winsound.Beep(int(round(freqs[interval+fret])),noteLength)

def readGstring (line): #input: line such as e|---2--3--4--...\n
    arr = []  #output: returns list - if char is number makes tuple in the right place, else-writes None
    for i in range(len(line)):
        if line[i].isdigit():
            arr.append((line[0],int(line[i])))
        else:
            arr.append(None)
    return arr

def combineGstrings (lines): #input: array of arrays from readGstring. output: one list that has all the notes by order
    arr = [] #initializes the list we start with to contain all the data
    maxLen = min(len(w) for w in lines) #FIX THIS?>TAKES MINIMAL LENGTH INSTEAD OF RUNNING ON EVERY COLUMN!!
    for i in range(maxLen):
        for j in range(len(lines)):
            if lines[j][i] != None:
                arr.append(lines[j][i])
                break #adds the note from the highest string in every column!
    return arr

def tabSecToNoteArray (Gstrings): #input: 1 tab section. output: array of ('note',fretNumber)
    spl = Gstrings.split('\n')
    arr = []
    for w in spl:
        arr.append(readGstring(w))
    arr = combineGstrings(arr)
    return arr

def addressToTabSecArray (adrs): #site address-> output: tab section array(strings)
    data = urllib.request.urlopen(adrs).read()
    wanted = re.findall(r'e[^{A-G},e]*\\nB[^{A-G},e]*\\nG[^{A-G},e]*\\nD[^{A-G},e]*\\nA[^{A-G},e]*\\nE[^{A-G},e,\\n]*',str(data),re.M)
    for i in range(len(wanted)):
        wanted[i] = re.sub(r'(\\n)+','\\n',wanted[i]) #multiple \\n in a row -> only one (for splitting later)
    return wanted

def tabSecArrayToNotes (TSA): #TabSecArray['e|--|','B|2-|'] -> output: note tuple array
    arr = [tabSecToNoteArray(sec) for sec in TSA]
    arr = [note for sublist in arr for note in sublist] #flatten 2d array
    return arr

def ma(adrs): #gets address and plays the song
    arr = addressToTabSecArray(adrs)
    arr = tabSecArrayToNotes(arr)
    for note in arr:
        playS(*note)
    return

def ma1(adrs):
    arr = addressToTabSecArray(adrs)
    arr = tabSecArrayToNotes(arr)
    print("Play!")
    for i in range(len(arr)):
        k1 = msvcrt.getch().decode('ASCII')
        if k1 == 'b':
            playS(*arr[i])
    return

while True:
    ma1("https://tabs.ultimate-guitar.com/m/metallica/nothing_else_matters_ver7_tab.htm")
    input()
    
"""
main: make tuples of (fretLetter,fretNumber), and for each - playS
re.findall(r'e\|.*\nB\|.*\nG\|.*\nD\|.*\nA\|.*\nE\|.*\n',"e|-- #!wanted = [w for w in wanted if (len(w) > 7)] #
----------------------|------------------------|\n
B|------------------------|------------------------|\n
G|-------0----------------|-------0----------------|\n
D|----4-----4-----4-----4-|----4-----4-----4-----4-|\n
A|-2-----------2-----5----|-0-----------0-----5----|\n
E|------------------------|------------------------|\n")
"""

"""
playS('E',1)
playS('A',3)
playS('D',0); playS('E',1); playS('E',4); playS('A',2); playS('D',2)
playS('E',4); playS('A',2); playS('D',2); playS('G',1); playS('A',2)
playS('D',2); playS('G',1); playS('B',0); playS('e',0)"""
