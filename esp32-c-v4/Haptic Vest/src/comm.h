typedef std::function<void(String message)> OnMessageReceived;

void setupComms(IPAddress ipAdress, OnMessageReceived cb);
void sendMessage(char const* message);
void sendHeartBeat();