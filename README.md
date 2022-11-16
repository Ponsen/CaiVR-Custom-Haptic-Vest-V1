Not even alpha - but some stuff works so far

# Changelog:
- updated upper and lower motor casing to fit https://www.az-delivery.de/en/products/vibrationsmotor-modul very tighly (no rattle)

## Added POC for an ESP-32 Dev Kit C V4 (https://www.az-delivery.de/products/esp-32-dev-kit-c-v4).
### Features
- UPD trafic
- registration process to host

### TODO
- actual motor mapping/control
- wifi config via phone

## Added POC for node server to controll devices
(should work on all supported node platforms)
### Features
- UDP trafic
- registration process for clients

## TODO
- actual motor mapping/control
- UI for testing and mapping
- other devices like gloves and trackers (SlimeVR?)

# Notes

## Networking on Windows
Node server might not work due to windows firewall. Either turn it off or do what I ended up doing:
To reduce latency I've added a wifi hotspot to my host machine. Usually those networks are seen as public networks. Chaning it to a private one made the node server receive UPD  packages. This might not be an issue if you're using your router.

- To elevate the hotspot open the `Local Security Policy` window (Win+R -> secpol.msc).
- Open `Network List Manager Policies`.
- Click on `Action` -> `Show All Networks`. Your Hotspot show now be visible. Double click on it
- Open Tab `Network Location` and set `Location Type` to `Private`.
- Apply and Close the window. The UPD server should now be able to receive UDP packets

-------------------------------------------

All files for the DIY vest are available here now.
Written guide in Progress.

PC client script and configs all need to be in same folder.