/**
 * Module dependencies.
 */

var express = require('express')
  , fs = require('fs')
  , mysql = require('mysql')
  , hash = require('./pass').hash;

  /*var socket = require('./client.js');*/
var config = JSON.parse(fs.readFileSync('config.json'));
var listen_port = config.port;
var app = module.exports = express();
var log = console.log;
/*var sql_options = { host: "127.0.0.1", port: 3306, user: "berm", password: "berm", database: "berm" };*/
var sql_options = { host: "192.168.1.32", port: 3306, user: "berm", password: "berm", database: "berm" };
var sql = mysql.createConnection(sql_options);

// config
app.set('view engine', 'ejs');
app.set('views', __dirname + '/views');

// middleware
app.use(express.bodyParser());
app.use(express.cookieParser('shhhh, very secret'));
app.use(express.session());
app.use(express.logger('dev'));

// Session-persisted message middleware
app.use(function(req, res, next){
  var err = req.session.error
    , msg = req.session.success;
  delete req.session.error;
  delete req.session.success;
  res.locals.message = '';
  if (err) {
    res.locals.f_err = true;
    res.locals.message = err;
  } else if (msg) {
    res.locals.f_err = false;
    res.locals.message = msg;
  }
  next();
});

// database
var users = {};
getUsers();
function getUsers() {
  sql.query("SELECT name FROM user",
      function(err, rows, fields) {
      if (err) throw err;

      var usr = {};
      for (i=0; i<rows.length; i++) {
      usr[rows[i].name] = { "name":rows[i].name };
      log(usr[rows[i].name].name);
      }
      users = usr;
      });
}

// when you create a user, generate a salt
// and hash the password ('foobar' is the pass here)
/*createHash('aa', 'aa', updateUser);*/
function createHash(user, pass, callback, next) {
  hash(pass, function(err, salt, hash){
    if (err) throw err;
    // store the salt & hash in the "db"
    callback(null, user, salt, hash, next);
  });
}
function createUser(err, user, salt, hash, next) {
  if (err) throw err;
  var q = "INSERT INTO user ( name, salt, hash ) VALUES ( '" + user + "', '" + salt + "', '" + hash.toString('base64') + "');";
  log(q);
  sql.query(q, function(err, rows, fields) {
    if (err) throw err;
    if (3 == arguments.length) {
      next();
    }
  });
}
function updateUser(err, user, salt, hash, callback) {
  if (err) throw err;
  /*log('user: ' + user + '\nsalt: ' + salt + '\nhash: ' + hash.toString('base64'));*/
  var q = "UPDATE user SET salt='" + salt + "', hash='" + hash.toString('base64') + "' WHERE name='" + user + "';";
  log(q);
  sql.query(q, function(err, rows, fields) {
    if (err) throw err;
    if (5 == arguments.length) {
      callback();
    }
  });
}
function getUserHash(err, user, callback) {
  if (err) throw err;
  var q = "SELECT salt, hash FROM user WHERE name='" + user + "';";
  log(q);
  sql.query(q, function(err, rows, fields) {
    if (err) throw err;
    if (rows.length !== 1) {
      callback("ERR: database query!");
    }
    log(rows[0]);
    callback(null, user, rows[0].salt, new Buffer(rows[0].hash, 'base64'));
  });
}

// Authenticate using our plain-object database of doom!
function authenticate(name, pass, fn) {
  if (!module.parent) console.log('authenticating %s:%s', name, pass);
  var user = users[name];
  /*log(JSON.stringify(users));*/
  // query the db for the given username
  if (!user) return fn(new Error('cannot find user'));
  // apply the same algorithm to the POSTed password, applying
  // the hash against the pass / salt, if there is a match we
  // found the user
  getUserHash(null, name, function(err, user, dbSalt, dbHash) {
    if (err) return fn(new Error(err));
    hash(pass, dbSalt, function(err, hash){
      if (err) return fn(err);
      var i=0;
      for(i=0; i<hash.length; i++) {
        if (hash[i] != dbHash[i])
          break;
      }
      if (i == hash.length) return fn(null, user);
      log('invalid error');
      fn(new Error('invalid password'));
    });
  });
}

function restrict(req, res, next) {
  if (req.session.user) {
    next();
  } else {
    if (req.path !== '/')
      req.session.error = 'Access denied!';
    res.redirect('/login');
  }
}

app.get('/', function(req, res){
    /*res.redirect('login');*/
  res.render('login');
});

app.get('/restricted', restrict, function(req, res){
    /*res.send('Wahoo! restricted area, click to <a href="/logout">logout</a>');*/
  res.redirect('home');
});

app.get('/logout', function(req, res){
  // destroy the user's session to log them out
  // will be re-created next request
  req.session.destroy(function(){
    res.redirect('/');
  });
});

app.get('/login', function(req, res){
  res.render('login');
});

app.post('/login', function(req, res){
  authenticate(req.body.username, req.body.password, function(err, user){
    if (user) {
      // Regenerate session when signing in
      // to prevent fixation 
      req.session.regenerate(function(){
        // Store the user's primary key 
        // in the session store to be retrieved,
        // or in this case the entire user object
        req.session.user = user;
        req.session.success = 'Authenticated as ' + user
          + ' You may now access restricted area.';
        res.redirect('back');
      });
    } else {
      req.session.error = 'Authentication failed, please check your '
        + ' username and password.'
        + ' (use "tj" and "foobar")';
      res.redirect('login');
    }
  });
});

var path = require('path');
app.get('/jquery/:file', function(req,res,next){
  res.sendfile(path.join(__dirname, 'public', 'jquery', req.params.file));
});
app.get('/jquery/images/:file', function(req,res,next){
  res.sendfile(path.join(__dirname, 'public', 'jquery', 'images', req.params.file));
});
app.get('/favicon.ico', function(req,res,next) {
  res.sendfile(path.join(__dirname, 'public', req.path));
});

app.get('/home', restrict, function(req,res,next) {
  res.locals.message = 'Hello ' + req.session.user + ', this is your home.';
  if (req.query.cmd !== undefined) {
    log('ask: ', req.query.cmd);
    /*socket.askGateway(req.query.cmd, function(msg) {*/
    /*res.locals.message = msg;*/
    /*res.render('home');*/
    /*});*/
    return;
  }
  res.render('home');
});

app.get('/signup', function(req,res,next) {
  res.locals.message = 'Please fill the blanks';
  res.render('create_user');
});
app.post('/signup', function(req,res,next) {
  res.locals.message = 'Please fill the blanks';
  res.render('create_user');
});

app.post('/create_user', function(req,res,next) {
  createHash(req.body.username, req.body.password, createUser, next);
}, succesToHome);

function succesToHome(req, res) {
  res.locals.message = 'User creating is Successful!';
  res.render('home');
  getUsers();
}

app.post('/setip', function(req,res,next) {
  res.render('create_user');
});

if (!module.parent) {
  app.listen(listen_port);
  console.log('Express started on port ' + listen_port);
}

