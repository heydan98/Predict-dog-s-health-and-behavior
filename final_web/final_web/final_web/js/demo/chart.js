// Set new default font family and font color to mimic Bootstrap's default styling
Chart.defaults.global.defaultFontFamily = 'Nunito', '-apple-system,system-ui,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif';
Chart.defaults.global.defaultFontColor = '#858796';

let dataByDate = {};
const MAX_DATA_POINTS = 20;
let temperatureChart;
let behaviorChart;

// Fetch data
async function fetchData(date) {
  const today = new Date().toISOString().split('T')[0];
  let url = 'https://wfyo32vatc.execute-api.ap-southeast-2.amazonaws.com/database/get_database';
  
  if (date === today) {
    url += '?limit=5';
  }
  
  const response = await fetch(url);
  const data = await response.json();
  
  // Phân loại dữ liệu theo ngày
  data.forEach(item => {
    const itemDate = new Date(item.timestamp).toISOString().split('T')[0];
    if (!dataByDate[itemDate]) {
      dataByDate[itemDate] = { temperature: [], behavior: [] };
    }
    dataByDate[itemDate].temperature.push(item);
  });
  
  return data;
}

async function fetchData_health(date) {
  const today = new Date().toISOString().split('T')[0];
  let url = 'https://285l3svbjf.execute-api.ap-southeast-2.amazonaws.com/user/get_database';
  
  if (date === today) {
    url += '?limit=10';
  }
  
  const response = await fetch(url);
  const data_health = await response.json();
  
  // Phân loại dữ liệu theo ngày
  data_health.forEach(item => {
    const itemDate = new Date(item.timestamp).toISOString().split('T')[0];
    if (!dataByDate[itemDate]) {
      dataByDate[itemDate] = { temperature: [], behavior: [] };
    }
    dataByDate[itemDate].behavior.push(item);
  });
  
  return data_health;
}
// Temperature chart for past dates
function temperaturedrawPast(data) {
  if (!data || data.length === 0) {
    if (temperatureChart && temperatureChart.data && temperatureChart.data.datasets) {
      temperatureChart.data.datasets[0].data = [];
      temperatureChart.update();
    }
    return;
  }

  const sortedData = data.sort((a, b) => new Date(a.timestamp) - new Date(b.timestamp));
  
  const chartData = sortedData.map(item => ({
    x: new Date(item.timestamp),
    y: item.temp
  }));

  if (temperatureChart && temperatureChart.data && temperatureChart.data.datasets) {
    temperatureChart.data.datasets[0].data = chartData;
    
    const temperatures = chartData.map(item => item.y);
    const minTemp = Math.min(...temperatures);
    const maxTemp = Math.max(...temperatures);
    const padding = (maxTemp - minTemp) * 0.1;

    temperatureChart.options.scales.yAxes[0].ticks.min = Math.floor(minTemp - padding);
    temperatureChart.options.scales.yAxes[0].ticks.max = Math.ceil(maxTemp + padding);

    temperatureChart.update();
  }
}

// Temperature chart for current date
function temperaturedrawCurrent(data) {
  const sortedData = data.sort((a, b) => new Date(b.timestamp) - new Date(a.timestamp));
  const latestData = sortedData.slice(0, MAX_DATA_POINTS).map(item => item.temp);

  if (temperatureChart) {
    temperatureChart.data.datasets[0].data.push(latestData[0]);
    temperatureChart.data.datasets[0].data.shift();
    temperatureChart.data.labels = temperatureChart.data.labels.map((_, i) => i);
    temperatureChart.update();
  }
}

