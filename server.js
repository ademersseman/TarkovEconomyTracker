const express = require('express');
const app = express();
const bodyParser = require('body-parser');

var priceindex = -1;

app.use(bodyParser.urlencoded({ extended: true }));

app.set('view engine', 'ejs');

app.post('/getinput', (req, res) => {
  const fs = require('fs')
  const input = "" + `${req.body.input}`;

  fs.writeFile('userinput.txt', input.replace(/\s/g, "").replace("-","").replace("(","").replace("\"","").replace("\'","").toLowerCase(), err => {
  if (err) {
    console.error(err)
    return
  }
  //file written successfully
  })
  //res.send(`${req.body.input}`);
});

app.get('/getprice', (req, res) => {//NEED TO CREATE NEW HTML WEBSITE FOR THIS TO WORK
  var fs = require('fs');
  const contents = fs.readFileSync('programoutput.txt').toString() + fs.readFileSync('currenthour.txt').toString();
  console.log(contents);
  res.send(contents);
});

app.get('/', (req, res) => {
  res.render('index');
});

app.get('/refresh', (req, res) => {
  location.reload();
});

app.listen(8000, () => {
  console.log(`Server running on port 8000`);
});
