const char htmlContent[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <title>Data validation interface</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <style>
      body {
        font-family: Roboto, sans-serif;
      }
      .container {
        width: 95%;
        max-width: 450px;
        margin: 0 auto;
        padding: 30px;
        border: 2px solid #2e1212;
        border-radius: 30px;
        background-color: #3093e4;
        overflow: auto;
        display: flex;
        flex-direction: column;
        align-items: stretch;
      }
      .header {
        text-align: center;
      }
      .frame {
        flex: 1;
        margin-bottom: 20px;
        display: flex;
        flex-direction: column;
      }
      .table {
        width: 100%;
        border-collapse: collapse;
      }
      .table th,
      .table td {
        padding: 10px;
        text-align: left;
        border: 1px solid #1279c8;
        background-color: #b6e4f4;
      }
      .table th {
        text-align: center;
      }

      .table .header-row th {
        background-color: skyblue;
      }
      .button {
        text-align: center;
      }
      .button button {
        padding: 10px 20px;
        font-size: 16px;
        background-color: #33bee9;
        color: #ff0000;
        border: none;
        border-radius: 7px;
        cursor: pointer;
      }
      .button button:hover {
        background-color: #0e8ac0;
      }

      .right-column {
        text-align: center;
        flex: 1;
      }
      .left-column {
        text-align: center;
      }
      .right-column input[type="text"] {
        width: 100%;
        box-sizing: border-box;
      }
      .right-column select {
        width: 100%;
        padding: 8px;
        border: 10px solid #f40d0d;
        border-radius: 5px;
        box-sizing: border-box;
      }
      .column-container {
        display: flex;
        flex-wrap: wrap;
      }
      .column-container .column {
        flex: 1;
      }
      .hidden {
        display: none;
      }
      .data-send-container {
        display: flex;
        flex-direction: column;
        align-items: center;
        text-align: center;
      }
      .data-send-container input[type="text"] {
        max-width: 100px;
        box-sizing: border-box;
      }
      .hidden-row {
        display: none;
      }
      .button-container {
        display: flex;
        align-items: center;
        justify-content: space-between;
      }
      .button-container button {
        font-size: 18px;
        background-color: #4caf50;
        color: #020010;
        border: none;
        border-radius: 5px;
        cursor: pointer;
        padding: 5px 10px;
      }
      .button-container button:hover {
        background-color: #45a049;
      }
      .centered {
        margin: 0 auto;
      }
      #rs485Options {
        margin: 0 auto;
        display: block;
      }
      .analog-inputs {
        display: none;
      }

      .show-analog-inputs .analog-inputs {
        display: table-cell;
      }
      .input-half-width {
        width: calc(50% - 1px); 
        margin: 0;
      }
      .digital-inputs,
      .customize-inputs {
        display: none;
      }
      .show-digital-inputs .digital-inputs,
      .show-customize-inputs .customize-inputs {
        display: table-cell;
      }
      .input-small {
        width: 40px; 
      }
    </style>
    <script>
      
      function formatValue(inputField) {
        const inputValue = inputField.value;

        // Insert a space after every two characters, except at the beginning
        inputField.value = inputValue.replace(/(\d{2})(?=\d)/g, '$1 ');
      }
      function toggleMode() {
        var modeSelect = document.getElementById("modeSelect");
        var sim4GSettings = document.querySelectorAll(".hidden#sim4GSettings");
        var wifiSettings = document.querySelectorAll(".hidden#wifiSettings");

        if (modeSelect.value === "SIM4G") {
          sim4GSettings.forEach((element) => {
            element.style.display = "table-row";
          });
          wifiSettings.forEach((element) => {
            element.style.display = "none";
          });
          var mosquittoSettings = document.querySelectorAll(".hidden#mosquittoSettings");
          var awsSettings = document.querySelectorAll(".hidden#awsSettings");
          mosquittoSettings.forEach((element) => {
            element.style.display = "none";
          });
          awsSettings.forEach((element) => {
            element.style.display = "none";
          });
        } else if (modeSelect.value === "WIFI"){
          sim4GSettings.forEach((element) => {
            element.style.display = "none";
          });
          wifiSettings.forEach((element) => {
            element.style.display = "table-row";
          });
        }
        else {
          sim4GSettings.forEach((element) => {
            element.style.display = "none";
          });
          wifiSettings.forEach((element) => {
            element.style.display = "none";
          });
        }
      }

      function toggleModeWifi() {
        var modeWifiSelect = document.getElementById("modeWifiSelect");
        
        var mosquittoSettings = document.querySelectorAll(".hidden#mosquittoSettings");
        var awsSettings = document.querySelectorAll(".hidden#awsSettings");

        if (modeWifiSelect.value === "Mosquitto") {
          mosquittoSettings.forEach((element) => {
            element.style.display = "table-row";
          });
          awsSettings.forEach((element) => {
            element.style.display = "none";
          });
        } else if (modeWifiSelect.value === "AWS") {
          mosquittoSettings.forEach((element) => {
            element.style.display = "none";
          });
          awsSettings.forEach((element) => {
            element.style.display = "table-row";
          });
        } else {
          mosquittoSettings.forEach((element) => {
            element.style.display = "none";
          });
          awsSettings.forEach((element) => {
            element.style.display = "none";
          });
        }
      }

      function toggleClassWifi() {
        var classSelectMosquitto = document.getElementById("classSelectMosquitto");
        var classSelectAWS = document.getElementById("classSelectAWS");
      }
      function checkmodeSelect() {
        var modeSelect = document.getElementById("modeSelect").value;
        if (modeSelect === "SIM4G") {
          document
            .getElementById("modeSelect")
            .setAttribute("data-type", "SIM4G");
        } else if (modeSelect === "WIFI") {
          document
            .getElementById("modeSelect")
            .setAttribute("data-type", "WIFI");
        }
      }

      function checkmodeWifiSelect() {
        var modeWifiSelect = document.getElementById("modeWifiSelect").value;

        if (modeWifiSelect === "Mosquitto") {
          document
            .getElementById("modeWifiSelect")
            .setAttribute("data-type", "Mosquitto");
        } else {
          document
            .getElementById("modeWifiSelect")
            .setAttribute("data-type", "AWS");
        }
      }

      function addDataRow(button) {
        var hiddenRow = document.querySelector(".hidden-row");
        if (hiddenRow) {
          hiddenRow.classList.remove("hidden-row");
          dataRowCount++;
        }

        var addButton = document.querySelector(".add-button");
        addButton.disabled = dataRowCount >= 5;

        var removeButton = document.querySelector(".remove-button");
        removeButton.disabled = dataRowCount <= 0;
      }

      function removeDataRow(button) {
        var row = button.parentNode.parentNode;
        row.style.display = "none";
        dataRowCount--;

        var addButton = document.querySelector(".add-button");
        addButton.disabled = dataRowCount >= 5;

        var removeButton = document.querySelector(".remove-button");
        removeButton.disabled = dataRowCount <= 0;
      }

      document.addEventListener("DOMContentLoaded", function () {
        var addButton = document.querySelector(".add-button");
        var removeButton = document.querySelector(".remove-button");

        // Initialize dataRowCount based on the initial number of visible data rows
        dataRowCount = document.querySelectorAll(
          ".data-row:not(.hidden-row)"
        ).length;

        addButton.disabled = dataRowCount >= 5;
        removeButton.disabled = dataRowCount <= 0;

        var addDataButtons = document.querySelectorAll(".add-data-button");
        addDataButtons.forEach(function (button) {
          button.addEventListener("click", function () {
            addDataRow(button);
          });
        });
      });

      function sendValues() {
        var dataType;
        var value1 = document.getElementById("value1").value;
        var value2 = document.getElementById("value2").value;
        var value3 = document.getElementById("value3").value;
        var value4 = document.getElementById("value4").value;
        var value5 = document.getElementById("value5").value;
        var value6 = document.getElementById("value6").value;
        var value7 = document.getElementById("value7").value;
        var modeSelect = document.getElementById("modeSelect").value;
        var modeWifiSelect = document.getElementById("modeWifiSelect").value;
        // var classSelectMosquitto = document.getElementById("classSelectMosquitto").value;
        // var classSelectAWS = document.getElementById("classSelectAWS").value;
        // var rs485Options = document.getElementById("rs485Options").value;
        
        var xhr = new XMLHttpRequest();
        xhr.open("POST", "/process", true);
        xhr.setRequestHeader(
          "Content-Type",
          "application/x-www-form-urlencoded"
        );
        xhr.onreadystatechange = function () {
          if (xhr.readyState === 4 && xhr.status === 200) {
            var successMessage = document.createElement("div");
            successMessage.className = "success-message";
            successMessage.style.cssText = "width: 300px; height: auto; padding: 20px; border: 1px solid #ccc; border-radius: 10px; background-color: #3498db; box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.3); position: fixed; top: 50%; left: 50%; transform: translate(-50%, -50%); display: flex; flex-direction: column; align-items: center; justify-content: center;";
            
            var successText = document.createElement("div");
            successText.innerHTML = "Send success";
            successText.style.cssText = "color: #fff; margin-bottom: 20px; text-align: center; font-weight: bold; font-size: 24px;";
            
            var okButton = document.createElement("button");
            okButton.innerHTML = "OK";
            okButton.style.cssText = "background-color: #2ecc71; border: none; border-radius: 5px; padding: 10px 20px; color: #fff; cursor: pointer; font-size: 18px;";
            okButton.onclick = function () {
              countdown();
              successMessage.style.display = "none";
            };
            
            successMessage.appendChild(successText);
            successMessage.appendChild(okButton);
            document.body.appendChild(successMessage);}
        };
        xhr.send(
          "value1=" + encodeURIComponent(value1) +
            "&value2=" + encodeURIComponent(value2) +
            "&value3=" + encodeURIComponent(value3) +
            "&value4=" + encodeURIComponent(value4) +
            "&value5=" + encodeURIComponent(value5) +
            "&value6=" + encodeURIComponent(value6) +
            "&value7=" + encodeURIComponent(value7) +
            
            "&modeSelect=" + encodeURIComponent(modeSelect)+
            "&modeWifiSelect=" + encodeURIComponent(modeWifiSelect)
        );
      }

      function countdown() {
        var count = 5;
        var countdownMessage = document.createElement("div");
        countdownMessage.style.cssText = "position:fixed;top:50%;left:50%;transform:translate(-50%,-50%);fontSize:18px;";
        countdownMessage.innerHTML = "Page will close in " + count + "s...";
      
        document.querySelector(".container").appendChild(countdownMessage);
      
        var countdownInterval = setInterval(function () {
          if (--count <= 0) {
            clearInterval(countdownInterval);
            window.location.href = "/thankyou.html";
          } else {
            countdownMessage.innerHTML = "Page will close in " + count + "s...";
          }
        }, 1000);
      }
    </script>
  </head>
  <body>
    <div class="container">
      <div class="frame">
        <!-- <div class="header" style="position: fixed; top: 1; width: 30%; z-index: 5;"> -->
        <div class="header">
          <svg version="1.0"
               x="0px" 
               y="0px" 
               width="65"
               height="63.788"
               viewBox="0 0 100 95.152" 
               enable-background="new 0 0 100 95.152" 
               class="woof">
                <path 
                      fill-rule="evenodd" 
                      clip-rule="evenodd" 
                      d="M75.754,91.754c-11.632,1.836-9.73-4.492-9.864-11.711l-0.686-14.15l-2.987,13.671
                        c-0.545,2.594-1.23,4.972,2.446,6.876c2.59,2.248-0.332,5.448-3.121,5.037c-8.518,0.277-7.284-5.376-6.548-9.385l0.479-5.79
                        c0.343-3.944,1.093-6.4,1.295-11.913l-1.15-2.988c-9.188-2.381-15.524-4.152-22.664-7.962l-5.114,0.679
                        c-2.311,0.342-1.692,3.066-1.966,4.834l-0.747,4.827l-4.228,10.961l-0.331,0.952c-0.279,0.754-0.211,3.745,0.886,5.304l2.237,4.561
                        c1.77,2.59,2.244,8.987-2.515,6.938l-3.541-1.49c-2.513-1.021-4.562-8.028-5.436-13.133l0.069-7.151
                        c-2.183-0.675-3.07,12.191-2.183,14.836l2.183,6.675c0.671,2.168-1.84,2.922-3.61,2.922c-4.154,0-5.992-4.218-6.876-7.76
                        l-0.675-2.646c-1.03-4.084-0.488-10.285,1.625-13.894l0.951-2.233c1.903-4.431-0.41-7.631-0.951-9.878
                        c-1.77-7.216-3.2-14.225,1.771-21.167l0.346-0.475c-0.144-0.542-1.576-1.695-2.46-4.417c-1.907-6.133-3.608-16.197-1.22-16.197
                        l1.562-0.077c4.021,0,2.854,16.607,7.627,15.522l34.574,0.343l6.056-1.64c2.25-0.606,4.152-2.035,6.122-3.264l3.94-2.447
                        c4.561,4.562,8.514,7.352,15.045,9.732c1.774,13.537-0.199,16.54-1.902,24.628l-0.688,3.269c-1.216,5.787-1.627,11.779-1.35,17.7
                        l0.134,2.789c-0.065,2.857,1.559,3.06,3.807,3.741C77.526,87.536,78.543,90.394,75.754,91.754L75.754,91.754L75.754,91.754z">
                </path>
                <path 
                      fill-rule="evenodd" 
                      clip-rule="evenodd" 
                      d="M83.062,9.073c1.901,1.297,4.286,3.608,6.191,5.646l2.388,2.592
                        c1.562,1.625,3.94,1.425,6.055,1.834l0.743,0.135c2.312,1.218,1.576,3.673,0.819,5.512l-0.477,1.153
                        c-2.183,5.247-8.645,5.657-13.208,3.198l-2.035-1.083H82.85l-1.422,0.952l-2.927,2.314c-5.92-2.591-10.751-5.714-14.637-9.866
                        l8.922-11.167l2.991-4.352l0.61-1.43l0.686-1.572c0.544-1.759,2.788-3.464,4.43-2.789c1.559,0.686,2.58,3.674,2.169,5.249
                        l-0.408,1.77L83.062,9.073L83.062,9.073L83.062,9.073z">
                </path>
          </svg>
          <h2 style="text-align: center">Dog monitor</h2>
        </div>
        <!-- create to input dog weight -->
        
                
        <div class="column-container">
          <!-- Cột WIFI -->
          <div class="column" id="protocolTable">
            <table class="table">
              <tr>
                <th>Dog's Weight (kg)</th>
                <td><input id="value7" type="text" /></td>
              </tr>
                <tr class="header-row">
                <th colspan="3">PROTOCOL</th>
              </tr>
              
              <tr>
                <th>Mode</th>
                <td>
                  <select id="modeSelect" onchange="toggleMode()">
                    <option value="">-- Select Mode --</option>
                    <option value="SIM4G">NB-IoT</option>
                    <option value="WIFI">WiFi</option>
                  </select>
                </td>
                <!-- </tr> -->
              </tr>
              <tr class="hidden" id="wifiSettings">
                <th>Mode WiFi</th>
                <td>
                  <select id="modeWifiSelect" onchange="toggleModeWifi()">
                    <option value="">-- Select Mode --</option>
                    <option value="AWS">AWS</option>
                    <option value="Mosquitto">Mosquitto</option>
                  </select>
                </td>
              </tr>
              
            <!-- Ẩn bảng thông số chế độ Mosquitto ban đầu -->
            <tr class="hidden" id="mosquittoSettings"> 
            </tr>
            <tr class="hidden" id="mosquittoSettings">
                <th>SSID</th>
                <td><input id="value1" type="text" /></td>
            </tr>
            <tr class="hidden" id="mosquittoSettings">
                <th>PASSWORD</th>
                <td><input id="value2" type="text" /></td>
            </tr>
            <tr class="hidden" id="mosquittoSettings">
                <th>BROKER ADDRESS</th>
                <td><input id="value3" type="text" /></td>
            </tr>
            <tr class="hidden" id="mosquittoSettings">
                <th>TOPIC</th>
                <td><input id="value4" type="text" /></td>
            </tr>
            <tr class="hidden" id="awsSettings">
                <th>SSID</th>
                <td><input id="value5" type="text" /></td>
            </tr>
            <tr class="hidden" id="awsSettings">
                <th>PASSWORD</th>
                <td><input id="value6" type="text" /></td>
            </tr>
            
            </table>
          </div>
        </div>
      </div>
      <div class="button">
        <!-- <button type="button" onclick="toggleDataRows()">+</button> -->
        <button type="button" onclick="sendValues()">Send</button>
      </div>
      <div class="button">
        <form id="uploadForm" method="POST" enctype="multipart/form-data" style="display: inline">
          <input type="file" name="update" />
          <button type="button" onclick="updateFirmware()">updateOTA</button>
        </form>
      </div>
      <div id="firmwareStatus"></div>
      </div>
    </div>
  </body>
</html>
)=====";