// Import the required modules
const WebSocket = require('ws');  // WebSocket library for real-time communication
const http = require('http');  // HTTP module for creating the server

// Create an HTTP server to handle requests
const server = http.createServer((req, res) => {
  // Handle GET request for the root URL
  if (req.method === 'GET' && req.url === '/') {
    res.writeHead(200, { 'Content-Type': 'text/plain' });  // Set the response header to text/plain
    res.end('Hello from HTTP server!');  // Send a simple response
  }
  // Handle POST request for the /send endpoint
  else if (req.method === 'POST' && req.url === '/send') {
    let body = '';  // Variable to collect the request body

    // Collect the incoming data chunks
    req.on('data', chunk => {
      body += chunk;  // Append each chunk to the body variable
    });

    // When all data has been received, send it to WebSocket clients
    req.on('end', () => {
      console.log('Message received:', body);  // Log the received message

      // Send the message to all connected WebSocket clients
      wss.clients.forEach((client) => {
        if (client.readyState === WebSocket.OPEN) {  // Ensure the WebSocket client is open
          client.send(body);  // Send the message to the client
        }
      });

      // Respond to the HTTP client indicating the message has been sent
      res.writeHead(200, { 'Content-Type': 'text/plain' });
      res.end('Message sent!');
    });
  } else {
    // Return a 404 response for all other requests
    res.writeHead(404);
    res.end('Not found');
  }
});

// Create a WebSocket server and attach it to the HTTP server
const wss = new WebSocket.Server({ server });

// Handle new WebSocket connections
wss.on('connection', (ws) => {
  console.log('New WebSocket client connected');  // Log when a new client connects
  
  // Handle messages received from WebSocket clients
  ws.on('message', (message) => {
    console.log('Message from client:', message);  // Log the message received from the client

    // Broadcast the message to all other WebSocket clients
    wss.clients.forEach((client) => {
      if (client !== ws && client.readyState === WebSocket.OPEN) {  // Ensure the client is not the sender
        client.send(message);  // Send the message to the other clients
      }
    });
  });

  // Handle the event when a WebSocket connection is closed
  ws.on('close', () => {
    console.log('WebSocket connection closed');  // Log when the connection is closed
  });
});

// Send a ping to all connected WebSocket clients every minute
setInterval(() => {
  wss.clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {  // Check if the client is still open
      client.ping();  // Send the ping
      console.log('Ping sent to client');  // Log the ping
    }
  });
}, 60000);  // 60000 ms = 1 minute

// Start the server on port 8080
server.listen(8080, () => {
  console.log('Server is running on port 8080');  // Log when the server starts
});