function initChartPast(ctx) {
  Chart.defaults.global.defaultFontFamily = 'Nunito', '-apple-system,system-ui,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif';
  Chart.defaults.global.defaultFontColor = '#858796';

  temperatureChart = new Chart(ctx, {
    type: 'line',
    data: {
      datasets: [{
        label: 'Temperature',
        data: [],
        borderColor: 'rgba(0, 92, 185, 1)',
        backgroundColor: 'rgba(0, 92, 185, 0.1)',
        borderWidth: 2.5,
        pointRadius: 3,
        pointHoverRadius: 5
      }]
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      layout: {
        padding: {
          left: 10,
          right: 25,
          top: 25,
          bottom: 0
        }
      },
      scales: {
        xAxes: [{
          type: 'time',
          time: {
            unit: 'hour',
            displayFormats: {
              hour: 'HH:mm'
            },
            tooltipFormat: 'll HH:mm'
          },
          distribution: 'linear',
          ticks: {
            maxTicksLimit: 24
          }
        }],
        yAxes: [{
          ticks: {
            maxTicksLimit: 5,
            padding: 10,
          },
          gridLines: {
            color: "rgb(234, 236, 244)",
            zeroLineColor: "rgb(234, 236, 244)",
            drawBorder: false,
            borderDash: [2],
            zeroLineBorderDash: [2]
          }
        }],
      },
      legend: {
        display: false
      },
      tooltips: {
        backgroundColor: "rgb(255,255,255)",
        bodyFontColor: "#858796",
        titleMarginBottom: 10,
        titleFontColor: '#6e707e',
        titleFontSize: 14,
        borderColor: '#dddfeb',
        borderWidth: 1,
        xPadding: 15,
        yPadding: 15,
        displayColors: false,
        intersect: false,
        mode: 'index',
        caretPadding: 10,
      }
    }
  });
}

function initChartCurrent(ctx) {
  temperatureChart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: Array.from({ length: MAX_DATA_POINTS }, (_, i) => i),
      datasets: [{
        label: 'Temperature',
        data: new Array(MAX_DATA_POINTS).fill(null),
        borderColor: 'rgba(0, 92, 185, 1)',
        backgroundColor: 'rgba(0, 0, 0, 0)',
        borderWidth: 2.5
      }]
    },
    options: {
      maintainAspectRatio: false,
      height: 300,
      legend: {
        display: false
      },
      scales: {
        xAxes: [{
          ticks: {
            display: false
          }
        }]
      },
      animation: {
        duration: 0
      }
    }
  });
}

function calculateLegendPosition(canvas, cutoutSize) {
  const canvasWidth = canvas.width;
  const canvasHeight = canvas.height;
  const legendHeight = 50;
  const minDimension = Math.min(canvasWidth, canvasHeight);
  const cutoutRadius = cutoutSize / 2;

  if (minDimension > cutoutSize + legendHeight) {
    return {
      position: 'bottom',
      y: canvasHeight - cutoutRadius - legendHeight / 2
    };
  } else {
    return {
      position: 'right',
      x: canvasWidth - cutoutRadius - 30
    };
  }
}

function calculateCutoutSize(canvas) {
  const canvasWidth = canvas.width;
  const canvasHeight = canvas.height;
  const minDimension = Math.min(canvasWidth, canvasHeight);
  const cutoutRadius = minDimension * 0.3;
  return cutoutRadius * 2;
}

function renderBehaviorChart(data) {
  if (data.length === 0) {
    const emptyData = {
      'lying chest': 0, 'sniffing': 0, 'galloping': 0,
      'walking': 0, 'trotting': 0, 'sitting': 0, 'standing': 0
    };
    updateBehaviorChartData(emptyData);
    return;
  }

  const behaviorCounts = {
    'lying chest': 0, 'sniffing': 0, 'galloping': 0,
    'walking': 0, 'trotting': 0, 'sitting': 0, 'standing': 0
  };

  data.forEach(item => {
    const behavior = item.behavior.toLowerCase();
    if (behaviorCounts.hasOwnProperty(behavior)) {
      behaviorCounts[behavior]++;
    }
  });

  updateBehaviorChartData(behaviorCounts);
}

