from uuid import uuid4
from flask import Flask, jsonify, request
from BlockChain import Blockchain

app = Flask(__name__)
node_identifier = str(uuid4()).replace('-', '')
blockchain = Blockchain()


# launch this at the start to add the files into the network nodes
@app.route('/at/launch', methods=['GET', 'POST'])
def at_launch():
    blockchain.at_launch()
    return jsonify('DONE')


# adds the file and the data along with the links to each node
@app.route('/pre/defined/values', methods=['POST'])
def pre_defined_values():
    values = request.get_json()
    required = ['file', 'node', 'address', 'file2', 'node2', 'address2',
                'data', 'next_node', 'next_address', 'data2', 'next_node2', 'next_address2']
    if not all(keys in values for keys in required):
        return 'Error', 400
    blockchain.pre_defined_values(values['file'], values['node'], values['address'],
                                  values['file2'], values['node2'], values['address2'],
                                  values['data'], values['next_node'], values['next_address'],
                                  values['data2'], values['next_node2'], values['next_address2'])
    return jsonify('DONE')


# create the file using json
@app.route('/create/data', methods=['POST'])
def create_data():
    values = request.get_json()
    required = ['file', 'data']
    if not all(keys in values for keys in required):
        return 'Error', 400
    blockchain.create_data(values['file'], values['data'])
    return jsonify()


# sends fat and address
@app.route('/send/data', methods=['POST'])
def send_data():
    values = request.get_json()
    required = ['fat', 'address']
    if not all(keys in values for keys in required):
        return 'Error', 400
    blockchain.send_data(values['fat'], values['address'])
    return jsonify()


# update the data after validation and consensus is completed
@app.route('/update/data', methods=['POST'])
def update_data():
    values = request.get_json()
    required = ['fat', 'address']
    if not all(keys in values for keys in required):
        return 'Error', 400
    blockchain.update_data(values['fat'], values['address'])
    return jsonify()


# edit version 1
@app.route('/write/data', methods=['POST'])
def write_data():
    values = request.get_json()
    required = ['file', 'data', 'new_data', 'sender_port']
    if not all(keys in values for keys in required):
        return 'Error', 400
    blockchain.write_data(values['file'], values['data'], values['new_data'], values['sender_port'])
    return jsonify()


# edit version 2
@app.route('/write', methods=['POST'])
def write():
    values = request.get_json()
    required = ['file', 'data', 'sender_port']
    if not all(keys in values for keys in required):
        return 'Error', 400
    blockchain.write(values['file'], values['data'], values['sender_port'])
    return jsonify()


# returns fat info [file name, node, address ]
@app.route('/get/fat/info', methods=['GET'])
def get_fat_info_info():
    response = blockchain.fat_info
    return jsonify(response)


# return address info [data, next node, next address]
@app.route('/get/address/info', methods=['GET'])
def get_address_info_info():
    response = blockchain.address_info
    return jsonify(response)


# get fat log from editing node
@app.route('/get/fat', methods=['GET'])
def get_fat_info():
    response = blockchain.fat
    return jsonify(response)


# get fat links to validate editing nodes request
@app.route('/fat/info', methods=['GET', 'POST'])
def node_info():
    values = request.get_json()
    required = ['file']
    if not all(keys in values for keys in required):
        return 'Error', 400
    response = blockchain.get_fat_info(values['file'])
    return jsonify(response)


# get address info
@app.route('/address/info', methods=['GET', 'POST'])
def data_info():
    values = request.get_json()
    required = ['address']
    if not all(keys in values for keys in required):
        return 'Error', 400
    response = blockchain.get_address_info(values['address'])
    return jsonify(response)


# update version 2
@app.route('/update', methods=['POST'])
def update():
    values = request.get_json()
    required = ['data', 'address']
    if not all(keys in values for keys in required):
        return 'Error', 400
    blockchain.update_address_info(values['data'], values['address'])
    return jsonify()


# create a new block once validation and consensus is valid
@app.route('/new/block/info', methods=['POST'])
def new_block_info():
    blockchain.new_block_info()
    return jsonify()


# make sure fat is correctly linked
@app.route('/check/edit', methods=['POST'])
def check_edit():
    values = request.get_json()
    required = ['file', 'sender_port']
    if not all(keys in values for keys in required):
        return 'Error', 400
    blockchain.check_fat(values['file'], values['sender_port'])
    return jsonify(), 200


# returns the blockchain
@app.route('/get/chain', methods=['GET'])
def get_chain():
    response = {
        'chain': blockchain.chain,
        'length': len(blockchain.chain)
    }
    return jsonify(response), 200


# validate the previous hash of the blockchain
@app.route('/validate/previous/chain', methods=['GET'])
def validate_previous_chain():
    response = blockchain.validate_previous_chain()
    if response:
        return jsonify(), 200
    return jsonify(), 201


if __name__ == '__main__':
    from argparse import ArgumentParser

    parser = ArgumentParser()
    parser.add_argument('-p', '--port', default=5000, type=int, help='port to listen to')
    args = parser.parse_args()
    port = args.port
    app.run(host='127.0.0.1', port=port)
