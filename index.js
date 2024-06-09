const express = require('express');
const app = express();
const path = require('path');
const port = 3000;
const router = require('./src/routes/index');

// Set views and view engine for rendering templates
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');
// Serve static files from the 'public' directory
app.use(express.static(path.join(__dirname, 'public')));
app.use('/', router);
require('./src/blynkapi');


//blynk server api path ./src/blynkapi.js
require('./src/blynkapi');

// Start the server
app.listen(port, () => {
    console.log(`Server listening at http://localhost:${port}`);
});
