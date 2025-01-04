import logging
from threading import Thread
from win10toast_click import ToastNotifier
from popup_window import show_popup

def show_notification(title, text):
    try:
        # Benachrichtigung anzeigen
        notifier = ToastNotifier()
        notifier.show_toast(
            title=title,
            msg=text,
            duration=30,  # Dauer in Sekunden
            threaded=True,
            icon_path=''
        )
        # Fenster mit Benachrichtigung anzeigen
        Thread(target=show_popup, args=(title, text)).start()
    except Exception as e:
        logging.error(f"Fehler beim Anzeigen der Benachrichtigung: {e}")
