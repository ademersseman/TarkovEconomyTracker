const express = require('express');
const app = express();
const bodyParser = require('body-parser');

app.use(bodyParser.urlencoded({ extended: true }));

app.set('view engine', 'ejs');
//post request recieves the user input from the HTML form and writes to file userinput.txt
app.post('/getinput', (req, res) => {
  const fs = require('fs')
  const input = "" + `${req.body.input}`;

  fs.writeFile('userinput.txt', input.replace(/\s/g, "").replace("-","").replace("(","").replace("\"","").replace("\'","").toLowerCase(), err => {//writes to the file removing whitespace and undesirable characters
  if (err) {
    console.error(err)
    return
  }
  //file written successfully
  })
});
//get request sends the contents of programoutput.txt with the contents of currenthour.txt appended to it
app.get('/getprice', (req, res) => {
  var fs = require('fs');
  const contents = fs.readFileSync('programoutput.txt').toString() + fs.readFileSync('currenthour.txt').toString();
  console.log(contents);
  res.send(contents);
});
//default page
app.get('/', (req, res) => {
  res.render('index');
});
//renders library
app.get('/library', (req, res) => {
  res.render('library');
});
//listens to my local network
app.listen(8000, () => {
  console.log(`Server running on port 8000`);
});
