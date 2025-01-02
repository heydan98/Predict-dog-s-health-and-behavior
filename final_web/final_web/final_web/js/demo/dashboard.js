// Set new default font family and font color to mimic Bootstrap's default styling
Chart.defaults.global.defaultFontFamily = 'Nunito', '-apple-system,system-ui,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif';
Chart.defaults.global.defaultFontColor = '#858796';


async function fetchData() {
  const response = await fetch('https://wfyo32vatc.execute-api.ap-southeast-2.amazonaws.com/database/get_database?limit=10');
  const data = await response.json();
  return data;
}

async function fetchData_health() {
  const response = await fetch('https://285l3svbjf.execute-api.ap-southeast-2.amazonaws.com/user/get_database?limit=10');
  const data_health = await response.json();
  return data_health;
}

function getLatestValueByTimestamp(data, key) {
  let items = data;
  
  // Kiểm tra nếu data là một object có thuộc tính 'items'
  if (data && typeof data === 'object' && Array.isArray(data.items)) {
    items = data.items;
  }
  
  if (Array.isArray(items) && items.length > 0) {
    const sortedData = items.sort((a, b) => new Date(b.timestamp) - new Date(a.timestamp));
    const latestItem = sortedData[0];
    return latestItem[key];
  } else {
    console.log("Data is not in the expected format:", data);
    return null; // hoặc giá trị mặc định khác
  }
}

function getLatestValueByTimestamp_health(data, key) {
  if (Array.isArray(data)) {
    const sortedData = data.sort((a, b) => new Date(b.timestamp) - new Date(a.timestamp));
    const latestItem = sortedData[0];
    return latestItem[key];
  } else {
    // Xử lý trường hợp data không phải là mảng
    return null; // hoặc giá trị mặc định khác
  }
}

let map; 
function initMap(data) {
  const latestLat = getLatestValueByTimestamp(data, 'lat');
  const latestLong = getLatestValueByTimestamp(data, 'long');
  const latLng = [latestLat, latestLong];
  const currentLocation = `Latitude: ${latestLat}, Longitude: ${latestLong}`;

  // Kiểm tra nếu map đã được khởi tạo
  if (!map) {
    map = L.map('map').setView(latLng, 13);

    L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
      attribution: currentLocation,
      maxZoom: 18,
    }).addTo(map);
  } else {
    // Nếu map đã được khởi tạo, chỉ cần cập nhật vị trí mới
    // map.setView(latLng, 13);
  }

  L.marker(latLng).addTo(map);
}

function temperature(data) {
  const latestTemperature = getLatestValueByTimestamp(data, 'temp');
  const TemperatureContentElement = document.getElementById('temperature-content');
  if (TemperatureContentElement !== null) {
    TemperatureContentElement.textContent = latestTemperature;
  }

}

function updateBehavior(data) {
  const latestBehavior = getLatestValueByTimestamp_health(data, 'behavior');
  const behaviorContentElement = document.getElementById('behavior-content');
  behaviorContentElement.textContent = latestBehavior;
}

function updateHealthStatus(data) {
  const latestHealth = getLatestValueByTimestamp_health(data, 'health');
  const HealthContentElement = document.getElementById('health-content');
  HealthContentElement.textContent = latestHealth;
  HealthContentElement.classList.remove('good', 'normal', 'bad');
  // Thêm lớp màu mới tùy theo giá trị của latestHealth
  if (latestHealth.toLowerCase() === 'good') {
    HealthContentElement.classList.add('good');
  } else if (latestHealth.toLowerCase() === 'normal') {
    HealthContentElement.classList.add('normal');
  } else {
    HealthContentElement.classList.add('bad');
  }
}

function Batterry(data) {
  const latestbatterry = getLatestValueByTimestamp(data, 'battery');
  const batterryContentElement = document.getElementById('batterry-content');
  batterryContentElement.textContent = latestbatterry;
  
  const acidFillElement = document.querySelector('.acid .fill');
  const acidHeight = (latestbatterry / 100) * 250; // Chiều cao tối đa của cục pin là 250px
  acidFillElement.style.setProperty('--acid-height', `${acidHeight}px`);
  const chargingIcon = document.querySelector('.percentage i');
  chargingIcon.style.setProperty('--display-charging', latestbatterry < 100 ? 'inline' : 'none');

  const batteryElement = document.querySelector('.battery');

  if (latestbatterry < 20) {
    acidFillElement.classList.add('full-low-battery');
    batteryElement.classList.add('low-battery');
  } else {
    acidFillElement.classList.remove('full-low-battery');
    batteryElement.classList.remove('low-battery');
  }
}

