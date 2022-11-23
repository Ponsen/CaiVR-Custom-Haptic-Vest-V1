typedef std::function<void(String message)> OnDriveCommand;

void setupComms(IPAddress ipAdress, OnDriveCommand cb);
void sendMessage(char const* message);