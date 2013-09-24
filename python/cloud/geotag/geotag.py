from google.appengine.api import users
import webapp2
import pyodbc
import json
import collections

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

class WorkorderHandler(webapp2.RequestHandler):
  def get(self, wonr):
    cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER=ERPTEST\\ERP;DATABASE=GeoRiotinto;UID=sa;PWD=sa')
    cursor = cnxn.cursor();
    cursor.execute("""
        SELECT ID, TIMESTAMP, LABELNR, PASS, ERRORCODE, VERIFIED FROM TAGSREAD
        WHERE WONR = 
        """)

    rows = cursor.fetchall()

    rowarray_list = []
    for row in rows:
      t = (row.id, row.serial)
      rowarray_list.append(t)

    j = json.dumps(rowarray_list)
    self.response.write('This is the WorkorderHandler. '
        'The work order number is %s' % wonr)

routes = [
  (r'/', MainPage),
  (r'/geo', GeotagHandler),
  (r'/geo/(\d+)', WorkorderHandler),
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