function updateBehaviorChartData(behaviorCounts) {
  const behaviorLabels = Object.keys(behaviorCounts);
  const behaviorData = Object.values(behaviorCounts);

  const ctx = document.getElementById('behaviorChart').getContext('2d');
  const canvas = ctx.canvas;
  const cutoutSize = calculateCutoutSize(canvas);
  const legendPosition = calculateLegendPosition(canvas, cutoutSize);

  if (!behaviorChart) {
    behaviorChart = new Chart(ctx, {
      type: 'doughnut',
      data: {
        labels: behaviorLabels,
        datasets: [{
          data: behaviorData,
          backgroundColor: [
            '#4e73df', '#1cc88a', '#36b9cc', '#f6c23e',
            '#e74a3b', '#858796', '#6610f2'
          ],
          hoverBackgroundColor: [
            '#2e59d9', '#17a673', '#2c9faf', '#f6b820',
            '#e02d1b', '#6c757d', '#4e08b6'
          ],
          hoverBorderColor: 'rgba(234, 236, 244, 1)'
        }]
      },
      options: {
        maintainAspectRatio: false,
        tooltips: {
          backgroundColor: 'rgb(255,255,255)',
          bodyFontColor: '#858796',
          borderColor: '#dddfeb',
          borderWidth: 1,
          xPadding: 15,
          yPadding: 15,
          displayColors: false,
          caretPadding: 10
        },
        legend: {
          display: true,
          position: legendPosition.position,
          align: legendPosition.position === 'right' ? 'start' : 'center',
          labels: {
            boxWidth: 12
          },
          x: legendPosition.x,
          y: legendPosition.y
        },
        cutoutPercentage: (cutoutSize / Math.min(canvas.width, canvas.height)) * 100
      }
    });
  } else {
    behaviorChart.data.labels = behaviorLabels;
    behaviorChart.data.datasets[0].data = behaviorData;
    behaviorChart.update();
  }
}

function renderHealthCalendar(data) {
  const calendarEl = document.getElementById('calendar');
  const calendarContainer = calendarEl.parentNode;

  const existingLegend = calendarContainer.querySelector('.health-legend');
  if (existingLegend) {
    calendarContainer.removeChild(existingLegend);
  }

  const calendar = new FullCalendar.Calendar(calendarEl, {
    initialView: 'dayGridMonth',
    events: [],
    dateClick: function(info) {
      updateChartsForDate(info.dateStr);
    },
    height: 300,
    eventContent: function(arg) {
      let arrayOfDomNodes = [];
      let titleElement = document.createElement('div');
      titleElement.classList.add('fc-event-title');
      titleElement.style.fontSize = '12px';
      titleElement.innerText = arg.event.title;
      arrayOfDomNodes.push(titleElement);
      return { domNodes: arrayOfDomNodes };
    }
  });
  calendar.render();

  const eventData = [];
  const healthCounts = {};

  data.forEach(item => {
    const itemDate = new Date(item.timestamp);
    const dateStr = itemDate.toISOString().split('T')[0];
    const health = item.health;

    if (!healthCounts[dateStr]) {
      healthCounts[dateStr] = {
        'Excellent': 0, 'Good': 0, 'Normal': 0, 'Bad': 0
      };
    }

    healthCounts[dateStr][health]++;
  });

  for (const date in healthCounts) {
    const counts = healthCounts[date];
    const maxHealth = Object.keys(counts).reduce((a, b) => counts[a] > counts[b] ? a : b);
    const color = maxHealth === 'Excellent' ? 'blue' : maxHealth === 'Good' ? 'green' : maxHealth === 'Normal' ? 'yellow' : 'red';

    eventData.push({
      title: '',
      start: date,
      color: color
    });
  }

  calendar.removeAllEvents();
  calendar.addEventSource(eventData);

  const legendContainer = document.createElement('div');
  legendContainer.classList.add('health-legend');
  legendContainer.style.marginTop = '20px';

  const legendCanvas = document.createElement('canvas');
  const ctx = legendCanvas.getContext('2d');
  const healthColors = [
    [
      { color: 'green', label: 'Good' },
      { color: 'yellow', label: 'Normal' },
      { color: 'red', label: 'Bad' }
    ],
    [
      { color: 'blue', label: 'Excellent' }
    ]
  ];
  
  const legendWidth = 300;
  const legendHeight = 80;
  const radius = 10;
  const spacing = 30;
  const fontSize = 16;
  const lineHeight = 40;
  legendCanvas.width = legendWidth;
  legendCanvas.height = legendHeight;

  ctx.font = `${fontSize}px Arial`;
  
  healthColors.forEach((row, rowIndex) => {
    let x = radius + spacing;
    const y = legendHeight / 3 + rowIndex * lineHeight;
    
    row.forEach(({ color, label }) => {
      ctx.beginPath();
      ctx.arc(x, y, radius, 0, 2 * Math.PI);
      ctx.fillStyle = color;
      ctx.fill();
      ctx.fillStyle = 'black';
      ctx.textAlign = 'left';
      ctx.textBaseline = 'middle';
      ctx.fillText(label, x + radius*2, y);
      x += radius * 2 + spacing*1.2 + ctx.measureText(label).width;
    });
  });

  legendContainer.appendChild(legendCanvas);
  calendarContainer.appendChild(legendContainer);
}

