const wsUri = "ws://127.0.0.1:8081";
const websocket = new WebSocket(wsUri);

websocket.addEventListener("open", () => {
    console.log("CONNECTED");
});

websocket.addEventListener("message", (event) => {
    try {
        const message = JSON.parse(event.data);

        console.log(
            `RECEIVED: ${message.type}: ${message.payload}`
        );
    } catch (error) {
        console.error("Received invalid JSON:", event.data);
    }

    processMessage(message);
});

websocket.addEventListener("close", (event) => {
    console.log("CONNECTION CLOSED");
});

websocket.addEventListener("error", (error) => {
    console.error("WEBSOCKET ERROR:", error);
});
