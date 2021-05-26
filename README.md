# 499_project
Repository for Hunter CS capstone project 

## Blockchain

keeps track of the edits of the files and keep a record of the edits in the blockchain system. The edits will be based on the fat log of the file with the entire link of the file and its location across the network.

The fat file will be formated as 

Fat_info = [ ‘file_name’, ‘node_number’, ‘node_number_location’ ]

Address_info = [ ‘data’, ‘next_node_number’, ‘next_node_number_location’] 

# How to run 

make 4 nodes on pycharm

python network.py

5000-5003


1) post   -  at/launch

2) post  - write/data

on body raw json


{

    "file":"FILENAME",
    
    "data":"######",
    
    "new_data":"F100",
    
    "sender_port":"127.0.0.1:5000"
    
}

3) get -   get/chain
