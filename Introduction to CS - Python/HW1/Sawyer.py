in_file = "our_input.txt"
out_file = "output.txt"
txt = open(in_file, 'r')
newText = open(out_file, 'w')
for line in txt:
    lineSpl=line.split()
    newText.write(str(len(lineSpl))+'\n\n')
txt.close()
newText.close()
