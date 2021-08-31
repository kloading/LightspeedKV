#!/usr/bin/env python

'''
import socket

TCP_IP = '127.0.0.1'
TCP_PORT = 6379
BUFFER_SIZE = 1024
MESSAGE = "Hello, World!"

GET = "get"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
s.send(GET.encode('ascii'))
data = s.recv(BUFFER_SIZE)


print ("received data:", data)
'''
from pymemcache.client.base import Client

client = Client('localhost')
client.set('some_key', 'some_value')
#result = client.get('some_key')
#print(result)

