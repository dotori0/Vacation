var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');


var indexRouter = require('./routes/index');
var usersRouter = require('./routes/users');

var app = express();
var http = require('http').Server(app);

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

var mongoose = require('mongoose');

mongoose.connect('mongodb+srv://[DB 주소]', {useNewUrlParser : true});

var db = mongoose.connection;
db.on('error', console.error.bind(console, 'connection error: '));
db.once('open', function() {
  console.log('DB connected');
});

var Schema = mongoose.Schema;

var memo = new Schema({
  author : String,
  contents : String,
  date : Date
});

var memoModel = mongoose.model('Memo', memo);

/* GET home page. */
app.get('/', function(req, res, next) {
  res.render('index', { title: 'Express' });
});

var io = require('socket.io')(http);

io.on('connect', function(socket){
  memoModel.find({}, function(err, data) {
    io.to(socket.id).emit('new data', data);
  });

  socket.on('create', function(author, contents) {
    var Memo = new memoModel();
    Memo.author = author;
    Memo.contents = contents;
    Memo.date = Date.now();

    Memo.save(function(err) {
      if(err) 
        throw err;
      else {
        memoModel.find({}, function(err, data) {
          io.emit('new data', data);
        });
      }
    });
  });
});

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

http.listen(3000, function() {
  console.log('server open');
})

module.exports = app;
