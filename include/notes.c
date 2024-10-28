/*

Make error for too many arguments in exec argv

Safe Callof should exit

*/

/*

c4a2c8% ./minishell
minishell: env | grep p
GIO_LAUNCHED_DESKTOP_FILE=/usr/share/applications/code.desktop
XDG_SEAT_PATH=/org/freedesktop/DisplayManager/Seat0
DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/101858/bus
GNOME_DESKTOP_SESSION_ID=this-is-deprecated
PATH=/home/myakoven/.local/share/junest/bin:/home/myakoven/.local/share/junest/bin:/home/myakoven/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
SESSION_MANAGER=local/c4a2c8.42berlin.de:@/tmp/.ICE-unix/9512,unix/c4a2c8.42berlin.de:/tmp/.ICE-unix/9512
XDG_SESSION_PATH=/org/freedesktop/DisplayManager/Session0
KRB5CCNAME=FILE:/tmp/krb5cc_101858_NWT2lK
GPG_AGENT_INFO=/run/user/101858/gnupg/S.gpg-agent:0:1
XDG_DATA_DIRS=/usr/share/ubuntu:/usr/share/gnome:/home/myakoven/.local/share/flatpak/exports/share:/var/lib/flatpak/exports/share:/usr/local/share:/usr/share:/var/lib/snapd/desktop
CHROME_DESKTOP=code-url-handler.desktop
GIT_ASKPASS=/usr/share/code/resources/app/extensions/git/dist/askpass.sh
VSCODE_GIT_ASKPASS_MAIN=/usr/share/code/resources/app/extensions/git/dist/askpass-main.js
minishell: env | grep PATH
XDG_SEAT_PATH=/org/freedesktop/DisplayManager/Seat0
PATH=/home/myakoven/.local/share/junest/bin:/home/myakoven/.local/share/junest/bin:/home/myakoven/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
XDG_SESSION_PATH=/org/freedesktop/DisplayManager/Session0
minishell: export PATH=hello
minishell: env | grep PATH
msh: grep: command not found
free(): double free detected in tcache 2
free(): double free detected in tcache 2
free(): double free detected in tcache 2
zsh: IOT instruction (core dumped)  ./minishell
*/

// CLEANING ISSUE