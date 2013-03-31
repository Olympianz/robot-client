# Traffic Monitoring Implementation
This project aims to record the traffic transformation between robotic client and servers. It is cooperated with Karma so to run the basic setup we need 3 stepsas follows:

1. Run Mobilesim under Mobilesim root directory.(Need to pick a map to run it )
    - More details can be found on Mobilesim README file on official site
    - Sample command, Mobilesim -m {path} -r p3dx
2. Run Server
    - Under server directory, create a build folder
    - In build folder, run cmake ..
    - run make
    - run ./server -rttp 8101 -sp 7272 (Client port # and Server port #)
3. Run Client
    - Under server director, create a build folder
    - In build folder, run cmake ..
    - run make
    - run ./client -hosts {path for configuration file, such as ../simulator.txt }


