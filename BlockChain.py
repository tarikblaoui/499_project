import hashlib
import json
from urllib.parse import urlparse
import requests


class Blockchain:
    def __init__(self):
        self.fat_info = []
        self.address_info = []
        self.fat = []
        self.chain = []
        self.nodes = set()
        self.new_block(previous_hash='0')
        self.create_nodes(address='http://127.0.0.1:5000')
        self.create_nodes(address='http://127.0.0.1:5001')
        self.create_nodes(address='http://127.0.0.1:5002')
        self.create_nodes(address='http://127.0.0.1:5003')
        self.temp_address = []
        self.temp_fat = []

    # create nodes
    def create_nodes(self, address):
        parsed_url = urlparse(address)
        if parsed_url.netloc:
            self.nodes.add(parsed_url.netloc)
        elif parsed_url.path:
            self.nodes.add(parsed_url.path)
        else:
            raise ValueError('Invalid')

    # add predetermined data into nodes
    def at_launch(self):
        requests.post('http://127.0.0.1:5000/pre/defined/values', json={
            'file': 'FILENAME', 'node': '5000', 'address': 0,
            'file2': 'FILENAME2', 'node2': '5000', 'address2': 1,
            'data': '!!!!!!', 'next_node': '5001', 'next_address': 0,
            'data2': '!!!!!!', 'next_node2': '5001', 'next_address2': 1})

        requests.post('http://127.0.0.1:5001/pre/defined/values', json={
            'file': 'FILENAME', 'node': '5001', 'address': 0,
            'file2': 'FILENAME2', 'node2': '5001', 'address2': 1,
            'data': '@@@@@@', 'next_node': '5002', 'next_address': 0,
            'data2': '@@@@@@', 'next_node2': '5002', 'next_address2': 1})

        requests.post('http://127.0.0.1:5002/pre/defined/values', json={
            'file': 'FILENAME', 'node': '5002', 'address': 0,
            'file2': 'FILENAME2', 'node2': '5002', 'address2': 1,
            'data': '######', 'next_node': '5003', 'next_address': 0,
            'data2': '######', 'next_node2': '5003', 'next_address2': 1})

        requests.post('http://127.0.0.1:5003/pre/defined/values', json={
            'file': 'FILENAME', 'node': '5003', 'address': 0,
            'file2': 'FILENAME2', 'node2': '5003', 'address2': 1,
            'data': '$$$$$$', 'next_node': '4999', 'next_address': -1,
            'data2': '$$$$$$', 'next_node2': '4999', 'next_address2': -1})

    def pre_defined_values(self, file, node, address, file2, node2, address2,
                           data, next_node, next_address, data2, next_node2, next_address2):
        self.fat_info.append([file, node, address])
        self.address_info.append([data, next_node, next_address])
        self.fat_info.append([file2, node2, address2])
        self.address_info.append([data2, next_node2, next_address2])

    # create file
    # split data into 4 clusters
    # send this data into the nodes each with a part of the data
    def create_data(self, file, data):
        length = len(data) % 4
        count = 0
        while count != length:
            data = data + ' '
            count = count + 1
        n = int(len(data) / 4)
        parts = [data[i:i + n] for i in range(0, len(data), n)]

        file_to_send_counter = 0
        counter = 0
        port = '500'
        location = len(self.fat_info)
        network = self.nodes
        for node in network:
            fat_info = [file, port + str(counter), location]
            counter = counter + 1
            address_info = [parts[file_to_send_counter], port + str(counter), location]
            file_to_send_counter = file_to_send_counter + 1
            requests.post(f'http://127.0.0.1:{node}/send/data', json={
                'fat': fat_info, 'address': address_info
            })

    def send_data(self, fat, address):
        self.fat[fat[2]].append(fat)
        self.address_info[fat[2]].append(address)

    # adding fat log and address info into the nodes
    def adding_info(self, file):
        start = 0
        start2 = 0
        self.temp_fat.append((requests.post('http://127.0.0.1:5000/fat/info', json={'file': file})).json())
        self.temp_address.append((requests.post(f'http://127.0.0.1:{self.temp_fat[start][1]}/address/info',
                                                json={'address': self.temp_fat[start][2]})).json())
        start = start + 1
        while start != 4:
            self.temp_fat.append((requests.post(f'http://127.0.0.1:{self.temp_address[start2][1]}/fat/info',
                                                json={'file': file})).json())
            self.temp_address.append((requests.post(f'http://127.0.0.1:{self.temp_fat[start][1]}/address/info',
                                                    json={'address': self.temp_fat[start][2]})).json())
            start = start + 1
            start2 = start2 + 1

    # validates the hash of the block to prevent tampering
    def check_hash(self):
        network = self.nodes
        for node in network:
            response = requests.get(f'http://{node}/validate/previous/chain')
            if response.status_code != 200:
                return False

    # write version 1,
    # keep track of the node that we want to edit and this will be sent to the node once all validation and consensus
    # passes
    # if all network consensus is achieved we update the data
    def write_data(self, file, data, new_data, sender_port):
        self.check_hash()
        self.adding_info(file)

        node_to_edit = 5000
        for x in self.temp_address:
            if x[0] == data:
                x[0] = new_data
                break
            node_to_edit = node_to_edit + 1

        for x in self.fat_info:
            self.fat.append(x)

        consensus = 0
        network = self.nodes
        for node in network:
            if node != sender_port:
                response = requests.post(f'http://{node}/check/edit', json={
                    'file': file, 'sender_port': sender_port
                })
                if response.status_code == 200:
                    consensus = consensus + 1
        check = self.consensus(consensus, sender_port)
        if check:
            self.update(check, node_to_edit, new_data)

    # write version 2
    # breaks the data into 4 clusters
    # create a fat log
    # validate the fat log with other nodes
    # send the updates to replace the data on all the nodes
    def write(self, file, data, sender_port):
        length = len(data) % 4
        count = 0
        while count != length:
            data = data + ' '
            count = count + 1
        n = int(len(data) / 4)
        parts = [data[i:i + n] for i in range(0, len(data), n)]
        file_to_send_counter = 0
        counter = 0
        port = '500'
        location = 0
        for x in parts:
            while x[0] != file:
                location = location + 1

        while counter < 4:
            self.fat.append([file, port + str(counter), location])
            counter = counter + 1
            self.temp_address.append([parts[file_to_send_counter], port + str(counter), location])
            file_to_send_counter = file_to_send_counter + 1

        consensus = 0
        network = self.nodes
        for node in network:
            if node != sender_port:
                response = requests.post(f'http://{node}/check/edit', json={
                    'file': file, 'sender_port': sender_port
                })
                if response.status_code == 200:
                    consensus = consensus + 1
        check = self.consensus(consensus, sender_port)
        if check:
            network = self.nodes
            for node in network:
                fat_info = [file, port + str(counter), location]
                counter = counter + 1
                address_info = [parts[file_to_send_counter], port + str(counter), location]
                file_to_send_counter = file_to_send_counter + 1
                requests.post(f'http://127.0.0.1:{node}/update/data', json={
                    'fat': fat_info, 'address': address_info
                })
            network = self.nodes
            for node in network:
                requests.post(f'http://{node}/new/block/info', json={})

    # edit the update for all the nodes edit version 2
    def update_data(self, fat, address):
        self.fat[fat[2]] = fat
        self.address_info[fat[2]] = address

    # update the data in node we want to edit for edit version 1
    def update(self, check, node_to_edit, new_data):
        if check:
            requests.post(f'http://127.0.0.1:{node_to_edit}/update', json={
                'data': new_data, 'address': self.temp_fat[1][2]
            })

            network = self.nodes
            for node in network:
                requests.post(f'http://{node}/new/block/info', json={})
        else:
            self.temp_address = []
            self.temp_fat = []
            self.fat = []

    # makes sure the fat is valid and correctly linked
    def check_fat(self, file, sender_port):
        self.adding_info(file)
        temp_fat = []
        for x in self.temp_fat:
            self.fat.append(x)

        temp_fat.append(requests.get(f'http://{sender_port}/get/fat').json())
        if self.fat == temp_fat:
            return True
        else:
            return False

    # network must agree on the request
    def consensus(self, accepts, sender_port):
        total = 0
        network = self.nodes
        for x in network:
            if x != sender_port:
                total = total + 1
        if accepts / total > 0.5:
            return True
        else:
            return False

    # creates the block
    def new_block_info(self):
        last_block = self.last_block
        previous_hash = self.hash(last_block)
        self.new_block(previous_hash)

    # add the block into the blockchain and reset the values for the next request
    def new_block(self, previous_hash):
        block = {
            'index': len(self.chain) + 1,
            'fat_log': self.fat,
            'previous_hash': previous_hash or self.hash(self.chain[-1])
        }
        self.fat = []
        self.temp_address = []
        self.temp_fat = []
        self.chain.append(block)
        return block

    # makes sure the blockchain is valid before a request is made
    def validate_previous_chain(self):
        previous_block = self.chain[0]
        counter = 1
        while counter < len(self.chain):
            current_block = self.chain[counter]
            previous_hash = self.hash(previous_block)
            if current_block['previous_hash'] != previous_hash:
                return False
            previous_block = current_block
            counter += 1
        return True

    # get fat info
    def get_fat_info(self, file):
        for data in self.fat_info:
            if data[0] == file:
                return data
            else:
                return False

    # get address info based on the fat info index 1 and 2
    def get_address_info(self, address):
        return self.address_info[address]

    # update the info based on edit version 1
    def update_address_info(self, data, address):
        self.address_info[address][0] = data

    @staticmethod
    def hash(block):
        block_hash = json.dumps(block, sort_keys=True).encode()
        return hashlib.sha256(block_hash).hexdigest()

    @property
    def last_block(self):
        return self.chain[-1]
