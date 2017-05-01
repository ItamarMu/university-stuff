from tkinter import *

root = Tk()
 
myContainer1 = Frame(root)
myContainer1.pack()

button1 = Button(myContainer1)      ### (1)
button1["text"]= "Hello, World!"    ### (2)
button1["background"] = "green"     ### (3)
button1.pack()	                    ### (4)

root.mainloop()
