#By Tarik Blaoui
#Class CSCI 49900
#Porfessor Soumik Dey
#This file is for the group project

import random
from random import choice


def write_block(disk_nb,block,next_disk_nb,next_block):
	try:
		if (next_block < 10):
			next_block = "0"+str(next_block) 
	except:
		pass 
	start_number = block*204
	place_holder = ''
	for i in range(200):
		place_holder = place_holder + "|"

	place_holder = place_holder + " " 

	with open("disks/disk%s.txt" % disk_nb, 'r+') as myfile:
		data = myfile.read()
		myfile.seek(start_number)

		newData = str(next_disk_nb) + str(next_block)  + place_holder
		print("---------------")
		print(newData)

		myfile.write(newData)




init_string = ""

for i in range(99):
	for y in range(200):
		init_string = init_string + "|"
	init_string = init_string + " "

list_disk_files = ["disk0.txt","disk1.txt","disk2.txt","disk3.txt"]

for d in list_disk_files:
	with open("disks/" + d, "w+") as f:
		f.write(init_string)



total_block0 = 99
total_block1 = 99
total_block2 = 99
total_block3 = 99

list_block0 = []
list_block1 = []
list_block2 = []
list_block3 = []

x = 0
while (x < 99):
	list_block0.append(x)
	list_block1.append(x)
	list_block2.append(x)
	list_block3.append(x)
	x += 1

mega_list  =  []
mega_list.append(list_block0)
mega_list.append(list_block1)
mega_list.append(list_block2)
mega_list.append(list_block3)

iteration = 0
while (len(mega_list) > 0):
	exclude = []
	print(mega_list)
	for i in  range(0,len(mega_list)):
		print("out of range err %s" % i)
		print(mega_list[i])
		if (len(mega_list[i]) == 0):  
			exclude.append(i)


	
	random_disk  =  choice([i for i in range(0, len(mega_list) ) if i not in exclude])
	print(random_disk)
	try:
		random_positon = random.randint(0, len(mega_list[random_disk])) -1
	
		random_block = mega_list[random_disk][random_positon]
	except ValueError:
		random_block = 0
	
	if iteration == 0:
		random_disk = 0
		random_positon = 0
		random_block = 0
		iteration = iteration + 1 
	



	mega_list[random_disk].pop(random_positon)

	for i in  range(0,len(mega_list)):
		if (len(mega_list[i]) == 0):  
			exclude.append(i)

	try:
		random_disk_next  =  choice([i for i in range(0, len(mega_list) ) if i not in exclude])
	except IndexError:
		random_disk_next = "|"

	try:
		if random_disk_next != "|":
			random_positon_next = random.randint(0, len(mega_list[random_disk_next])) -1
	
			random_block_next = mega_list[random_disk_next][random_positon_next]
		else:
			random_block_next = "||"
	except ValueError:
		random_disk_next = 0


	#mega_list[random_disk_next].pop(random_block_next)

	print("disk: %s | block: %s ---> disk: %s | block: %s" % (random_disk,random_block,random_disk_next,random_block_next))
	print("megalist: %s" % (len(mega_list)))
	try:
		print("list0: %s" % (len(mega_list[0])))
	except IndexError:
		pass
	try:
		print("list1: %s" % (len(mega_list[1])))
	except IndexError:
		pass
	try:
		print("list2: %s" % (len(mega_list[2])))
	except IndexError:
		pass
	try:
		print("list3: %s" % (len(mega_list[3])))
	except IndexError:
		pass


	write_block(random_disk,random_block,random_disk_next,random_block_next)

	if len(mega_list[0]) == 0  and len(mega_list[1]) == 0 and len(mega_list[2]) == 0 and len(mega_list[3]) == 0:
		print("Done")
		break









