## ECSE 457 / COMP 310 - Operating Systems
### Assignment 1 - RPC - Felix Simard (260865674)
#####

###### Running the code
Here are the instructions to run my assignment.

Connecting to *lab2-4.cs.mcgill.ca* (the server used thoughout my assignment):

    ssh fsimar6@lab2-4.cs.mcgill.ca

Running the **backend**:

    cd ~/path/to/my/assignment
    cd a1
    make rpc
    ./backend     // will default to 0.0.0.0 11657
    (or)
    ./backend <HOST> <PORT>


Running the **frontend** (in an new terminal):

    cd ~/path/to/my/assignment
    cd a1  
    ./frontend     // will default to 0.0.0.0 11657
    (or)
    ./frontend <HOST> <PORT>

###### Difficulties Encountered

- The MIMI server was not allowing me to connect my frontend to a running backend (Failed to create server error). I tried to look into the Dicussion forum threads to find answers, emailed TAs, but no success to fix my MIMI server. Hence, the *lab2-4.cs.mcgill.ca* server was used to complete the assignment.
- There was some confusion about the functionalities that "exit", "quit", and "shutdown" should have. I did my best to follow the grading rubric, the assignment PDF and the answered threads on the forum.
