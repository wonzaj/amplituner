const char index_html[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Amplituner</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
        button { padding: 15px 30px; font-size: 20px; border: none; border-radius: 5px; cursor: pointer; }
        .on { background-color: green; color: white; }
        .off { background-color: red; color: white; }
        #volumeControl { margin: 20px auto; width: 200px; height: 200px; }
        #sliderValue { position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); font-size: 24px; font-weight: bold; }
    </style>
    <link rel="stylesheet" href="/roundslider.css">
    <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
    <script src="/roundslider.js"></script>
</head>
<body>
    <h1>Amplituner</h1>
    <button id="powerButton" class="off" onclick="togglePower()">OFF</button>
    <h2>Volume</h2>
    <div id="volumeControl"></div>

    <script>    
        function fetchState() { 
            fetch('/state')
                .then(response => response.json())
                .then(data => {
                const powerButton = document.getElementById('powerButton');
                if (data.power === 'ON') {
                    powerButton.classList.remove('off');
                    powerButton.classList.add('on');
                    powerButton.innerText = 'ON';
                } else {
                    powerButton.classList.remove('on');
                    powerButton.classList.add('off');
                    powerButton.innerText = 'OFF';
                }

                // Aktualizacja wartości slidera
                $("#volumeControl").roundSlider("setValue", data.volume);
                $("#sliderValue").text(data.volume);
        })
        .catch(err => console.error('Error fetching state:', err));
        }

        function togglePower() {
            const button = document.getElementById('powerButton');
            const currentState = button.classList.contains('on') ? 'OFF' : 'ON';
            fetch('/power', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ power: currentState })
            }).then(() => fetchState());
        }

        function updateVolume(event) {
            const newVolume = event.value; 
            document.getElementById('volumeValue').innerText = newVolume;
            fetch('/volume', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ volume: newVolume })
            });
        }

        $(document).ready(function() 
        {
        $("#volumeControl").roundSlider({
            radius: 100,
            width: 15,
            handleSize: "+8",
            handleShape: "dot",
            sliderType: "min-range",
            value: 50,
            min: 0,
            max: 100,
            change: function(event) {
                $("#sliderValue").text(event.value);
            fetch('/volume', 
            {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ volume: event.value })
            });
        }
    });

    fetchState();
});
    </script>
</body>
</html>
)rawliteral";
