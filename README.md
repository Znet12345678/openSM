# openSM
openSM is a new project of mine in which I aim to make an open source social media platform written in C. I aim to make it so that you can upload files,direct message someone, write to your story and much much more. As of now the things that I have done are as follows:
Client and server can communicate well.
Client and server can authenticate
You can post to your wall(very buggy)
You can read others walls(Very buggy)
Some I/O Functions implemented that will help in the future
# How to Use It?
There are two components that you're likely intersted in. And that's the client and the server
Both are easy to use, for the server run server/server.proto CONFIG_FILE_NAME
for the client run client/client.proto IP_ADDRESS
# Config File
The config file that is included in this repo is config.example
There are four feilds that are required as of now and those are maxConnections,maxUsers,user_path, and story_path.
maxConnections specifies how many people can be logged into the server at once.
maxUsers says how many users can exist on the server, whether they are logged in or out (right now the config parser reads it into a variable in a structure but the
server does nothing with it)
user_path is the path where the server stores the files for handling users.
story_path is where the the server stores the story files
these two CANNOT be the same directory
# Example config
maxConnections: 100 //100 people can be connected at once
maxUsers: 200 //200 users can exist on the system
user_path: /root/opensm_server/users //User files are stored in this directory
story_path: /root/opensm_server/stories //Story files are stored in this directory
file_path: /root/opensm_server/files //Stores user files in this directory
# Using the server
All you have to do is execute the binary(server/server.proto) and pass your config file as an argument
From there it will parse the config and run the server
# Using the client
In order to use the client there has to be a server running somewhere that you want to connect to. Let's say you're the one running that server.
To connect to that server you would run client/client.proto 127.0.0.1 and that will connect to the server
There are several commands that you can issue
# lsusr
lsusr lists all the users on the system
# ls
lists all the posts that everyone has made
# ws
Writes a story under your username The story is everything after the 1st arg
# Compilation
This version has only been tested under linux, so it most likely will not work on mac or windows. I already have a Mac OS X port that you can find under my repositories
but it is out of date. On a linux system just type "make" and it should compile
# Server output
The output of the server if all goes well should be as follows
(Executed by: server/server.proto config.example)
openSM Server V0.1-Alpha //Version output
Starting up
Initializing values
Seting socket options
Binding port
Listening
Waiting for connections ...
Once a client connects you will get a very verbose message such as this:
//Socket test
Recieving Info from Client
Waiting for initial character
Got byte!
Recieving info
//User header
Checking to see if name root is avaliable
Login successful for user: root
Done
Reciving command
and then much more will be outputed based on what the client decides to do
#Easy Fix(usually) Server Runtime Errors
Error opening file or any other File, I/O Error
This could be due to any number of things, check the perror output for the exact error. Most often it would be a permission issue
so try running it as root
Couldn't bind to port, most likely this is due to the server crashing or being shut down abruptly. Try restarting the shell and if that doesn't work
reboot your computer. If that doesn't work check your firewall.
# Client output
Again the client will be very verbose and will dump some of the bytes sent to it. All you really have to worry about is the authentication
If all goes well you should see at the bottom "Successfully logged in"
From that shell you should be able to run the command mentioned previously.