let alerts = [];
let unreadAlertsCount = 0;
let readAlerts = [];

function generateTemperatureAlert(data) {
  const temperature = getLatestValueByTimestamp(data, 'temp');
  const timestamp = getLatestValueByTimestamp(data, 'timestamp');
  if (temperature > 39.5) {
    return {
      type: 'danger',
      icon: 'fas fa-temperature-high',
      message: `High temperature alert: ${temperature}°C`,
      timestamp: new Date(timestamp)
    };
  } else if (temperature < 37.5) {
    return {
      type: 'warning',
      icon: 'fas fa-temperature-low',
      message: `Low temperature alert: ${temperature}°C`,
      timestamp: new Date(timestamp)
    };
  }
  return null;
}

function generateHealthAlert(data_health) {
  const health = getLatestValueByTimestamp_health(data_health, 'health');
  const timestamp = getLatestValueByTimestamp_health(data_health, 'timestamp');
  if (health.toLowerCase() === 'bad') {
    return {
      type: 'danger',
      icon: 'fas fa-heartbeat',
      message: 'Health status: Bad. Immediate attention required.',
      timestamp: new Date(timestamp)
    };
  } else if (health.toLowerCase() === 'normal') {
    return {
      type: 'warning',
      icon: 'fas fa-heart',
      message: 'Health status: Normal. Monitor closely.',
      timestamp: new Date(timestamp)
    };
  }
  return null;
}
function generateBatteryAlert(data) {
  const battery = getLatestValueByTimestamp(data, 'battery');
  const timestamp = getLatestValueByTimestamp(data, 'timestamp');
  if (battery <= 20) {
    return {
      type: 'warning',
      icon: 'fas fa-battery-quarter',
      message: `Low battery alert: ${battery}%`,
      timestamp: new Date(timestamp)
    };
  } else if (battery === 100) {
    return {
      type: 'info',
      icon: 'fas fa-battery-full',
      message: 'Battery fully charged: 100%',
      timestamp: new Date(timestamp)
    };
  }
  return null;
}

function updateAlerts(data, data_health) {
  const tempAlert = generateTemperatureAlert(data);
  const healthAlert = generateHealthAlert(data_health);
  const batteryAlert = generateBatteryAlert(data);
  
  function alertExists(newAlert) {
    return alerts.some(existingAlert => 
      existingAlert.message === newAlert.message &&
      isSameMinute(existingAlert.timestamp, newAlert.timestamp)
    );
  }

  function isAlertRead(newAlert) {
    return readAlerts.some(readAlert => 
      readAlert.message === newAlert.message &&
      isSameMinute(readAlert.timestamp, newAlert.timestamp)
    );
  }

  function addAlert(alert) {
    if (!alertExists(alert)) {
      alerts.push(alert);
      if (!isAlertRead(alert)) {
        unreadAlertsCount++;
      }
    }
  }

  if (tempAlert) addAlert(tempAlert);
  if (healthAlert) addAlert(healthAlert);
  if (batteryAlert) addAlert(batteryAlert);
  
  // Loại bỏ các cảnh báo cũ (giữ lại cảnh báo mới nhất trong mỗi phút)
  const uniqueAlerts = [];
  const seenMessages = new Set();
  
  for (const alert of alerts.sort((a, b) => b.timestamp - a.timestamp)) {
    const messageKey = `${alert.message}-${alert.timestamp.getFullYear()}-${alert.timestamp.getMonth()}-${alert.timestamp.getDate()}-${alert.timestamp.getHours()}-${alert.timestamp.getMinutes()}`;
    if (!seenMessages.has(messageKey)) {
      uniqueAlerts.push(alert);
      seenMessages.add(messageKey);
    }
  }
  
  alerts = uniqueAlerts;
  
  updateAlertsDisplay();
}

