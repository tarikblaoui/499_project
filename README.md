# 499_project
Repository for Hunter CS capstone project 

# Blockchain

keeps track of the edits of the files and keep a record of the edits in the blockchain system. The edits will be based on the fat log of the file with the entire link of the file and its location across the network.

The fat file will be formated as 

Fat_info = [ ‘file_name’, ‘node_number’, ‘node_number_location’ ]

Address_info = [ ‘data’, ‘next_node_number’, ‘next_node_number_location’] 

## How to run 

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



# Regression

Files:

regressions.py

regressions.ipynb (jupyter notebook for visualization)

Life Expectancy DataLife Expectancy Data.csv (from https://www.kaggle.com/kumarajarshi/life-expectancy-who)


Usage:

command line with no arguments:

python3 regression.py

WILL NOT WORK UNLESS YOU DOWNLOAD THE DATASET ABOVE


command line with arguments:

python3 regression.py %filename% %variable 1% %variable 2% ... %variable n% %target variable%

%filename% must point to a csv

%variables% must be part of the header row inside %filename%

Will only work with numerical data, and best with continuous, not catagorical data

# Networking

Running the server:

./server portnumber

Running the clients:

./client portnumber message

Portnumber for server and clients should all be the same



