import tkinter
import ctypes

def ch():
    if entry.get() == entry2.get():
        ctypes.windll.user32.MessageBoxW(0, "HI", "WAZA", 1)

root = tkinter.Tk()
root.title("MAGIC!")
root.geometry("375x200")
label = tkinter.Label(root, text = "MAGIC!", font=('Helvetica', 20))
label.pack()
entry = tkinter.Entry(root,width=10)
entry2 = tkinter.Entry(root)
#entry(root).grid(row = 1, column = 1)
#entry.geometry("100x100")
entry.pack()
entry2.pack()
butt = tkinter.Button(root, text = "Check", command = ch)
butt.pack()
#entry = [tkinter.Entry(root)]*9
#x.pack() for x in entry
root.mainloop()
