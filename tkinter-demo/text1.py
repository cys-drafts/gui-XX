from Tkinter import *

KEYWORD = "keyword"
kw_list = ["static","int","long","void","return","char","for","while","do",
           "switch","case","if","else","break","goto","const","auto","float"]

class Editor:
    def __init__(self, text):
        text.tag_config(KEYWORD, foreground="red")
        text.bind('<KeyPress-space>', self.wordHandler)
        text.bind('<KeyPress-Return>', self.newlineHandler)
        self.text = text
        self.index_start = text.index(INSERT)
    def wordHandler(self, event):
        currIndex = text.index(INSERT)
        word = text.get(self.index_start, currIndex)
        print word
        if word in kw_list:
            text.tag_add(KEYWORD, self.index_start, currIndex)
        self.index_start = currIndex + "+1c"
    def newlineHandler(self, event):
        self.index_start = text.index(INSERT) + "+1c"
    
win = Tk()
text = Text(win)

text.pack()

editor = Editor(text)

win.mainloop()
