const fetch = require('node-fetch');
const axios = require('axios');
const cron = require('node-cron');
const express = require('express');
const bodyParser = require('body-parser');
const router = express.Router();

router.use(bodyParser.urlencoded({ extended: true }));


// Function to make the API call with specified pin and value
const makeAPICall = async (pin, value) => {
    try {
        const response = await axios.get('https://blr1.blynk.cloud/external/api/update', {
            params: {
                token: process.env.TOKEN,
                [pin]: value
            }
        });
        console.log(`API Call Successful for pin ${pin}:`, response.data);
    } catch (error) {
        console.error(`Error making API call for pin ${pin}:`, error);
    }
};

// Define a loop function to make API calls for both pins
let loopInterval;
let isLoopRunning = false;

const startLoop = () => {
    if (!isLoopRunning) {
        isLoopRunning = true;
        loopInterval = setInterval(async () => {
            console.log('Making API calls...');
            await makeAPICall('v1', '0'); // Example value for v1 pin
            await makeAPICall('v2', '0'); // Example value for v2 pin
        }, 5000); // 5000 milliseconds = 5 seconds
    }
};

// Function to stop the loop
const stopLoop = () => {
    if (isLoopRunning) {
        clearInterval(loopInterval);
        isLoopRunning = false;
    }
};
// Routes (assuming this is inside your Express router)
router.get('/', (req, res) => {
  res.render('loging', );
  // Render your EJS template here
});

router.get('/start-loop', (req, res) => {
    startLoop();
    res.sendStatus(200);
});

router.get('/stop-loop', (req, res) => {
    stopLoop();
    res.sendStatus(200);
});

//disco effect
// Function to make the API call with specified pin and value
const makeAPICall2 = async (pin, value) => {
  try {
      const response = await fetch(`https://blr1.blynk.cloud/external/api/update?token=3FIal-QhcyJjX-doLiqLDyHbRCtAJVvS&${pin}=${value}`);
      if (response.ok) {
          console.log(`API Call Successful for pin ${pin} with value ${value}`);
      } else {
          console.error('API Call failed');
      }
  } catch (error) {
      console.error('Error making API call:', error);
  }
};

let discoInterval;
let v3Value = 0;
let v4Value = 1;

// Function to start the disco effect
const startDisco = () => {
  discoInterval = setInterval(() => {
      // Toggle the values of v3 and v4
      v3Value = 1 - v3Value;
      v4Value = 1 - v4Value;
      // Make API calls to update the values
      makeAPICall2('v3', v3Value);
      makeAPICall2('v4', v4Value);
  }, 800); // Change the interval as needed for the disco effect speed
};

const stopDisco = () => {
  clearInterval(discoInterval);
};

router.get('/startDisco', (req, res) => {
  startDisco();
  res.redirect('/');
});

router.get('/stopDisco', (req, res) => {
  stopDisco();
  res.redirect('/');
});


router.post('/login', (req, res) => {
  const { username, password } = req.body;
  // Basic authentication logic (replace with your actual authentication logic)
  if (username === 'krishna' && password === process.env.PASSWORD) {
    res.render('index')
  } else {
      res.render('loging')
  }
});



module.exports = router;