let currentViewDate = new Date().toISOString().split('T')[0];
let temperatureUpdateInterval;
let behaviorUpdateInterval;

function updateChartsForDate(date) {
  currentViewDate = date;
  const today = new Date().toISOString().split('T')[0];
  
  clearInterval(temperatureUpdateInterval);
  clearInterval(behaviorUpdateInterval);
  
  const temperatureChartElement = document.getElementById('temperatureChart');
  const ctx = temperatureChartElement.getContext('2d');
  
  if (date === today) {
    initChartCurrent(ctx);
    fetchAndUpdateDatatemp(date);
    fetchAndUpdateDatabehavior(date);
    temperatureUpdateInterval = setInterval(() => fetchAndUpdateDatatemp(date), 1000);
    behaviorUpdateInterval = setInterval(() => fetchAndUpdateDatabehavior(date), 1000);
  } else if (dataByDate[date]) {
    initChartPast(ctx);
    temperaturedrawPast(dataByDate[date].temperature);
    renderBehaviorChart(dataByDate[date].behavior);
  } else {
    initChartPast(ctx);
    temperaturedrawPast([]);
    renderBehaviorChart([]);
  }
}

async function fetchAndUpdateDatatemp(date) {
  const data = await fetchData(date);
  const today = new Date().toISOString().split('T')[0];
  
  if (date === today) {
    temperaturedrawCurrent(dataByDate[today]?.temperature || []);
  }
}

async function fetchAndUpdateDatabehavior(date) {
  const data_health = await fetchData_health(date);
  const today = new Date().toISOString().split('T')[0];
  
  if (date === today) {
    renderBehaviorChart(dataByDate[today]?.behavior || []);
  }
  renderHealthCalendar(data_health);
}

// Alert 
let alerts = [];
let unreadAlertsCount = 0;
let readAlerts = [];

function getLatestValueByTimestamp(data, key) {
  if (Array.isArray(data)) {
    const sortedData = data.sort((a, b) => new Date(b.timestamp) - new Date(a.timestamp));
    const latestItem = sortedData[0];
    return latestItem[key];
  } else {
    return null;
  }
}

function getLatestValueByTimestamp_health(data, key) {
  if (Array.isArray(data)) {
    const sortedData = data.sort((a, b) => new Date(b.timestamp) - new Date(a.timestamp));
    const latestItem = sortedData[0];
    return latestItem[key];
  } else {
    return null;
  }
}

function generateTemperatureAlert(data) {
  const temperature = getLatestValueByTimestamp(data, 'temp');
  const timestamp = getLatestValueByTimestamp(data, 'timestamp');
  if (temperature > 40) {
    return {
      type: 'danger',
      icon: 'fas fa-temperature-high',
      message: `High temperature alert: ${temperature}°C`,
      timestamp: new Date(timestamp)
    };
  } else if (temperature < 36) {
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

async function updateAlert() {
  const data = await fetchData();
  const data_health = await fetchData_health();
  updateAlerts(data, data_health);
}

setInterval(updateAlert, 1000);
updateAlert();

document.addEventListener('DOMContentLoaded', () => {
  const temperatureChartElement = document.getElementById('temperatureChart');
  if (temperatureChartElement) {
    const ctx = temperatureChartElement.getContext('2d');
    updateChartsForDate(currentViewDate);
  }
  const alertsDropdown = document.getElementById('alertsDropdown');
  if (alertsDropdown) {
    alertsDropdown.addEventListener('click', handleAlertClick);
  }
});