document.addEventListener('deviceready', function() {
  if (window.cordova) {
    cordova.plugins.notification.local.requestPermission(function (granted) {
      console.log('Permission has been granted: ' + granted);
    });
  } else if (window.Capacitor) {
    const { LocalNotifications } = Capacitor.Plugins;
    LocalNotifications.requestPermissions();
  }
}, false);

function updateAlertsDisplay() {
  const alertsContainer = document.getElementById('alerts-container');
  const alertsCounter = document.getElementById('alerts-counter');
  
  unreadAlertsCount = alerts.filter(alert => !isAlertRead(alert)).length;
  alertsCounter.textContent = unreadAlertsCount > 0 ? unreadAlertsCount : '';
  
  alertsContainer.innerHTML = '';
  
  alerts.forEach(alert => {
    const alertElement = document.createElement('a');
    alertElement.className = 'dropdown-item d-flex align-items-center';
    alertElement.href = '#';
    
    const iconDiv = document.createElement('div');
    iconDiv.className = 'mr-3';
    const iconCircle = document.createElement('div');
    iconCircle.className = `icon-circle bg-${alert.type}`;
    const icon = document.createElement('i');
    icon.className = `${alert.icon} text-white`;
    iconCircle.appendChild(icon);
    iconDiv.appendChild(iconCircle);
    
    const contentDiv = document.createElement('div');
    const timeDiv = document.createElement('div');
    timeDiv.className = 'small text-gray-500';
    timeDiv.textContent = alert.timestamp.toLocaleString();
    const messageSpan = document.createElement('span');
    messageSpan.className = 'font-weight-bold';
    messageSpan.textContent = alert.message;
    contentDiv.appendChild(timeDiv);
    contentDiv.appendChild(messageSpan);
    
    alertElement.appendChild(iconDiv);
    alertElement.appendChild(contentDiv);
    
    alertsContainer.appendChild(alertElement);

    if (isAlertRead(alert)) {
      alertElement.style.opacity = '1';  // Làm mờ các thông báo đã đọc
    }
    
    alertsContainer.appendChild(alertElement);
  });
}

function isAlertRead(alert) {
  return readAlerts.some(readAlert => 
    readAlert.message === alert.message &&
    isSameMinute(readAlert.timestamp, alert.timestamp)
  );
}

function isSameMinute(date1, date2) {
  return date1.getFullYear() === date2.getFullYear() &&
         date1.getMonth() === date2.getMonth() &&
         date1.getDate() === date2.getDate() &&
         date1.getHours() === date2.getHours() &&
         date1.getMinutes() === date2.getMinutes();
}

function handleAlertClick() {
  readAlerts = alerts.map(alert => ({
    message: alert.message,
    timestamp: alert.timestamp
  }));
  updateAlertsDisplay();
}

document.addEventListener('DOMContentLoaded', () => {
  // Các đoạn mã khác giữ nguyên

  const alertsDropdown = document.getElementById('alertsDropdown');
  if (alertsDropdown) {
    alertsDropdown.addEventListener('click', handleAlertClick);
  }
});

let lastThirtyData = [];

function updateDeviceStatus(data) {
  const deviceStatusElement = document.getElementById('device-status-content');
  
  // Thêm dữ liệu mới vào mảng
  lastThirtyData.push(JSON.stringify(data));
  
  // Giữ chỉ 30 giá trị mới nhất
  if (lastThirtyData.length > 3) {
    lastThirtyData.shift();
  }
  
  // Kiểm tra xem 30 giá trị có giống nhau không
  const allSame = lastThirtyData.every(item => item === lastThirtyData[0]);
  
  if (lastThirtyData.length === 3 && allSame) {
    deviceStatusElement.textContent = 'OFF';
    deviceStatusElement.style.color = 'red';
  } else {
    deviceStatusElement.textContent = 'ON';
    deviceStatusElement.style.color = 'green';
  }
}

// Modify the existing updateData function
async function updateData() {
  const data = await fetchData();
  const data_health = await fetchData_health();
  temperature(data);
  Batterry(data);
  initMap(data);
  updateBehavior(data_health);
  updateHealthStatus(data_health);
  updateAlerts(data, data_health);
  updateDeviceStatus(data);
  // renderBehaviorChart(data);
}

setInterval(updateData, 1000);
updateData();
