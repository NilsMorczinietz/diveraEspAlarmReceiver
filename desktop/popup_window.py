from tkinter import Tk, Label, Button, Frame

def show_popup(title, text):
    background_color = "#001e4c"
    border_color = "#52000b"
    border_width = 9  # Breite der Umrandung

    def close_window():
        root.destroy()

    # Neues Tkinter-Fenster erstellen
    root = Tk()
    root.title(title)

    # Fenstergröße inkl. Rahmen (400x200 + 2*border_width für Rahmen)
    content_width, content_height = 500, 200
    total_width = content_width + 2 * border_width
    total_height = content_height + 2 * border_width

    root.geometry(f"{total_width}x{total_height}")
    root.configure(bg=border_color)  # Rahmenfarbe setzen

    # Frame für den Inhalt des Fensters erstellen
    content_frame = Frame(root, bg=background_color, width=content_width, height=content_height)
    content_frame.place(x=border_width, y=border_width)  # Platzierung im Inneren des Rahmens

    # Propagation deaktivieren, um feste Größe zu erzwingen
    content_frame.pack_propagate(False)

    # Schriftart definieren
    title_font = ("Arial", 17, "bold")
    text_font = ("Arial", 12, "bold") 
    button_font = ("Arial", 10, "bold")

    # Inhalt in das Frame hinzufügen
    label_title = Label(content_frame, text=title, font=title_font, bg=background_color, fg="white")
    label_title.pack(pady=(20, 5))  # Abstand oben und unten

    label_text = Label(content_frame, text=text, font=text_font, bg=background_color, fg="white", wraplength=content_width - 40)
    label_text.pack(pady=5)

    close_button = Button(content_frame, text="SCHLIESSEN", command=close_window, bg="white", fg="black", font=button_font)
    close_button.pack(pady=20)

    # Fenster immer im Vordergrund
    root.attributes('-topmost', True)

    # Fenster anzeigen
    root.mainloop()