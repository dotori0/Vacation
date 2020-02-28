var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');

var indexRouter = require('./routes/index');
var usersRouter = require('./routes/users');

var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);
app.use('/users', usersRouter);
app.use('/chat', require('./routes/chat.js'));

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

var users = [];

io.on('connection', function(socket) {
  console.log('user connected : ' + socket.id);

  socket.on('create name', function(name) {
    socket.name = name;
    users.push(name);
    console.log(users);
    console.log('\'' + name + '\'님이 접속 하셨습니다.');
    io.emit('receive message', '<span style="color: blue;">\'' + name + '\'님이 접속 하셨습니다.</span>');
  })

  socket.on('send message', function(name, text) {
    var msg = name + ' : ' + text;
    socket.name = name;
    console.log(msg);
    io.emit('receive message', msg);
  })

  socket.on('show users', function() {
    user = '';
    for (var i in users) {
      user += users[i] + ' ';
    }
    io.to(socket.id).emit('receive message', user);
  })

  socket.on('disconnect', function() {
    users.splice(users.findIndex(function(item) { return item == socket.name }), 1);
    console.log('\'' + socket.name + '\'님이 나가셨습니다.');
    io.emit('receive message', '<span style="color: red;">\'' + socket.name + '\'님이 나가셨습니다.</span>');
  });
});

http.listen(80, function() {
  console.log('Server Open');
});