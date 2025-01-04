from tkinter import Tk, Label, Button

def show_popup(title, text):
    def close_window():
        root.destroy()

    # Neues Tkinter-Fenster erstellen
    root = Tk()
    root.title(title)
    root.geometry("400x200")
    root.configure(bg="red")

    # Fenster immer im Vordergrund
    root.attributes('-topmost', True)

    # Titel und Text als Label hinzufügen
    label_title = Label(root, text=title, font=("Helvetica", 16), bg="red", fg="white")
    label_title.pack(pady=10)

    label_text = Label(root, text=text, font=("Helvetica", 12), bg="red", fg="white", wraplength=380)
    label_text.pack(pady=10)

    # Button zum Schließen des Fensters
    close_button = Button(root, text="Schließen", command=close_window, bg="white", fg="black")
    close_button.pack(pady=10)

    # Fenster anzeigen
    root.mainloop()
