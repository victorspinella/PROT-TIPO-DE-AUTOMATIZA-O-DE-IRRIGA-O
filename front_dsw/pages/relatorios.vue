<template>
  <Header />
  <div class="table-pesquisa">
    <table class="custom-table-pesquisa">
      <thead>
        <tr>
          <th>Data Incial</th>
          <th>Data Final</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td class="table-pesquisa-datas"><input v-model="dataInicio" class="datas" type="date" id="dataInicio"></td>
          <td class="table-pesquisa-datas"><input v-model="dataFinal" class="datas" type="date" id="dataFinal"></td>
          <td><button class="button" @click="listarRelatorios">Listar Relatórios</button></td>
        </tr>
      </tbody>
    </table>
  </div>

  <div class="page-container">
    <br>
    <div v-if="listaRelatorios.length !== 0" class="table-container">
      <table class="custom-table">
        <thead>
          <tr>
            <th>ID Estufa</th>
            <th>Nome Estufa</th>
            <th>Localização</th>
            <th>Capacidade</th>
            <th>Status Bomba</th>
            <th>Status Válvula</th>
            <th>Corrente Bomba</th>
            <th>Umidade Solo</th>
            <th>Umidade Ambiente</th>
            <th>Temperatura Ambiente</th>
            <th>Data</th>
            <th>Hora</th>
          </tr>
        </thead>
        <tbody>
          <tr v-for="relatorio in listaRelatorios" :key="relatorio.LeituraID">
            <td>{{ relatorio.EstufaID }}</td>
            <td>{{ relatorio.Estufa ? relatorio.Estufa.nome : 'N/A' }}</td>
            <td>{{ relatorio.Estufa ? relatorio.Estufa.localizacao : 'N/A' }}</td>
            <td>{{ relatorio.Estufa ? relatorio.Estufa.capacidade : 'N/A' }}</td>
            <td>{{ relatorio.StatusBomba }}</td>
            <td>{{ relatorio.StatusValvula }}</td>
            <td>{{ relatorio.CorrenteBomba }}</td>
            <td>{{ relatorio.UmidadeSolo }}</td>
            <td>{{ relatorio.UmidadeAmbiente }}</td>
            <td>{{ relatorio.TemperaturaAmbiente }}</td>
            <td>{{ formatDate(relatorio.DataLeitura) }}</td>
            <td>{{ formatTime(relatorio.DataLeitura) }}</td>
            <td><button class="button-delete" @click="excluirRelatorio(relatorio.LeituraID)">Excluir</button></td>
          </tr>
        </tbody>
      </table>
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue';
import { api } from '../infra/axios-config';

const listaRelatorios = ref([]);
const urlRelatorios = ref();
const dataInicio = ref(null);
const dataFinal = ref(null);

const urlPrincipal = 'http://localhost:4000';

const listarRelatorios = async () => {
  urlRelatorios.value = `${urlPrincipal}/relatorios?dataInicio=${dataInicio.value}&dataFim=${dataFinal.value}`;

  if (!dataInicio.value && !dataFinal.value) {
    urlRelatorios.value = `${urlPrincipal}/relatorio`;
  } else if (!dataInicio.value || !dataFinal.value) {
    window.alert("Selecione as duas datas");
    return;
  }

  try {
    const response = await api.get(urlRelatorios.value);
    console.log(response);
    listaRelatorios.value = response.data;
  } catch (error) {
    console.error('Erro ao buscar dados:', error);
    if (error.response && error.response.status === 401) {
      window.alert('Usuário não autenticado');
    }
  }
};

const excluirRelatorio = async (id) => {
  try {
    const response = await api.delete(`${urlPrincipal}/deleterelatorio/${id}`);
    listarRelatorios();
    console.log(response);
    window.alert("Relatório Excluído");
  } catch (error) {
    if (error.response && error.response.status === 401) {
      window.alert('Usuário não autenticado');
    }
    window.alert("Erro ao excluir relatório");
    console.log(error);
  }
};

const formatDate = (input) => {
  const date = new Date(input);
  const day = String(date.getDate()).padStart(2, '0');
  const month = String(date.getMonth() + 1).padStart(2, '0');
  const year = date.getFullYear();
  return `${day}/${month}/${year}`;
};

const formatTime = (input) => {
  const date = new Date(input);
  const hours = String(date.getHours()).padStart(2, '0');
  const minutes = String(date.getMinutes()).padStart(2, '0');
  const seconds = String(date.getSeconds()).padStart(2, '0');
  return `${hours}:${minutes}:${seconds}`;
};
</script>

<style scoped>
.table-pesquisa {
  display: flex;
  justify-content: center;
  align-items: center;
  column-gap: 5%;
  padding-top: 5%;
}

.datas {
  border: 2px solid black;
  padding: 10px;
  border-radius: 5px;
}

.table-pesquisa-datas {
  padding-right: 100%;
}

.page-container {
  display: flex;
  justify-content: flex-start;
  align-items: center;
  height: 100vh;
  flex-direction: column;
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

.custom-table-pesquisa {
  width: 60%;
  border-collapse: collapse;
}

.custom-table-pesquisa th,
.custom-table-pesquisa td {
  text-align: start;
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

.button {
  margin: 10px;
  padding: 10px 20px;
  background-color: #007BFF;
  color: #fff;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  transition: background-color 0.3s ease;
  width: 170px;
}

.button:hover {
  background-color: #75aae2;
}

.button-delete {
  margin: 10px;
  padding: 10px 20px;
  background-color: #007BFF;
  color: #fff;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  transition: background-color 0.3s ease;
  width: 100px;
}

.button-delete:hover {
  background-color: #75aae2;
}

.manual-buttons button {
  margin: 5px;
  padding: 10px 20px;
  background-color: #007BFF;
  color: #fff;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  transition: background-color 0.3s ease;
  width: 100px;
}

.manual-buttons button:hover {
  background-color: #75aae2;
}
</style>
