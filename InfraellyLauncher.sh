if [ ! -f ./infraelly.bin ]
then
    echo "infraelly not compiled."
    exit 1
fi

if [ ! -x ./infraelly.bin ]
then
    echo "setting executable bit on infraelly.bin"
    chmod +x infraelly.bin
fi

running=1
mode=0
unknownMode=0

S_IP=127.0.0.1
S_PORT=1337

while [ $running -eq 1 ]
do
    clear
    echo "                         ____"
    echo "      __                / __ \                                /\     /\ "
    echo "     /_/               / /  \/                               / /    / / "
    echo "     __    ____       / /_    ____     ____       ____      / /    / / "
    echo "    / /   / __ \     / ___\  / __ \   / __ \     / __ \    / /    / /   /\  /\ "
    echo "   / /   / /  \ \   / /     / /  \/  / /  \ \   / ____/   / /    / /   / / / / "
    echo "  / /_  / /   / /  / /     / /       \ \__/ /_  \  \___  / /_   / /_   \ \/ / "
    echo "  \__/  \/    \/   \/      \/         \______/   \____/  \__/   \__/    \  / "
    echo "                                                                        / / "
    echo " ______________________________________________________________________/ / "
    echo "/   ____________________________________________________________________/ "
    echo "\__/"
    echo "                                               Copyright Tony Huynh 2007-2010"
    echo 

    echo "Please select mode:"
    echo "[1] Standard (client)"
    echo "[2] Server"
    echo "[3] Mapper"
    echo "[4] Animator"
    echo "[5] Item editor"
    echo "[6] Test Bed"
    echo "[7] Server + clientx2 (ie, connect to self)"
    echo "[8] Two Clients"
    echo "[9] Quit"

    if [ $unknownMode -eq 1 ]
    then
        echo "Unknown recognised mode: $mode"
        unknownMode=0
    fi
     
    read mode
    echo 

    case $mode in
        "1")
            echo "Infraelly Client"
            
            echo "Server IP: \c"
            read S_IP
            echo "Game port: \c"
            read S_PORT

            echo "Launching Infraelly: $S_IP: $S_PORT"
            ./infraelly.bin;;

        "2")
            echo "Infraelly Server"
            echo "Game port: \c"
            read S_PORT

            echo "Launching Server at port $S_PORT"
            ./infraelly.bin -server;;

        "3")
            echo "Starting Map Editor"
            ./infraelly.bin -mapper;;

        "4")
            echo "Starting Animator"
            ./infraelly.bin -animator;;

        "5")
            echo "Starting Item Editor"
            ./infraelly.bin -item;;

        "6")
            echo "Launching Test Area"
            ./infraelly.bin -test;;

        "7")
            ./infraelly.bin -server &
            ./infraelly.bin &
            ./infraelly.bin &
            exit 0;;
            
        "8")
            echo "Infraelly Client x2"

            echo "Server IP: \c"
            read S_IP
            echo "Game port: \c"
            read S_PORT

            echo "Launching Infraelly: $S_IP: $S_PORT x2"
            ./infraelly.bin &
            ./infraelly.bin &
            exit 0;;

        "9")
            running=0;;

        "666")
            echo ERROR
            ./infraelly.bin -crash;;
            
        *)
            unknownMode=0;;
    esac
done

