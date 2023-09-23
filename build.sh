/usr/bin/clang++ -fcolor-diagnostics -fansi-escape-codes -g ./*.cpp ./OSSpecific/*.mm ./JSONmodel/*.cpp ./UI/*.cpp -o ./Binary/ClipboardManager -framework Cocoa -framework Foundation -std=c++11 -lncurses
chmod +x ./Binary/ClipboardManager
