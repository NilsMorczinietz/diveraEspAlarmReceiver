import requests
from tkinter import Tk, Label, Button, Frame
from my_secrets import divera_accesskey, divera_id

def show_popup(title, text):
    background_color = "#001e4c"
    border_color = "#52000b"
    border_width = 9  # Breite der Umrandung

    def close_window():
        root.destroy()

    def make_draggable(window):
        def on_drag(event):
            window.geometry(f"+{event.x_root}+{event.y_root}")
        
        window.bind("<B1-Motion>", on_drag)

    def link_2():
        url = f"https://app.divera247.com/statusgeber.html?status=2&accesskey={divera_accesskey}&ucr={divera_id}"
        response = requests.get(url)  # HTTP-GET-Anfrage im Hintergrund ausführen
        if response.status_code == 200:
            print("Anfrage erfolgreich ausgeführt")
            # close_window()
        else:
            print(f"Fehler: {response.status_code}")

    # Neues Tkinter-Fenster erstellen
    root = Tk()
    root.title(title)

    # Fenster ohne Titelleiste
    root.overrideredirect(True)  # Entfernt die Fensterleiste (Titlebar)

    # Fenstergröße inkl. Rahmen (400x200 + 2*border_width für Rahmen)
    content_width, content_height = 500, 200
    total_width = content_width + 2 * border_width
    total_height = content_height + 2 * border_width

    # Berechnung der Bildschirmgröße
    screen_width = root.winfo_screenwidth()
    screen_height = root.winfo_screenheight()

    # Berechnung der Position des Fensters, um es in die Mitte zu setzen
    position_top = int((screen_height - total_height) / 2)
    position_left = int((screen_width - total_width) / 2)

    # Fenstergröße und Position setzen
    root.geometry(f"{total_width}x{total_height}+{position_left}+{position_top}")
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

    status_2_button = Button(content_frame, text="Einsatzbereit", command=link_2, bg="white", fg="black", font=button_font)
    status_2_button.pack(pady=5)

    close_button = Button(content_frame, text="SCHLIESSEN", command=close_window, bg="white", fg="black", font=button_font)
    close_button.pack(pady=20)

    # Fenster immer im Vordergrund
    root.attributes('-topmost', True)

    # Mach das Fenster verschiebbar
    make_draggable(root)

    # Fenster anzeigen
    root.mainloop()