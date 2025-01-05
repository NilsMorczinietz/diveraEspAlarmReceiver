from tkinter import Tk, Label, Button, Frame, Canvas
import ctypes
import platform
import requests
from my_secrets import divera_accesskey, divera_id

# DPI-Awareness aktivieren
def set_dpi_awareness():
    if platform.system() == "Windows":
        try:
            ctypes.windll.shcore.SetProcessDpiAwareness(2)  # Aktiviert Per-Monitor DPI-Awareness
        except Exception:
            pass

set_dpi_awareness()

def create_round_button(parent, text, bg_color, outline_color, command):
    """Erstellt einen runden Button in einem Canvas."""
    canvas = Canvas(parent, width=110, height=110, bg="#001e4c", highlightthickness=0)
    oval = canvas.create_oval(5, 5, 105, 105, fill=bg_color, outline=outline_color, width=2)
    canvas.create_text(55, 55, text=text, font=("Segoe UI", 5, "bold"), fill="white", tags="button_text")

    # Binde die Klickfunktion an den Button
    canvas.tag_bind(oval, "<Button-1>", lambda event: command())
    canvas.tag_bind("button_text", "<Button-1>", lambda event: command())

    return canvas

def make_request(url):
    """Führt eine GET-Anfrage mit dem angegebenen URL aus."""
    try:
        response = requests.get(url)
        if response.status_code == 200:
            print(f"Anfrage erfolgreich: {url}")
        else:
            print(f"Fehler bei der Anfrage ({response.status_code}): {url}")
    except requests.RequestException as e:
        print(f"Anfrage fehlgeschlagen: {e}")

def show_popup(title, text):
    background_color = "#001e4c"
    border_color = "#52000b"
    border_width = 9

    def close_window():
        root.destroy()

    def make_draggable(window):
        def on_drag(event):
            window.geometry(f"+{event.x_root}+{event.y_root}")
        
        window.bind("<B1-Motion>", on_drag)

    root = Tk()
    root.title(title)

    # DPI-Skalierung setzen
    root.tk.call("tk", "scaling", 2.5)  # 2.0 = Hohe DPI-Skalierung

    root.overrideredirect(True)

    content_width, content_height = 800, 380
    total_width = content_width + 2 * border_width
    total_height = content_height + 2 * border_width

    screen_width = root.winfo_screenwidth()
    screen_height = root.winfo_screenheight()

    position_top = int((screen_height - total_height) / 2)
    position_left = int((screen_width - total_width) / 2)

    root.geometry(f"{total_width}x{total_height}+{position_left}+{position_top}")
    root.configure(bg=border_color)

    content_frame = Frame(root, bg=background_color, width=content_width, height=content_height)
    content_frame.place(x=border_width, y=border_width)

    content_frame.pack_propagate(False)

    # Optimierte Schriftart
    title_font = ("Segoe UI", 15, "bold")
    text_font = ("Segoe UI", 10, "bold")
    button_font = ("Segoe UI", 8, "bold")

    label_title = Label(content_frame, text=title, font=title_font, bg=background_color, fg="white")
    label_title.pack(pady=(20, 5))

    label_text = Label(content_frame, text=text, font=text_font, bg=background_color, fg="white", wraplength=content_width - 40)
    label_text.pack(pady=5)

    # Rahmen für die Buttons
    button_frame = Frame(content_frame, bg=background_color)
    button_frame.pack(pady=10)

    # Buttons erstellen
    buttons = [
        {"text": "Einsatzbereit", "bg_color": "#1f2e49", "outline_color": "#134f0d", 
         "url": f"https://app.divera247.com/statusgeber.html?status=2&accesskey={divera_accesskey}&ucr={divera_id}"},
        {"text": "5 Minuten", "bg_color": "#1f2e49", "outline_color": "#1d7a12", 
         "url": f"https://app.divera247.com/statusgeber.html?status=3&accesskey={divera_accesskey}&ucr={divera_id}"},
        {"text": "10 Minuten", "bg_color": "#1f2e49", "outline_color": "#38962d", 
         "url": f"https://app.divera247.com/statusgeber.html?status=4&accesskey={divera_accesskey}&ucr={divera_id}"},
        {"text": "30 Minuten", "bg_color": "#1f2e49", "outline_color": "#966e2d", 
         "url": f"https://app.divera247.com/statusgeber.html?status=5&accesskey={divera_accesskey}&ucr={divera_id}"},
        {"text": "Nicht verfügbar", "bg_color": "#1f2e49", "outline_color": "#962d2d", 
         "url": f"https://app.divera247.com/statusgeber.html?status=6&accesskey={divera_accesskey}&ucr={divera_id}"},
    ]

    for i, btn in enumerate(buttons):
        button = create_round_button(
            parent=button_frame,
            text=btn["text"],
            bg_color=btn["bg_color"],
            outline_color=btn["outline_color"],
            command=lambda url=btn["url"]: make_request(url)
        )
        button.grid(row=0, column=i, padx=5)

    # Schließen-Button
    close_button = Button(content_frame, text="SCHLIESSEN", command=close_window, bg="white", fg="black", font=button_font)
    close_button.pack(pady=20)

    root.attributes('-topmost', True)

    make_draggable(root)
    root.mainloop()