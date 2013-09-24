from google.appengine.api import users
import webapp2

class MainPage(webapp2.RequestHandler):
  def get(self):
    user = users.get_current_user()
    if user:
      self.response.headers['Content-Type'] = 'text/plain'
      self.response.write('Hello, ' + user.nickname() + '!')
    else:
      self.redirect(users.create_login_url(self.request.uri))

class GeotagHandler(webapp2.RequestHandler):
  def get(self):
    foo = self.app.config.get('foo')
    self.response.write('foo value is %s' % foo)

class ProductHandler(webapp2.RequestHandler):
  def get(self, product_id):
    self.response.write('This is the ProductHandler. '
        'The product id is %s' % product_id)

routes = [
  (r'/', MainPage),
  (r'/geo', GeotagHandler),
  (r'/geo/(\d+)', ProductHandler),
]

config = {}
config['webapp2_extras.sessions'] = {
  'secret_key': 'something-very-very-secret-code',
}
config['foo'] = 'bar8'


application = webapp2.WSGIApplication(routes=routes, debug=True, config=config)

def main():
  application.run()

if __name__ == '__main__':
  main()

