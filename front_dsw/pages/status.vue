<template>
  <Header />
  <div class="page-container">
    <div class="charts-container">
      <div class="chart">
        <canvas id="temperaturaChart"></canvas>
      </div>
      <div class="chart">
        <canvas id="umidadeAmbienteChart"></canvas>
      </div>
      <div class="chart">
        <canvas id="umidadeSoloChart"></canvas>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue';
import { api } from '../infra/axios-config';
import { Chart, registerables } from 'chart.js';

Chart.register(...registerables);

const temperaturaData = ref([]);
const umidadeAmbienteData = ref([]);
const umidadeSoloData = ref([]);

const apiUrl = 'http://localhost:4000/tccarduino';

// Funções para buscar dados de cada endpoint
const fetchTemperaturaData = async () => {
  try {
    const response = await api.get(`${apiUrl}`);
    temperaturaData.value = response.data;
  } catch (error) {
    console.error('Erro ao buscar dados de temperatura:', error);
  }
};

const fetchUmidadeAmbienteData = async () => {
  try {
    const response = await api.get(`${apiUrl}`);
    umidadeAmbienteData.value = response.data;
  } catch (error) {
    console.error('Erro ao buscar dados de umidade do ambiente:', error);
  }
};

const fetchUmidadeSoloData = async () => {
  try {
    const response = await api.get(`${apiUrl}`);
    umidadeSoloData.value = response.data;
  } catch (error) {
    console.error('Erro ao buscar dados de umidade do solo:', error);
  }
};

// Função para renderizar os gráficos
const renderCharts = () => {
  new Chart(document.getElementById('temperaturaChart'), {
    type: 'line',
    data: {
      labels: temperaturaData.value.map(item => new Date(item.data).toLocaleString()),
      datasets: [{
        label: 'Temperatura (°C)',
        data: temperaturaData.value.map(item => item.TemperaturaAmbiente),
        borderColor: 'red',
        borderWidth: 1,
        fill: false
      }]
    }
  });

  new Chart(document.getElementById('umidadeAmbienteChart'), {
    type: 'line',
    data: {
      labels: umidadeAmbienteData.value.map(item => new Date(item.data).toLocaleString()),
      datasets: [{
        label: 'Umidade Ambiente (%)',
        data: umidadeAmbienteData.value.map(item => item.UmidadeAmbiente),
        borderColor: 'blue',
        borderWidth: 1,
        fill: false
      }]
    }
  });

  new Chart(document.getElementById('umidadeSoloChart'), {
    type: 'line',
    data: {
      labels: umidadeSoloData.value.map(item => new Date(item.data).toLocaleString()),
      datasets: [{
        label: 'Umidade Solo (%)',
        data: umidadeSoloData.value.map(item => item.UmidadeSolo),
        borderColor: 'green',
        borderWidth: 1,
        fill: false
      }]
    }
  });
};

onMounted(async () => {
  await Promise.all([
    fetchTemperaturaData(),
    fetchUmidadeAmbienteData(),
    fetchUmidadeSoloData()
  ]);
  renderCharts();
});
</script>

<style lang="scss" scoped>
.page-container {
  display: flex;
  justify-content: center;
  align-items: center;
  height: 100vh;
  flex-direction: column;
}

.charts-container {
  display: flex;
  justify-content: space-around;
  width: 100%;
  margin-top: 20px;
}

.chart {
  width: 30%;
}
</style>
