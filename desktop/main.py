import asyncio
import logging
from websocket_client import connect_to_server

# Logging einrichten
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s",
    handlers=[logging.FileHandler("websocket_notifier.log"), logging.StreamHandler()]
)

if __name__ == "__main__":
    try:
        asyncio.run(connect_to_server())
    except KeyboardInterrupt:
        logging.info("Programm wurde vom Benutzer beendet.")
