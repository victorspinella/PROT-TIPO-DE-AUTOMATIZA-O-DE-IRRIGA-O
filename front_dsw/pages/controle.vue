<template>
  <Header />
  <div class="page-container">
    <div class="switch-container">
      <label class="switch">
        <input type="checkbox" v-model="autoIrrigacao" @change="toggleAutoIrrigacao">
        <span class="slider round"></span>
      </label>
      <div class="switch-labels">
        <!-- <span>Desligado</span>
        <span>Ligado</span> -->
      </div>
    </div>
    <br>
    <div>{{ mensagemPadrao }}</div>
    <div v-if="listaControles.length !== 0" class="table-container">
      <table class="custom-table">
        <thead>
          <tr>
            <th>Estufa</th>
            <th>Bomba</th>
            <th>Válvula</th>
          </tr>
        </thead>
        <tbody>
          <tr v-for="controle in listaControles" :key="controle.id">
            <td>{{ controle.estufa_id }}</td>
            <td :class="{ 'ativo': controle.comando_bomba, 'inativo': !controle.comando_bomba }">
              {{ controle.comando_bomba ? "Ativa" : "Inativa" }}
            </td>
            <td :class="{ 'ativo': controle.comando_valvula, 'inativo': !controle.comando_valvula }">
              {{ controle.comando_valvula ? "Ativa" : "Inativa" }}
            </td>
          </tr>
        </tbody>
      </table>
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue';
import { api } from '../infra/axios-config';

const apiUrlControles = `http://localhost:4000`;
const mensagemPadrao = ref("");

const listaControles = ref([]);
const autoIrrigacao = ref(false);

const listarControles = async () => {
  try {
    const response = await api.get(`${apiUrlControles}/controlesIrrigacao`);
    listaControles.value = response.data;
  } catch (error) {
    console.error('Erro ao buscar dados:', error);
    if (error.response.status === 401) {
      window.alert('Usuário não autenticado');
    }
  }
};

const toggleAutoIrrigacao = async () => {
  try {
    if (autoIrrigacao.value) {
      await api.put(`${apiUrlControles}/ligar/controles/auto`);
    } else {
      await api.put(`${apiUrlControles}/desligar/controles/auto`);
    }
    mensagemPadrao.value = autoIrrigacao.value ? 'Irrigação automática ligada' : 'Irrigação automática desligada';
  } catch (error) {
    console.error('Erro ao alternar irrigação automática:', error);
    if (error.response.status === 401) {
      window.alert('Usuário não autenticado');
    }
  }
};

onMounted(() => {
  listarControles();
});
</script>

<style scoped>
.page-container {
  display: flex;
  justify-content: center;
  align-items: center;
  height: 100vh;
  flex-direction: column;
}

.switch-container {
  display: flex;
  flex-direction: column;
  align-items: center;
}

.switch {
  position: relative;
  display: inline-block;
  width: 100px; /* Aumentado o tamanho */
  height: 50px; /* Aumentado o tamanho */
  margin: 20px 0; /* Adicionado espaço em volta do botão */
}

.switch input {
  opacity: 0;
  width: 0;
  height: 0;
}

.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #ccc;
  transition: .4s;
}

.slider:before {
  position: absolute;
  content: "";
  height: 42px; /* Aumentado o tamanho */
  width: 42px; /* Aumentado o tamanho */
  left: 4px;
  bottom: 4px;
  background-color: white;
  transition: .4s;
}

input:checked + .slider {
  background-color: #2196F3;
}

input:checked + .slider:before {
  transform: translateX(50px); /* Ajustado para corresponder ao novo tamanho */
}

.slider.round {
  border-radius: 34px;
}

.slider.round:before {
  border-radius: 50%;
}

.switch-labels {
  display: flex;
  justify-content: space-between;
  width: 100%;
}

.switch-labels span {
  width: 50%;
  text-align: center;
}

.table-container {
  width: 80%;
  margin-top: 20px;
  overflow-y: auto;
}

.custom-table {
  width: 100%;
  border-collapse: collapse;
}

.custom-table th,
.custom-table td {
  border: 1px solid #ddd;
  padding: 8px;
  text-align: center;
}

.custom-table th {
  background-color: #f2f2f2;
  font-weight: bold;
}

.ativo {
  background-color: green;
  color: white;
}

.inativo {
  background-color: red;
  color: white;
}
</style>
