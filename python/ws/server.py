#!/usr/bin/env python

import tornado.ioloop
import tornado.web
import tornado.websocket

from tornado.options import define, options, parse_command_line

define("port", default=8888, help="run on the given port", type=int)

clients = dict()

class IndexHandler(tornado.web.RequestHandler):
    @tornado.web.asynchronous
    def get(self):
        #self.write("This is your response")
        #self.finish()
        self.render("index.html")

class WebSocketHandler(tornado.websocket.WebSocketHandler):
    count = 0

    def open(self, *args):
        self.id = self.get_argument("Id")
        self.stream.set_nodelay(True)
        clients[self.id] = {"id": self.id, "object": self}

    def on_message(self, message):
        self.count += 1
        print "Client %s sent a message: %s" % (self.id, message)
        self.write_message("You said: {msg} {count}".format(msg=message, count=self.count))

    def on_close(self):
        if self.id in clients:
            del clients[self.id]


app = tornado.web.Application([
    (r'/', IndexHandler),
    (r'/ws', WebSocketHandler),
])

if __name__ == '__main__':
    parse_command_line()
    app.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()

