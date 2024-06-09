document.addEventListener('DOMContentLoaded', () => {
    let v1Value = 0;
    let v2Value = 0;
    let v3Value = 0;
    let v4Value = 0;

    document.getElementById('button1').addEventListener('click', () => {
        v1Value = 1 - v1Value;
        makeAPICall('v1', v1Value);
    });

    document.getElementById('button2').addEventListener('click', () => {
        v2Value = 1 - v2Value;
        makeAPICall('v2', v2Value);
    });

    document.getElementById('button3').addEventListener('click', () => {
        v3Value = 1 - v3Value;
        makeAPICall('v3', v3Value);
    });

    document.getElementById('button4').addEventListener('click', () => {
        v4Value = 1 - v4Value;
        makeAPICall('v4', v4Value);
    });
});

const makeAPICall = async (pin, value) => {
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


document.addEventListener('DOMContentLoaded', () => {
    const button1 = document.getElementById('button1');
    const button2 = document.getElementById('button2');

    button12.addEventListener('click', () => {
        // Send AJAX request to start the loop
        fetch('/start-loop')
            .then(response => {
                if (response.ok) {
                    makeAPICall('v1', '0');
                    makeAPICall('v2', '0');
                    console.log('Loop started');
                } else {
                    console.error('Failed to start loop');
                }
            })
            .catch(error => {
                console.error('Error:', error);
            });
    });

    button11.addEventListener('click', () => {
        // Send AJAX request to stop the loop
        fetch('/stop-loop')
            .then(response => {
                if (response.ok) {
                   
                    console.log('Loop stopped');
                } else {
                    console.error('Failed to stop loop');
                }
            })
            .catch(error => {
                console.error('Error:', error);
            });
    });
});


//disco effect

document.addEventListener('DOMContentLoaded', () => {
    const button22 = document.getElementById('button22');

    button22.addEventListener('click', () => {
        const url = button22.dataset.running === 'true' ? '/stopDisco' : '/startDisco';
        fetch(url)
            .then(response => {
                if (response.ok) {
                    if (url === '/startDisco') {
                        console.log('Disco started');
                        button22.dataset.running = 'true';
                        button22.textContent = 'Stop Disco';
                    } else {
                        console.log('Disco stopped');
                        button22.dataset.running = 'false';
                        button22.textContent = 'Start Disco';
                    }
                } else {
                    console.error('Failed to toggle disco');
                }
            })
            .catch(error => {
                console.error('Error:', error);
            });
    });
});
